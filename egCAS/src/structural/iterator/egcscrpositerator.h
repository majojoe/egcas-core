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
class EgcNodeIterator;

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
        virtual const quint32 next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous mathml id.
         */
        virtual const quint32 previous(void);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next mathml id.
         */
        virtual const quint32 peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous mathml id.
         */
        virtual const quint32 peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void);
        /**
         * @brief node returns the associated node we last jumped over
         * @return the node we last jumped over
         */
        virtual const EgcNode* node(void);
        /**
         * @brief rightSide true if the cursor is at the right side of the formula element, or on the left
         * @return true if the cursor is on the right side, false if it is on the left
         */
        virtual bool rightSide(void);
        /**
         * @brief lastId returns the last id we jumped over. The result is only valid if there is any mapping the
         * iterator can operate on.
         * @return the id we jumped over lastly.
         */
        virtual const quint32& id(void);
        /**
         * @brief subIndex returns the subindex of the element we last jumped over. Note: index is not position!
         * index = position-1. Therefore this is different to the m_subPos value of the rendering position data struct.
         * The index -1 means that there is no subindex or it is invalid (points to beginning or end of some subdata).
         * @return the subindex we last jumped over
         */
        virtual qint32& subIndex(void);

private:
        /**
         * @brief mathmlIdExisting checks if a mathml id for the current node is existing.
         * @param node the node to check for existence of a mathml id
         * @return true if a id is existing, false otherwise
         */
        bool mathmlIdExisting(EgcNode* node) const;
        /**
         * @brief prevNodeWithId iterates the node iterator to the prevoius node that has a valid mathml id and sets the
         * internal node pointer accordingly
         */
        void prevNodeWithId(void);
        /**
         * @brief nextNodeWithId iterates the node iterator to the next node that has a valid mathml id and sets the
         * internal node pointer accordingly
         */
        void nextNodeWithId(void);
        /**
         * @brief hasNextSubind checks if there is another e.g. char with a subindex in the current node
         * @return true if there is another subindex in the current node, false otherwise
         */
        virtual bool hasNextSubind(void) const;
        /**
         * @brief hasPreviousSubind checks if there is another e.g. char with a subindex in the current node
         * @return true if there is another subindex previous to the current position in the current node, false otherwise
         */
        virtual bool hasPreviousSubind(void) const;
        /**
         * @brief nextSubind step a subindex forward if there is another one
         */
        virtual void nextSubind(void);
        /**
         * @brief nextSubind step a subindex backward if there is a previous one
         */
        virtual void previousSubind(void);

        const quint32 m_pseudoRef = 0; ///< a pseudo reference to to point to, if there is no other reference available
        EgcNode* m_prevNode;    ///< points to the previous Node that is used for cursor data on the screen
        EgcNode* m_nextNode;    ///< points to the next Node that is used for cursor data on the screen
        qint32 m_prevSubind;    ///< points to the previous subindex
        qint32 m_nextSubind;    ///< points to the next subindex
        qint32 m_subindHist;    ///< points to the last subindex we jumped over
        const EgcMathmlLookup& m_lookup;                ///< a reference to the lookup data
        QScopedPointer<EgcNodeIterator> m_nodeIter;     ///< the node iterator to iterate over the formula nodes
};

#endif // EGCSCRPOSITERATOR_H
