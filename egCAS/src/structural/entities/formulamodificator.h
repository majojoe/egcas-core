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
         * @brief handleAction handles the given action
         * @param action the action to execute
         */
        void handleAction(const EgcAction& action);
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
         * @brief moveCursor move cursor forward or backward
         * @param forward if true, cursor will be moved forward, backward otherwise
         */
        void moveCursor(bool forward);
        /**
         * @brief showCurrentCursor show the cursor of the currently active formula at current position
         */
        void showCurrentCursor(void);
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
         */
        void reStructureTree(void);


        EgcFormulaEntity& m_formula;            ///< reference to the formula this modificator is associated with
        FormulaScrVector m_vector;              ///< vector that contains all screen data to be able to iterate over the formula
        FormulaScrIter m_iter;                  ///< the iterator associated with this modificator
        EgcNode* m_lastUnderlinedNode;          ///< the last visible parent node that was given back
};

#endif // FORMULAMODIFICATOR_H
