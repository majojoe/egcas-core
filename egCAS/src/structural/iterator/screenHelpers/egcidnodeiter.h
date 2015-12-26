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

#ifndef EGCIDNODEITER_H
#define EGCIDNODEITER_H

#include <QtGlobal>
#include <QScopedPointer>
#include <QVector>
#include <QRectF>

class EgcNode;
class EgcFormulaEntity;
class EgcMathmlLookup;
class EgcNodeIterator;
enum class EgcIteratorState;


/**
 * @brief The EgcIdNodeIter class is a iterator for navigating between the formula glyphs
 */
class EgcIdNodeIter
{
public:        
        /// constructor for initialization with formula
        EgcIdNodeIter(const EgcFormulaEntity& formula);
        /// std destructor
        virtual ~EgcIdNodeIter();
        /**
         * @brief hasNext Checks if there is at most one more item after the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasNext(void) const;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasPrevious(void) const;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next mathml id.
         */
        virtual EgcNode &  next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous mathml id.
         */
        virtual EgcNode &  previous(void);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next mathml id.
         */
        virtual EgcNode& peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous mathml id.
         */
        virtual EgcNode& peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void);
        /**
         * @brief lastId returns the last id we jumped over.
         * @return the id we jumped over lastly.
         */
        virtual const quint32& id(void);
        /**
         * @brief getState retruns the state of the last traversion
         * @return state the state of the node we jumped over
         */
        virtual EgcIteratorState getLastState(void) const;

private:
        /**
         * @brief getMathmlId returns the mathml id of a node with a specified state and a predecessor or successor
         * @param node the node to check for existence of a mathml id
         * @param state the state of the node given
         * @param previousNode the node that is the predecessor of the given node in the formula tree
         * @param nextNode the node that is the successor of the given node in the formula tree
         * @return the mathml id for the given element, or 0 if no id was found
         */
        quint32 getMathmlId(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const;
        /**
         * @brief mathmlIdExisting checks if a mathml id for the current node is existing.
         * @param node the node to check for existence of a mathml id
         * @param state the state of the node given
         * @param previousNode the node that is the predecessor of the given node in the formula tree
         * @param nextNode the node that is the successor of the given node in the formula tree
         * @return true if a id is existing, false otherwise
         */
        bool mathmlIdExisting(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const;
        /**
         * @brief prevNodeWithId iterates the node iterator to the prevoius node that has a valid mathml id and sets the
         * internal node pointer accordingly
         */
        EgcNode& prevNodeWithId(void);
        /**
         * @brief nextNodeWithId iterates the node iterator to the next node that has a valid mathml id and sets the
         * internal node pointer accordingly
         */
        EgcNode& nextNodeWithId(void);


        QScopedPointer<EgcNodeIterator> m_nodeIterNext;     ///< the node iterator that points to the next node with a mathml id
        QScopedPointer<EgcNodeIterator> m_nodeIterPrev;     ///< the node iterator that points to the previous node with a mathml id
        EgcIteratorState m_histState;                       ///< last state of the node we jumped over
        const EgcMathmlLookup& m_lookup;                    ///< a reference to the lookup data
        quint32 m_histId;                                   ///< the mathml id of the last node we jumped over
};

#endif // EGCIDNODEITER_H
