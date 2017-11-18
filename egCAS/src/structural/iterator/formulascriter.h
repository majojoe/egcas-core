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
        /// constructor for initialization with formula
        FormulaScrIter(EgcFormulaEntity& formula, FormulaScrVector& vector);
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
         * @brief findNext find the next occurence of the given QString m_value element in the formula
         * @param value the value to search for
         * @return true if found
         */
        bool findNext(const QString& value);
        /**
         * @brief findPrevious find the previous occurence of the given  QString m_value element in the formula
         * @param value the value to search for
         * @return true if found
         */
        bool findPrevious(const QString &value);
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
        virtual void insert(FormulaScrElement element);
        /**
         * @brief remove removes the next or previous item.
         */
        virtual void remove();
        /**
         * @brief update the formula AST has changed, so update the internal references
         */
        void update(void);
        /**
         * @brief nodeAtCursor returns the node at the current cursor position
         * @return the node
         */
        EgcNode& nodeAtCursor(void);
        /**
         * @brief clear all elements from the underlying container and set the iterator appropriately
         */
        void clear(void);

private:
        /**
         * @brief setIterPos set the iterator position. ATTENTION: use only for internal purposes (DO NOT MAKE PUBLIC)
         * @param pos the position where to set the java style iterator. 0 is the same as toFront() and size() is the
         * same as toBack(). Use with care since this operation has complexity is O(n) where n is the vector size. If
         * given position is larger than size() of the vector, position will be set to toBack()
         */
        void setIterPos(quint32 pos);
        /**
         * @brief getIterPos returns the iterator position. ATTENTION: use only for internal purposes (DO NOT MAKE PUBLIC)
         * @return the position where the java style iterator is. 0 is the same as toFront() position and size() is the
         * same as toBack() position. Use with care since this operation has complexity is O(n) where n is the vector
         * size.
         */
        quint32 getIterPos(void);

        FormulaScrVector& m_vector;             ///< vector that contains the all the formula elements
        EgcFormulaEntity& m_formula;            ///< reference associated with the given formula
        QMutableVectorIterator<FormulaScrElement> m_iter;  ///< iterator for the formula vector above
        quint32 m_pos;                          ///< it's the iterator position as an index. Be careful since this can break very easiliy since we are using a Java Style iterator also!!!
};

#endif // FORMULASCRITER_H
