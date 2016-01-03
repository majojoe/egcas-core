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

#ifndef EGCSCRPOSITERATOR_H
#define EGCSCRPOSITERATOR_H

#include <QtGlobal>
#include <QScopedPointer>
#include <QVector>
#include <QRectF>

class EgcNode;
class EgcFormulaEntity;
class EgcMathmlLookup;
class EgcIdNodeIter;
class EgcSubindNodeIter;

// /**
// * @brief The EgcScrPosVisibility enum decribes the visibility of a formula element. A formula element can have visible
// * elements on the left and right side (e.g. parenthesis) or between two nodes.
// * The enums can be ORed.
// */
//enum class EgcScrPosPosition {NotVisible = 0, Left = 1, Right = 2, LeftAndRight = 3, BetweenNodes = 4};

/**
 * @brief The EgcScrPosIterator class is a iterator for navigating between the formula glyphs
 */
class EgcScrPosIterator
{
public:        
        /// constructor for initialization with formula
        EgcScrPosIterator(const EgcFormulaEntity& formula);
        /// std destructor
        virtual ~EgcScrPosIterator();
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
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next mathml id.
         */
        const quint32 next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous mathml id.
         */
        const quint32 previous(void);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next mathml id.
         */
        const quint32 peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous mathml id.
         */
        const quint32 peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        void toFront(void);
        /**
         * @brief node returns the associated node we last jumped over
         * @return the node we last jumped over
         */
        const EgcNode* node(void);
        /**
         * @brief rightSide true if the cursor is at the right side of the formula element, or on the left
         * @return true if the cursor is on the right side, false if it is on the left
         */
        bool rightSide(void);
        /**
         * @brief subIndex returns the subindex of the element we last jumped over. Note: index is not position!
         * index = position-1. Therefore this is different to the m_subPos value of the rendering position data struct.
         * The index -1 means that there is no subindex or it is invalid (points to beginning or end of some subdata).
         * @return the subindex we last jumped over
         */
        int subIndex(void);
        /**
         * @brief lastId returns the last id we jumped over. The result is only valid if there is any mapping the
         * iterator can operate on.
         * @return the id we jumped over lastly.
         */
        quint32 id(void);

private:
        const EgcMathmlLookup& m_lookup;                ///< a reference to the lookup data
        QScopedPointer<EgcIdNodeIter> m_nodeIter;       ///< the node iterator to iterate over the formula nodes
        EgcNode* m_node;                                ///< the last node we jumped over
        quint32 m_id;                                   ///< the last mathml id we jumped over
        QScopedPointer<EgcSubindNodeIter> m_subIdIter;  ///< iterator for the subindexes of a node
};

#endif // EGCSCRPOSITERATOR_H
