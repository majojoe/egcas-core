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

#ifndef EGCMATHMLLOOKUP_H
#define EGCMATHMLLOOKUP_H

#include <QVector>
#include <QHash>

class EgcNode;

/**
 * @brief The EgcMathmlIdMapping struct organizes the data to pack the contents in a container
 */
struct EgcMathmlIdMapping
{
        quint32 m_mathmlId;     ///< the mathml id that identifies a mathml node
        EgcNode* m_node;        ///< the formula element node it points to
};

class EgcMathmlLookup
{
        friend class EgcScrPosIterator;
public:
        EgcMathmlLookup();
        /**
         * @brief addId add a id <-> node pair for later lookup
         * @param id the mathml node id to add
         * @param node the node that is related to the also given mathml node id above
         */
        void addId(quint32 id, EgcNode& node);
        /**
         * @brief clear clears the lookup table
         */
        void clear(void);
        /**
         * @brief setMathmlIdSequence set the sequence in which the mathml id's follow each other
         * @param sequence the sequence of mathml id's given from the rendering library
         */
        void setMathmlIdSequence(QVector<quint32> sequence);
private:
        QHash<quint32, EgcNode*> m_tmpHash;        ///< temporary hash table (unsorted)
        QVector<EgcMathmlIdMapping> m_lookup;      ///< lookup to be able to make a relation of any mathml id to the nodes of the formula
};

#endif // EGCMATHMLLOOKUP_H
