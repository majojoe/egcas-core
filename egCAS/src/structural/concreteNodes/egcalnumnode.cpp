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
#include "specialNodes/egccontainernode.h"
#include "egcalnumnode.h"
#include "utils/egcutfcodepoint.h"


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

QRegularExpression EgcAlnumNode::s_ampersand = QRegularExpression("(.*[^_]+)_2([^_]+.*)");
QRegularExpression EgcAlnumNode::s_ampersandBegin = QRegularExpression("_2([^_]+.*)");
QRegularExpression EgcAlnumNode::s_semi = QRegularExpression("(.*[^_]+)_3([^_]+.*)");
QRegularExpression EgcAlnumNode::s_semiBegin = QRegularExpression("(.*[^_]+)_3");
QRegularExpression EgcAlnumNode::s_validator = QRegularExpression("[_0-9a-zA-ZΆ-ώ]+");
bool EgcAlnumNode::s_regexInitialized = false;

EgcAlnumNode::EgcAlnumNode() : m_value(QString::null), m_firstCharMightBeNumber{false}
{
        //optimize all the regexes
        if (!s_regexInitialized) {
                optimizeRegexes();
        }
}

EgcAlnumNode::EgcAlnumNode(bool firstCharMightBeNumber) : EgcAlnumNode {}
{
        if (firstCharMightBeNumber)
                m_firstCharMightBeNumber = true;
}

EgcAlnumNode::~EgcAlnumNode()
{

}

void EgcAlnumNode::setValue(const QString& varName)
{
        m_value = varName;
}

void EgcAlnumNode::setStuffedValue(const QString& varName)
{
        m_value = decode(varName);
}

QString EgcAlnumNode::getValue(void) const
{
        return m_value;
}

QString EgcAlnumNode::getStuffedValue(void)
{
        return encode(m_value);
}

bool EgcAlnumNode::valid(void)
{
        if (m_value.isEmpty())
                return false;
        else
                return true;
}

bool EgcAlnumNode::operator==(const EgcNode& node) const
{
        bool retval = false;

        if (node.getNodeType() == EgcNodeType::AlnumNode) {
                if (static_cast<const EgcAlnumNode&>(node).getValue() == m_value)
                        retval = true;
        }

        return retval;
}

int EgcAlnumNode::nrSubindexes(void) const
{
        return m_value.size();
}

bool EgcAlnumNode::insert(QChar character, int position)
{
        bool retval = false;

        if (!m_firstCharMightBeNumber) {
                //it's not allowed for a alphanumeric name to begin with a number
                if (position == 0 && character.isDigit())
                        return false;
        }

        if (s_validator.match(character).hasMatch()) {
                if (position <= m_value.size() && position >= 0) {
                        m_value.insert(position, character);
                        retval = true;
                }
        }

        return retval;
}

bool EgcAlnumNode::remove(int position)
{
        bool retval = false;

        if (position >= 0 && position < m_value.size()) {
                if (!(position == 0 && m_value[1].isDigit())) { //it's not allowed that the first character of a name is a digit
                        retval = true;
                        m_value.remove(position, 1);
                } else if (m_firstCharMightBeNumber) { // if it's allowed that the first char can be a number
                        retval = true;
                        m_value.remove(position, 1);
                }
        }

        return retval;
}

void EgcAlnumNode::optimizeRegexes()
{
        if (!s_regexInitialized) {
                s_regexInitialized = true;
                s_ampersand.optimize();
                s_ampersandBegin.optimize();
                s_semi.optimize();
                s_semiBegin.optimize();
        }
}

bool EgcAlnumNode::visibleSigns(EgcNodeSide side) const
{
        return true;
}

bool EgcAlnumNode::isAtomicallyBoundChild(void) const
{
        EgcContainerNode* parent = getParent();
        if (!parent)
                return false;

        return parent->determineIfChildIsAtomicallyBound(this);
}

QString EgcAlnumNode::encode(const QString& str)
{
        //replace (stuffing) of "_" with "__" since "_" is used to concatenate variable name and subscript
        QString tmp = EgcUtfCodepoint::encodeToXml(str).replace("_", "__");

        // ampersands and followning '#' in special symbols are replaced by "_2" for use in calculation kernel
        tmp = tmp.replace("&#", "_2");
        // ";" in special symbols are replaced by "_3" for use in calculation kernel
        tmp = tmp.replace(";", "_3");

        return tmp;
}

QString EgcAlnumNode::decode(const QString& str)
{
        /*the regexes are copied from static expressions since copying is cheap but we don't need to build up the
        regexes on the stack*/
        //handle ampersands

        //optimize all the regexes if not done so far
        if (!s_regexInitialized) {
                optimizeRegexes();
        }

        QString tmp = str;
        tmp = tmp.replace(QRegularExpression(s_ampersand), "\\1&#\\2");
        tmp.replace(QRegularExpression(s_ampersandBegin), "&#\\1"); //if the ampersand is at the beginning
        //handle ";"s
        tmp.replace(QRegularExpression(s_semi), "\\1;\\2");
        tmp.replace(QRegularExpression(s_semiBegin), "\\1;");  //if the ";" is at the end

        tmp.replace("__", "_");

        return EgcUtfCodepoint::decodeToUtf(tmp);

}
