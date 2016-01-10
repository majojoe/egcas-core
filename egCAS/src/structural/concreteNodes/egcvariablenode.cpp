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
#include "egcvariablenode.h"
#include "utils/egcutfcodepoint.h"


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

QRegularExpression EgcVariableNode::s_ampersand = QRegularExpression("(.*[^_]+)_2([^_]+.*)");
QRegularExpression EgcVariableNode::s_ampersandBegin = QRegularExpression("_2([^_]+.*)");
QRegularExpression EgcVariableNode::s_semi = QRegularExpression("(.*[^_]+)_3([^_]+.*)");
QRegularExpression EgcVariableNode::s_semiBegin = QRegularExpression("(.*[^_]+)_3");
QRegularExpression EgcVariableNode::s_varSubSeparator = QRegularExpression("(.*[^_]+)_1([^_]+.*)");
bool EgcVariableNode::s_initializeRegex = true;

EgcVariableNode::EgcVariableNode() : m_value(QString::null), m_subscript(QString::null)
{
        //optimize all the regexes
        if (s_initializeRegex) {
                s_initializeRegex = false;
                s_ampersand.optimize();
                s_ampersandBegin.optimize();
                s_semi.optimize();
                s_semiBegin.optimize();
                s_varSubSeparator.optimize();
        }
}

EgcVariableNode::~EgcVariableNode()
{

}

void EgcVariableNode::setValue(const QString& varName, const QString& subscript = QString::null)
{
        m_value = varName;
        m_subscript = subscript;
}

void EgcVariableNode::setValueRaw(const QString& varName)
{
        QString tmp = varName;
        QString value;
        QString subscript;
        /*the regexes are copied from static expressions since copying is cheap but we don't need to build up the
        regexes on the stack*/
        //handle ampersands
        tmp.replace(QRegularExpression(s_ampersand), "\\1&#\\2");
        tmp.replace(QRegularExpression(s_ampersandBegin), "&#\\1"); //if the ampersand is at the beginning
        //handle ";"s
        tmp.replace(QRegularExpression(s_semi), "\\1;\\2");
        tmp.replace(QRegularExpression(s_semiBegin), "\\1;");  //if the ";" is at the end

        QRegularExpressionMatch regexMatch = s_varSubSeparator.match(tmp);
        if (regexMatch.hasMatch()) {
                value = regexMatch.captured(1);
                subscript = regexMatch.captured(2);
        } else {
                value = tmp;
                subscript = QString::null;
        }

        value.replace("__", "_");
        subscript.replace("__", "_");

        m_value = EgcUtfCodepoint::decodeToUtf(value);
        m_subscript = EgcUtfCodepoint::decodeToUtf(subscript);
}

QString EgcVariableNode::getValue(void) const
{
        return EgcUtfCodepoint::encodeToXml(m_value);
}

QString EgcVariableNode::getSubscript(void) const
{
        return EgcUtfCodepoint::encodeToXml(m_subscript);
}

QString EgcVariableNode::getStuffedVar(void)
{
        //replace (stuffing) of "_" with "__" since "_" is used to concatenate variable name and subscript
        QString var = EgcUtfCodepoint::encodeToXml(m_value).replace("_", "__");
        QString sub = EgcUtfCodepoint::encodeToXml(m_subscript).replace("_", "__");

        QString tmp;
        if (sub.isEmpty())
                tmp = var;
        else
                tmp = var % "_1" % sub;
        // ampersands and followning '#' in special symbols are replaced by "_2" for use in calculation kernel
        tmp = tmp.replace("&#", "_2");
        // ";" in special symbols are replaced by "_3" for use in calculation kernel
        tmp = tmp.replace(";", "_3");

        return tmp;
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
        bool retval = false;

        if (node.getNodeType() == EgcNodeType::VariableNode) {
                if (    (static_cast<const EgcVariableNode&>(node).getValue() == m_value)
                     && ((static_cast<const EgcVariableNode&>(node).getSubscript() == m_subscript)))
                        retval = true;
        }

        return retval;
}

int EgcVariableNode::nrSubindexes(void) const
{
        return m_value.size() + m_subscript.size();
}
