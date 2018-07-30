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

#include "egcdifferentialnode.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>


EgcDifferentialNode::EgcDifferentialNode() : m_derivative{1}, m_differentialType{DifferentialType::leibnitz}
{

}

bool EgcDifferentialNode::valid(void)
{
        if (getDifferentialType() == DifferentialType::leibnitz) {
                if (m_childs.count() != 3)
                        return false;
                if (m_childs.at(0)) {
                        if (!m_childs.at(0)->valid())
                                return false;
                }
                if (m_childs.at(1)) {
                        if (!m_childs.at(1)->valid())
                                return false;
                }

        } else {
                return EgcFlexNode::valid();
        }

        return true;
}

void EgcDifferentialNode::setNrDerivative(quint8 derivative)
{
        if (derivative < 1)
                derivative = 1;
        m_derivative = derivative;
}

quint8 EgcDifferentialNode::getNrDerivative(void) const
{
        return m_derivative;
}

EgcDifferentialNode::DifferentialType EgcDifferentialNode::getDifferentialType() const
{
        return m_differentialType;
}

void EgcDifferentialNode::setDifferentialType(EgcDifferentialNode::DifferentialType type)
{
        m_differentialType = type;
}

void EgcDifferentialNode::serializeAttributes(QXmlStreamWriter& stream)
{
        stream.writeAttribute("derivative", QString("%1").arg(m_derivative));
        if (m_differentialType == DifferentialType::lagrange1)
                stream.writeAttribute("differential_type", "lagrange1");
        if (m_differentialType == DifferentialType::lagrange2)
                stream.writeAttribute("differential_type", "lagrange2");
        if (m_differentialType == DifferentialType::lagrange3)
                stream.writeAttribute("differential_type", "lagrange3");
        if (m_differentialType == DifferentialType::leibnitz)
                stream.writeAttribute("differential_type", "leibnitz");
}

void EgcDifferentialNode::deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr)
{
        (void) stream;
        (void) version;

        if (attr.hasAttribute("derivative"))
                setNrDerivative(static_cast<quint8>(attr.value("derivative").toUInt()));

        if (attr.hasAttribute("differential_type")) {
                QString type = attr.value("differential_type").toString();
                if (type == QString("lagrange1"))
                        setDifferentialType(DifferentialType::lagrange1);
                if (type == QString("lagrange2"))
                        setDifferentialType(DifferentialType::lagrange2);
                if (type == QString("lagrange3"))
                        setDifferentialType(DifferentialType::lagrange3);
                if (type == QString("leibnitz"))
                        setDifferentialType(DifferentialType::leibnitz);
        }
}
