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

#include "egcnodecreator.h"
#include "egcmatrixnode.h"
#include "structural/concreteNodes/egcvariablenode.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>



EgcMatrixNode::EgcMatrixNode() : m_rows{1}, m_columns{1}
{
}

bool EgcMatrixNode::valid(void)
{
        return EgcFlexNode::valid();
}

void EgcMatrixNode::setDimension(quint16 columns, quint16 rows)
{
        if (columns == 0)
                columns = 1;
        if (rows == 0)
                rows = 1;
        m_columns = columns;
        m_rows = rows;
}

quint16 EgcMatrixNode::rows(void) const
{
        return m_rows;
}

quint16 EgcMatrixNode::columns(void) const
{
        return m_columns;
}

EgcNode *EgcMatrixNode::getMatrixChild(quint32 index) const
{
        return getChild(index);
}

bool EgcMatrixNode::setMatrixChild(quint32 index, EgcNode &expression)
{
        return setChild(index, expression);
}

void EgcMatrixNode::serializeAttributes(QXmlStreamWriter& stream)
{
        QLatin1String str = EgcNodeCreator::stringize(getNodeType());
        if (str.size() != 0) {
                stream.writeAttribute("rows", QString::number(m_rows));
                stream.writeAttribute("columns", QString::number(m_columns));
        }
}

void EgcMatrixNode::deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr)
{
        (void) stream;
        (void) version;

        attr = stream.attributes();
        QString subscr;
        if (attr.hasAttribute("rows"))
                m_rows = static_cast<quint16>(attr.value("rows").toInt());
        if (attr.hasAttribute("columns"))
                m_columns = static_cast<quint16>(attr.value("columns").toInt());
}
