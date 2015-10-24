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

#ifndef EGCABSTRACTFORMULAITEM_H
#define EGCABSTRACTFORMULAITEM_H

#include <QHash>
#include "egcasiteminterface.h"

class EgcNode;
class EgcScreenPos;

class EgcAbstractFormulaItem : public EgcasItemInterface
{
public:
        /**
         * @brief ~EgcAbstractFormulaItem virtual destructor in order to be able to delete subclasses
         */
        virtual ~EgcAbstractFormulaItem() {}
        /**
         * @brief set the generic font size for all formulas (changes the overall font size of all formulas in a document).
         * If the font size of a specific formula should be changed, use the function setFontSize.
         * @param size the font size in points
         */
        virtual void setGenericFontSize(int size) = 0;
        /**
         * @brief getBaseFontSize returns the base font size of all formulas in a document
         * @return the base font size of all formulas
         */
        virtual int getGenericFontSize(void) = 0;
        /**
         * @brief set the font size for a formula (changes only the font size of this formula).
         * If the overall font size of all formulas should be changed, use the function setBaseFontSize.
         * @param size the font size in points
         */
        virtual void setFontSize(int size) = 0;
        /**
         * @brief getFontSize returns the font size of the current formula
         * @return the font size of the current formula
         */
        virtual int getFontSize(void) = 0;
        /**
         * @brief updateView update the view with the new mathml representation if anything changes
         */
        virtual void updateView(void) = 0;
};

#endif // EGCABSTRACTFORMULAITEM_H
