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

#ifndef EGCSCREENPOS_H
#define EGCSCREENPOS_H

#include <QHash>
#include <QPointF>
#include "specialNodes/egcnode.h"
#include "libegcas/eg_mml_document.h"


class EgcScreenPos
{
public:
        EgcScreenPos();
        /**
         * @brief setLookup set the lookup table, to be able to lookup the mathml id with the formula item nodes
         * @param lookup the lookup hash table (mathml id, EgcNode*)
         */
        void setMathmlMapping(QHash<quint32, EgcNode*> lookup);
        /**
         * @brief setPositions set the positions of the characters of the formula that have been calculated during
         * rendering
         * @param positions the positions of the chars, mathml id's and subindexes within the id's
         */
        void setPositions(QVector<EgRenderingPosition> positions);
        /**
         * @brief getNodeAtPos returns the node that fits best for the given position
         * WARNING: This function can be expensive.
         * @param pos the position inside the formula anybody e.g. clicked on
         * @param subIndex the subindex that is accompanied by the mathml id (EgNode*)
         * @return the node that is picked for the given position
         */
        EgcNode* getNodeAtPos(const QPointF &pos, int* subIndex);
        /**
         * @brief getNode returns the node that is related with the given mathml id
         * @param mathMlId the mathml id we search the node for
         * @return the node for the given mathml id, or (if not found) nullptr
         */
        EgcNode* getNode(quint32 mathMlId);
private:

        QHash<quint32, EgcNode*> m_lookup;      ///< the lookup table for looking up the node for a mathml id
        QVector<EgRenderingPosition> m_positions; ///< the rendering positions of the characters of a formula
};

#endif // EGCSCREENPOS_H
