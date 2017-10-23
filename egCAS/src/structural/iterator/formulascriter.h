/*
Copyright (c) 2017, Johannes Maier <maier_jo@gmx.de>
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

#ifndef FORMULASCRITER_H
#define FORMULASCRITER_H

#include "../visitor/formulascrelement.h"

class EgcFormulaEntity;
class EgcNode;

class FormulaScrIter
{
public:
        FormulaScrIter();

        /// constructor for initialization with formula
        FormulaScrIter(const EgcFormulaEntity& formula);
        /// std destructor
        virtual ~FormulaScrIter();
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
         * @brief findPrevious find the next occurence of the given formula element
         * @param value the value to search for
         * @return true if found
         */
        bool findNext(const FormulaScrElement &value);
        /**
         * @brief findPrevious find the previous occurence of the given formula element
         * @param value the value to search for
         * @return true if found
         */
        bool findPrevious(const FormulaScrElement &value);
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next item.
         */
        virtual FormulaScrElement & next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous item.
         */
        virtual FormulaScrElement & previous(void);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next item.
         */
        FormulaScrElement & peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous item.
         */
        FormulaScrElement & peekPrevious(void) const;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void);
        /**
         * @brief insert inserts the given item at the current position.
         */
        virtual void insert(FormulaScrElement type);
        /**
         * @brief remove removes the next or previous item.
         */
        virtual void remove();
};

#endif // FORMULASCRITER_H
