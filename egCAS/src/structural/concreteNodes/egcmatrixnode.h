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

#ifndef EGCMATRIXNODE_H
#define EGCMATRIXNODE_H

#include <QString>
#include "egcfnccontainernode.h"


/**
 * @brief The EgcMatrixNode class represents a differential
 */
class EgcMatrixNode : public EgcFlexNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcMatrixNode, EgcNodeType::MatrixNode);
public:
        EgcMatrixNode();
        /**
         * @brief valid checks if the subnode is valid. This can be the case if e.g. the child is not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void) override;
        /**
         * @brief setDimension set the number of columns and rows of the matrix
         * @param columns
         * @param rows
         */
        void setDimension(quint16 columns, quint16 rows);
        /**
         * @brief rows returns the number of rows of the matrix
         * @return number of rows
         */
        quint16 rows(void) const;
        /**
         * @brief columns returns the number of columns of the matrix
         * @return number of columns
         */
        quint16 columns(void) const;
        /**
         * @brief interface for serializing the attributes of a formula operation
         * @param stream the stream to use for serializing this class
         */
        virtual void serializeAttributes(QXmlStreamWriter& stream) override;

        /**
         * @brief deserialize interface for deserializing the attributes of a formula operation
         * @param stream the xml reader stream
         * @param version the version of the stream that is to be deserialized
         * @param attr the xml attributes provided by the parent
         */
        virtual void deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr) override;

protected:
        quint16 m_rows;
        quint16 m_columns;
};

#endif // EGCMATRIXNODE_H
