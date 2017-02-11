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

#ifndef EGCABSTRACTFORMULAENTITY_H
#define EGCABSTRACTFORMULAENTITY_H

#include <QVector>
#include "egcabstractentity.h"

class QString;
class EgcAbstractFormulaItem;
class EgcAction;

class EgcAbstractFormulaEntity : public EgcAbstractEntity
{
public:
        virtual ~EgcAbstractFormulaEntity() {}
        /**
         * @brief getMathMlCode returns the mathMl representation for this formula
         * @return the mathMl representation of this formula as a string
         */
        virtual QString getMathMlCode(void) = 0;
        /**
         * @brief setItem set the formula item that is associated with this entity
         * @param item the item to set (can also be a nullptr)
         */
        virtual void setItem(EgcAbstractFormulaItem* item) = 0;
        /**
         * @brief handleAction handles the given action (e.g. insert a char at the given position into the formula tree)
         * @param action the action given
         */
        virtual void handleAction(const EgcAction& action) = 0;
        /**
         * @brief cursorAtBegin checks if cursor is at beginning of formula
         * @return true if cursor is at beginning
         */
        virtual bool cursorAtBegin(void) = 0;
        /**
         * @brief cursorAtEnd checks if cursor is at end of formula
         * @return true if cursor is at end
         */
        virtual bool cursorAtEnd(void) = 0;
        /**
         * @brief isEmpty check if the formula is an empty formula
         * @return true if empty, false otherwise
         */
        virtual bool isEmpty(void) const = 0;
        /**
         * @brief setCursorPos set the cursor position at the given nodeId and sub position location
         * @param nodeId the node Id where to set the cursor
         * @param subPos the sub position where to set the cursor
         * @param rightSide set it at the right side or at the left of the given instance
         */
        virtual void setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide) = 0;

};

#endif // EGCABSTRACTFORMULAENTITY_H
