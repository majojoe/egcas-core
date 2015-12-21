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

#include <QMultiHash>

class EgcNode;


class EgcMathmlLookup
{
public:
        EgcMathmlLookup();
        /**
         * @brief addId add a id <-> node pair for later lookup
         * @param id the mathml node id to add
         * @param node the node that is related to the also given mathml node id above
         * @param visibility if and where a node has visible elements
         */
        void addId(EgcNode& node, quint32 id);
        /**
         * @brief clear clears the lookup table
         */
        void clear(void);
        /**
         * @brief getIdFrame returns the id of the frame of an mathml element
         * @param node the node we want the id of the frame for
         * @return the id associated with the frame of the given element or 0 if no id was found for the given node
         */
        quint32 getIdFrame(EgcNode& node) const;
        /**
         * @brief getIds returns a list with all mathml id's of the given node
         * @param node the node we want the id's for
         * @return a list with the id's associated with the given node. The list can be empty.
         */
        QList<quint32> getIds(EgcNode& node) const;
        /**
         * @brief getIdsNonFrame returns a list with all mathml id's associated with the given node without the frame id.
         * @param node is the node we want the id's for
         * @return a list with the id's without the frame id of the mathml element. The list can be empty.
         */
        QList<quint32> getIdsNonFrame(EgcNode& node) const;
        /**
         * @brief getIdCount returns the count of id's that are saved for the given node
         * @param node the node we want to know the number of id's for
         * @return the number of id's for the given node
         */
        int getIdCount(EgcNode& node) const;
        /**
         * @brief findNode returns the node for the given id. Warning: This function can be slow!
         * @param id the mathml id we want the node for
         * @return the node found for the given id, or nullptr in case when the node has not been found
         */
        EgcNode* findNode(quint32 id) const;
private:
        QMultiHash<EgcNode*, quint32> m_lookup;      ///< lookup to be able to make a relation of any mathml id to the nodes of the formula
};

#endif // EGCMATHMLLOOKUP_H
