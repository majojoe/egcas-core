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
#include "iterator/egcscrpositerator.h"

class EgcNode;

/**
 * @brief The EgcScrPosIds struct organizes the data to be able to lookup the mathml id's in a hash table
 */
struct EgcScrPosIds
{
        quint32 m_mathmlId;                     ///< the mathml id that identifies a mathml node
        EgcScrPosPosition m_visibility;       ///< where the mathml id has visible elements
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
         * @param visibility if and where a node has visible elements
         */
        void addId(EgcNode& node, quint32 id, EgcScrPosPosition visibility);
        /**
         * @brief clear clears the lookup table
         */
        void clear(void);
        /**
         * @brief getId get the mathml id for the given node
         * @param node the node to lookup the id for
         * @return the mathml id that is associated with the node given, or 0 if not found
         */
        quint32 getId(const EgcNode& node);
        /**
         * @brief isVisible checks if the node is visible at the given position (left, right, left and right, ...)
         * @param node the node to lookup the visibility for
         * @param position the visibility at the given position will be returned
         * @return true if the node has visible parts (or is manipulable at the given position), false if not
         */
        bool isVisible(const EgcNode& node, EgcScrPosPosition position);
        /**
         * @brief remove removes the given node from the lookup if stored
         * @param node the node to remove
         */
        void remove(EgcNode* node);

private:
        QHash<const EgcNode*, EgcScrPosIds> m_hash;   ///< hash table for lookup if and where a node has visible elements
};

#endif // EGCMATHMLLOOKUP_H
