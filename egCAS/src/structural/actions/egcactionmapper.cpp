/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include <QObject>
#include <QRegularExpression>
#include "egcactionmapper.h"

QRegularExpression EgcActionMapper::s_alnumKeyFilter = QRegularExpression("[._0-9a-zA-ZΆ-ώ]+");
bool EgcActionMapper::s_regexInitialized = false;

EgcActionMapper::EgcActionMapper()
{
        if (!s_regexInitialized) {
                s_regexInitialized = true;
                s_alnumKeyFilter.optimize();
        }
}

EgcAction EgcActionMapper::map(QKeyEvent *event)
{
        EgcAction action;
        action.m_op = EgcOperations::noAction;

        if (event == nullptr)
                return action;

        QString chr(event->text()[0]);
        if (chr.contains(s_alnumKeyFilter)) {
                action.m_op = EgcOperations::alnumKeyPressed;
                action.m_character = chr[0];

                return action;
        }

        if (event->modifiers()==Qt::ControlModifier) {
                switch(event->key()) {
                case Qt::Key_Minus:
                        action.m_op = EgcOperations::createSubId;
                        break;
                default:
                        action.m_op = EgcOperations::noAction;
                        break;
                }
                return action;
        }

        switch(event->key()) {
        case Qt::Key_Right:
                action.m_op = EgcOperations::cursorForward;
                break;
        case Qt::Key_Left:
                action.m_op = EgcOperations::cursorBackward;
                break;
        case Qt::Key_Space:
                action.m_op = EgcOperations::spacePressed;
                break;
        case Qt::Key_Backspace:
                action.m_op = EgcOperations::backspacePressed;
                break;
        case Qt::Key_Delete:
                action.m_op = EgcOperations::delPressed;
                break;
        case Qt::Key_ParenLeft:
                action = getMathOperationAction('(');
                break;
        case Qt::Key_ParenRight:
                action = getMathOperationAction(')');
                break;
        case Qt::Key_End:
                action.m_op = EgcOperations::endPressed;
                break;
        case Qt::Key_Home:
                action.m_op = EgcOperations::homePressed;
                break;
        case Qt::Key_Plus:
                action = getMathOperationAction('+');
                break;
        case Qt::Key_Minus:
                action = getMathOperationAction('-');
                break;
        case Qt::Key_Slash:
                action = getMathOperationAction('/');
                break;
        case Qt::Key_Asterisk:
                action = getMathOperationAction('*');
                break;
        case Qt::Key_Equal:
                action = getMathOperationAction('=');
                break;
        case Qt::Key_Colon:
                action = getMathOperationAction(':');
                break;

        default:
                action.m_op = EgcOperations::noAction;
                break;
        }
}

EgcAction EgcActionMapper::getMathOperationAction(QChar op)
{
        EgcAction action;

        action.m_op = EgcOperations::mathCharOperator;
        action.m_character = op;

        return action;
}

