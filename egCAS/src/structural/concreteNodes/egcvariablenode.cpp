/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <QString>
#include <QStringBuilder>
#include <QRegularExpression>
#include "egcvariablenode.h"

EgcVariableNode::EgcVariableNode() : m_value(QString::null), m_subscript(QString::null)
{
}

EgcVariableNode::~EgcVariableNode()
{

}

void EgcVariableNode::setValue(const QString& varName, const QString& subscript)
{
        m_value = varName;
        m_subscript = subscript;
}

void EgcVariableNode::setValueRaw(const QString& varName)
{
        QRegularExpression regex = QRegularExpression("(.*[^_]+)_([^_]+.*)");
        QRegularExpressionMatch regexMatch = regex.match(varName);
        if (regexMatch.hasMatch()) {
                QString tmp = regexMatch.captured(1);
                tmp.replace("__", "_");
                m_value = tmp;
                tmp = regexMatch.captured(2);
                tmp.replace("__", "_");
                m_subscript = tmp;
        } else {
                m_value = varName;
                m_value.replace("__", "_");
                m_subscript = QString::null;
        }
}

QString& EgcVariableNode::getValue(void)
{
        return m_value;
}

QString& EgcVariableNode::getSubscript(void)
{
        return m_subscript;
}

QString EgcVariableNode::getStuffedVar(void)
{
        //replace (stuffing) of "_" with "__" since "_" is used to concatenate variable name and subscript
        QString var = m_value.replace("_", "__");
        QString sub = m_subscript.replace("_", "__");

        return var % "_" % sub;
}

bool EgcVariableNode::valid(void)
{
        if (m_value.isEmpty())
                return false;
        else
                return true;
}
