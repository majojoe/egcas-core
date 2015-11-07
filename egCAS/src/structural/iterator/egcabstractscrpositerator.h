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

#ifndef EGCABSTRACTSCRPOSITERATOR_H
#define EGCABSTRACTSCRPOSITERATOR_H

#include <QtGlobal>
#include <QScopedPointer>
#include <QVector>
#include <QRectF>
#include <visitor/egcmathmllookup.h>

class EgcNode;

class EgcAbstractScrPosIterator
{
public:        
        /// constructor for initialization with formula
        EgcAbstractScrPosIterator(EgcMathmlLookup& data);
        /// std destructor
        virtual ~EgcAbstractScrPosIterator();
        /**
         * @brief hasNext Checks if there is at most one more item after the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasNext(void) const = 0;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasPrevious(void) const = 0;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next mathml id.
         */
        virtual const quint32 & next(void) = 0;
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous mathml id.
         */
        virtual const quint32 & previous(void) = 0;
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next mathml id.
         */
        virtual const quint32 & peekNext(void) const = 0;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous mathml id.
         */
        virtual const quint32 & peekPrevious(void) const = 0;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void) = 0;
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void) = 0;
        /**
         * @brief node returns the associated with node we last jumped over
         * @return the node we last jumped over
         */
        virtual const EgcNode* node(void) = 0;
        /**
         * @brief rightSide true if the cursor is at the right side of the formula element, or on the left
         * @return true if the cursor is on the right side, false if it is on the left
         */
        virtual bool rightSide(void) = 0;
        /**
         * @brief subIndex returns the subindex of the element we last jumped over
         * @return the subindex we last jumped over
         */
        virtual quint32& subIndex(void) = 0;
        /**
         * @brief hasNextSubind checks if there is another e.g. char with a subindex in the current node
         * @return true if there is another subindex in the current node, false otherwise
         */
        virtual bool hasNextSubind(void) const = 0;
        /**
         * @brief hasPreviousSubind checks if there is another e.g. char with a subindex in the current node
         * @return true if there is another subindex previous to the current position in the current node, false otherwise
         */
        virtual bool hasPreviousSubind(void) const = 0;
        /**
         * @brief nextSubind step a subindex forward if there is another one
         */
        virtual void nextSubind(void) = 0;
        /**
         * @brief nextSubind step a subindex backward if there is a previous one
         */
        virtual void previousSubind(void) = 0;
};

#endif // EGCABSTRACTSCRPOSITERATOR_H
