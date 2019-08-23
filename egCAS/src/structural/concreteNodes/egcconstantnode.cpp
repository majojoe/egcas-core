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

#include "egcnodecreator.h"
#include <QString>
#include <QStringBuilder>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "egcalnumnode.h"
#include "egcconstantnode.h"


bool EgcConstantNode::decode(QString representation, Constants &constant)
{
        bool ret = false;

        if (representation.startsWith("_const_")) {
                QString tmp = representation.remove(0, 7);
                if (tmp == "e") {
                        constant = Constants::e;
                        ret = true;
                } else if (tmp == "i") {
                        constant = Constants::i;
                        ret = true;
                } else if (tmp == "infinity") {
                        constant = Constants::infinity;
                        ret = true;
                } else if (tmp == "pi") {
                        constant = Constants::pi;
                        ret = true;
                } else {
                        constant = Constants::undefined;
                }
        }
}

QString EgcConstantNode::encode(Constants constant)
{
        QString ret("_const_");
        switch (constant) {
                case Constants::e:
                ret = ret % QString("e");
                break;
                case Constants::i:
                ret = ret % QString("i");
                break;
                case Constants::infinity:
                ret = ret % QString("infinity");
                break;
                case Constants::pi:
                ret = ret % QString("pi");
                break;
                case Constants::undefined:
                ret = ret % QString("undefined");
                break;
        }
        return ret;
}

bool EgcConstantNode::setConstantFromName(const QString representation)
{
        return decode(representation, m_constant);
}

EgcConstantNode::EgcConstantNode() : m_constant(Constants::undefined)
{

}

EgcConstantNode::EgcConstantNode(Constants constant) : m_constant(constant)
{

}

EgcConstantNode::EgcConstantNode(QString representation)
{
        if (!setConstantFromName(representation))
                m_constant = Constants::undefined;
}

EgcConstantNode::~EgcConstantNode()
{

}

void EgcConstantNode::setValue(Constants constant)
{
        m_constant = constant;
}

QString EgcConstantNode::getStringRepresentation()
{
        return encode(m_constant);
}

Constants EgcConstantNode::getValue(void) const
{
        return m_constant;
}

bool EgcConstantNode::valid(void)
{
        if (m_constant == Constants::undefined)
                return false;

        return true;
}

bool EgcConstantNode::operator==(const EgcNode& node) const
{
        if (node.getNodeType() != EgcNodeType::ConstantNode)
                return false;

        const EgcConstantNode& const_node = static_cast<const EgcConstantNode&>(node);

        if (m_constant == const_node.getValue())
                return true;

        return false;
}

bool EgcConstantNode::isOperation(void) const
{
        return false;
}

void EgcConstantNode::serialize(QXmlStreamWriter& stream, SerializerProperties& properties)
{
        (void)properties;
        QLatin1String str = EgcNodeCreator::stringize(getNodeType());
        if (str.size() != 0) {
                stream.writeStartElement(str);
                stream.writeCharacters(getStringRepresentation());
                stream.writeEndElement();
        }
}

void EgcConstantNode::deserialize(QXmlStreamReader& stream, SerializerProperties& properties)
{
        (void) properties;

        setConstantFromName(stream.readElementText());
}
