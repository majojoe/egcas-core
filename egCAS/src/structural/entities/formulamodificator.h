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
#ifndef FORMULAMODIFICATOR_H
#define FORMULAMODIFICATOR_H

#include "../visitor/formulascrelement.h"
#include "../iterator/formulascriter.h"
#include <structural/actions/egcaction.h>

class EgcFormulaEntity;

class FormulaModificator
{
public:
        ///std constructor
        FormulaModificator(EgcFormulaEntity& formula);
        virtual ~FormulaModificator();
        /**
         * @brief cursorAtBegin checks if the cursor is at the beginning of a formula
         * @return true if the cursor is at the beginning of the formula, false otherwise
         */
        bool cursorAtBegin(void);
        /**
         * @brief cursorAtEnd checks if the cursor is at the end of the formula
         * @return true if the cursor is at the end of the formula, false otherwise
         */
        bool cursorAtEnd(void);
        /**
         * @brief insertCharacter insert a character at the current cursor position
         * @param character the character to insert
         */
        void insertCharacter(QChar character);
        /**
         * @brief showCurrentCursor show the cursor of the currently active formula at current position
         */
        void showCurrentCursor(void);
        /**
         * @brief viewHasChanged is to be called when view has changed to complete insert or remove operation
         */
        void viewHasChanged(void);
        /**
         * @brief insertOperation insert a operation into the tree
         * @param operation the operation action to insert
         */
        void insertOperation(EgcAction operation);
        /**
         * @brief moveCursor move cursor forward or backward
         * @param forward if true, cursor will be moved forward, backward otherwise
         */
        void moveCursor(bool forward);
        /**
         * @brief insertBinaryOperation insert a binary operation
         * @param op the binary operation to insert as ASCII string that can be interpreted by the parser
         */
        void insertBinaryOperation(QString op);
        /**
         * @brief removeElement remove a character or operation at the current cursor position
         * @param previous if true the character before the current cursor position is remove, if false the character behind
         */
        void removeElement(bool previous);
        /**
         * @brief insertSegment inserts a complete segment of elements. This does not need to be done in a per elements
         * way, since the parser will split that up again correctly.
         * @param segment the segment to be insert (can consist of many elements)
         */
        void insertUnaryOperation(QString segment);
        /**
         * @brief createSubscript create a subscipt for a formula variable at the current cursor position
         */
        void createSubscript(void);
private:
        /**
         * @brief resetUnderline reset the underline
         */
        void resetUnderline(void);
        /**
         * @brief isUnderlineActive checks if any underline is active
         * @return true if underline is active, false otherwise
         */
        bool isUnderlineActive(void);
        /**
         * @brief id the mathml id of the currently active node
         * @return the mathml id
         */
        quint32 id(void) const;
        /**
         * @brief rightSide checks if node for representing the cursor is on the right side of the current cursor or
         * node or on the left side (Cursor is a Java Style cursor and has left and a right node).
         * @return true if on the right side, false if on the left
         */
        bool rightSide(void) const;
        /**
         * @brief nodeAtCursor returns the node at the current cursor
         * @return the node at the current cursor
         */
        EgcNode& nodeAtCursor(void) const;
        /**
         * @brief subPosition returns the subPosition of the current mathml id
         * @return the subPosition of the current mathml id
         */
        quint32 subPosition(void) const;
        /**
         * @brief reStructureTree restructure the tree e.g. after an insert or delete
         * @return true if restructing the tree was successful (insert or delete was successful)
         */
        bool reStructureTree(void);
        /**
         * @brief updateFormula updates everything needed (view and structure) to reflect the changes
         * @return true if the change was successful, false if not
         */
        bool updateFormula(void);
        /**
         * @brief insertEmptyNode insert an empty node at the current cursor position
         */
        void insertEmptyNode(void);
        /**
         * @brief isEmpty checks if formula is currently empty (contains just an empty element)
         * @return true if formula is empty, false if not
         */
        bool isEmpty(void) const;
        /**
         * @brief isEmptyElement checks if the previous or next element is an empty element
         * @param previous if true the previous element will be checked, if false the next element will be checked. If
         * there is no element at all, the result is false.
         * @return true if the checked element is an empty element
         */
        bool isEmptyElement(bool previous) const;


        EgcFormulaEntity& m_formula;            ///< reference to the formula this modificator is associated with
        FormulaScrVector m_vector;              ///< vector that contains all screen data to be able to iterate over the formula
        FormulaScrIter m_iter;                  ///< the iterator associated with this modificator
        EgcNode* m_lastUnderlinedNode;          ///< the last visible parent node that was given back
        bool m_changeAwaited;                     ///< is set if a change in the view is awaited. If this has happened we can complete the change operation
};

#endif // FORMULAMODIFICATOR_H
