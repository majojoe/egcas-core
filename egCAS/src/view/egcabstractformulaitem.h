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
         * @brief updateView update the view with the new mathml representation if anything changes
         */
        virtual void updateView(void) = 0;
        /**
         * @brief paintUnderline paint the underline that marks any mathml node adressed by the mathml id, to be able
         * to show the user the context of his operation (e.g. keystroke).
         * @param mathmlId the mathml id for which to paint a underline
         */
        virtual void showUnderline(quint32 mathmlId) = 0;
        /**
         * @brief showLeftCursor shows the cursor on the left side of a mathml element
         * @param mathmlId the mathml id to use for identifying the mathml element
         * @param subindex the subindex to use for identifying the mathml element (e.g. one character of a text)
         */
        virtual void showLeftCursor(quint32 mathmlId, quint32 subindex) = 0;
        /**
        * @brief showRightCursor shows the cursor on the right side of a mathml element
        * @param mathmlId the mathml id to use for identifying the mathml element
        * @param subindex the subindex to use for identifying the mathml element (e.g. one character of a text)
        */
        virtual void showRightCursor(quint32 mathmlId, quint32 subindex) = 0;
        /**
         * @brief hideCursors hide the cursors e.g. in case the element is moved
         */
        virtual void hideCursors(void) = 0;
        /**
         * @brief selectFormula select the formula and grab focus
         * @param selected if true formula gets selected, if false deselected
         */
        virtual void selectFormula(bool selected) = 0;
};

#endif // EGCABSTRACTFORMULAITEM_H
