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

#ifndef EGCSUBIDNODEITER_H
#define EGCSUBIDNODEITER_H

#include <QtGlobal>


class EgcNode;

/**
 * @brief The EgcSubidNodeIter class is a iterator for navigating between the formula glyphs
 */
class EgcSubindNodeIter
{
public:        
        /// constructor for initialization with formula, after initialization the iterator points to the beginning of
        /// the node.
        EgcSubindNodeIter(EgcNode& node);
        /// std destructor
        virtual ~EgcSubindNodeIter();
        /**
         * @brief setNode assign a new node to the iterator to work on.
         * @param node a reference to the node to set
         */
        void setNode(EgcNode& node);
        /**
         * @brief hasNext Checks if there is at most one more item after the current item.
         * @return True if an item was found, false otherwise.
         */
        bool hasNext(void) const;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @return True if an item was found, false otherwise.
         */
        bool hasPrevious(void) const;
        /**
         * @brief next Returns the next subid of a node and increments the iterator by one. Result is only valid, if
         * tested with hasNext() before.
         * @return a the next subid of a node.
         */
        int next(void);
        /**
         * @brief previous Returns the previous subid of a node and decrements the iterator by one.Result is only valid,
         * if tested with hasPrevious() before.
         * @return the previous subid of a node.
         */
        int previous(void);
        /**
         * @brief peekNext Returns the next subid of a node without incrementing the iterator.
         * @return the next subid of a node.
         */
        int peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous subid of a node without decrementing the iterator.
         * @return the previous subid of a node.
         */
        int peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the node (after the last item).
         */
        void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the node (before the first item).
         */
        void toFront(void);
        /**
         * @brief lastSubind returns the last subindex we jumped over
         * @return the last subindex we jumped over
         */
        int lastSubind(void);
        /**
         * @brief insert insert a character at the subIdIter's current position
         * @param character the character to insert
         * @return true if the operation was successful, false otherwise
         */
        bool insert(QChar character);

private:
        EgcNode* m_node;                ///< reference to node the iterator has been initialized with
        int m_nextId;                   ///< points to the next subid
        int m_prevId;                   ///< points to the previous subid
        int m_histId;                   ///< the last subind we jumped over

};

#endif // EGCSUBIDNODEITER_H
