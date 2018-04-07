/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#include <QString>
#include <QStringBuilder>
#include <QRegularExpression>
#include "egcalnumnode.h"
#include "egcvariablenode.h"
#include "structural/specialNodes/egcemptynode.h"
#include "utils/egcutfcodepoint.h"


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

const char emptyElement[] = "_empty";

QRegularExpression EgcVariableNode::s_varSubSeparator = QRegularExpression("(.*[^_]+)"
                                                                           % EgcVariableNode::getStuffedVarSeparator()
                                                                           % "(.+)");
bool EgcVariableNode::s_initializeRegex = true;

EgcVariableNode::EgcVariableNode() : m_value(QString::null), m_subscript(QString::null), m_subscrIsEmpty(false)
{
        //optimize all the regexes
        if (s_initializeRegex) {
                s_initializeRegex = false;
                s_varSubSeparator.optimize();
                EgcAlnumNode::optimizeRegexes();
        }        
}

EgcVariableNode::~EgcVariableNode()
{

}

void EgcVariableNode::setValue(const QString& varName, const QString& subscript)
{
        m_value = varName;
        m_subscript = subscript;
        if (EgcEmptyNode::isEmptyValue(subscript))
                m_subscrIsEmpty = true;
}

void EgcVariableNode::setStuffedVar(const QString& varName)
{
        QString tmp = varName;
        QString value;
        QString subscript;

        /*the regexes are copied from static expressions since copying is cheap but we don't need to build up the
        regexes on the stack*/
        QRegularExpressionMatch regexMatch = s_varSubSeparator.match(tmp);
        if (regexMatch.hasMatch()) {
                value = regexMatch.captured(1);
                subscript = regexMatch.captured(2);

                m_value = EgcAlnumNode::decode(value);
                if (subscript == QString(emptyElement)) {
                        m_subscrIsEmpty = true;
                        m_subscript = EgcEmptyNode::getEmptyValue();
                } else {
                        m_subscript = EgcAlnumNode::decode(subscript);
                }
        } else {
                m_value = EgcAlnumNode::decode(tmp);
                m_subscript = QString::null;
        }
}

QString EgcVariableNode::getValue(void) const
{
        return m_value;
}

QString EgcVariableNode::getSubscript(void) const
{
        return m_subscript;
}

QString EgcVariableNode::getStuffedValue(void)
{
        QString var;
        QString sub;

        var = EgcAlnumNode::encode(m_value);
        if (m_subscrIsEmpty)
                sub = m_subscript;
        else
                sub = EgcAlnumNode::encode(m_subscript);

        QString tmp;
        if (sub.isEmpty())
                tmp = var;
        else
                tmp = var % getStuffedVarSeparator() % sub;

        return tmp;
}

QString EgcVariableNode::getStuffedVarSeparator(void)
{
        return "_1";
}

bool EgcVariableNode::valid(void)
{
        if (m_value.isEmpty())
                return false;
        else
                return true;
}

bool EgcVariableNode::operator==(const EgcNode& node) const
{
        if (node.getNodeType() != EgcNodeType::VariableNode)
                return false;

        const EgcVariableNode& var_node = static_cast<const EgcVariableNode&>(node);

        if (    (m_value == var_node.getValue())
                        && (m_subscript == var_node.getSubscript()))
                return true;

        return false;
}

bool EgcVariableNode::isOperation(void) const
{
        return false;
}

bool EgcVariableNode::isSubscriptEmptyElement()
{
        return m_subscrIsEmpty;
}

