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
#include <QXmlStreamReader>
#include <QXmlStreamWriter>


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

QRegularExpression EgcAlnumNode::s_html_encoding_start = QRegularExpression("(_2)([0-9]+_3)");
QRegularExpression EgcAlnumNode::s_html_encoding_end = QRegularExpression("(&#[0-9]+)(_3)");
QRegularExpression EgcAlnumNode::s_validator = QRegularExpression("[_0-9a-zA-ZΆ-ώ]+");
QRegularExpression EgcAlnumNode::s_alnumChecker = QRegularExpression("^[_0-9a-zA-ZΆ-ώ]+$");
bool EgcAlnumNode::s_regexInitialized = false;

EgcAlnumNode::EgcAlnumNode() : m_value(QString()), m_firstCharMightBeNumber{false}
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

void EgcAlnumNode::optimizeRegexes()
{
        if (!s_regexInitialized) {
                s_regexInitialized = true;
                s_html_encoding_start.optimize();
                s_html_encoding_end.optimize();
                s_validator.optimize();
                s_alnumChecker.optimize();
        }
}

void EgcAlnumNode::serializeAttributes(QXmlStreamWriter& stream)
{
        stream.writeAttribute("value", m_value);
        if (m_firstCharMightBeNumber)
                stream.writeAttribute("firstCharMightBeNumber", QString("true"));
        else
                stream.writeAttribute("firstCharMightBeNumber", QString("false"));
}

void EgcAlnumNode::deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr)
{
        (void) stream;
        (void) version;

        if (attr.hasAttribute("value"))
                setValue(attr.value("value").toString());
        if (attr.hasAttribute("firstCharMightBeNumber")) {
                if (attr.value("firstCharMightBeNumber").toString() == QString("true"))
                        m_firstCharMightBeNumber = true;
                else
                        m_firstCharMightBeNumber = false;
        }
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
        tmp = tmp.replace(QRegularExpression(s_html_encoding_start), "&#\\2");
        tmp = tmp.replace(QRegularExpression(s_html_encoding_end), "\\1;");

        tmp.replace("__", "_");

        return EgcUtfCodepoint::decodeToUtf(tmp);

}

bool EgcAlnumNode::isAlnum(const QString& str)
{
        if (s_alnumChecker.match(str).hasMatch())
                return true;

        return  false;
}
