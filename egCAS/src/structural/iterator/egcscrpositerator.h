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
#include "view/egcscreenpos.h"

class EgcNode;
class EgcFormulaEntity;
class EgRenderingPosition;

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
         * @return a reference to the next item.
         */
        virtual EgcNode & next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous item.
         */
        virtual EgcNode & previous(void);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next item.
         */
        EgcNode & peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous item.
         */
        EgcNode & peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void);
        /**
         * @brief subindex returns the subindex we jumped over last time
         * @return the subindex
         */
        virtual quint32 subindex(void);
        /**
         * @brief node returns the node we jumped over last time
         * @return the node we jumped over using a traversal function
         */
        virtual const EgcNode& node();
private:
        const EgcFormulaEntity& m_formula;                              ///< reference to formula we use for iteration
        const EgcScreenPos& m_pos;                                      ///< reference to position class
        QScopedPointer<QVectorIterator<EgRenderingPosition>> m_i;       ///< iterator for screen positions
        const EgRenderingPosition* m_history;                           ///< pointer to the position we jumped over

};

#endif // EGCSCRPOSITERATOR_H
