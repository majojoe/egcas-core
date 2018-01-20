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

#include <QRectF>
#include <QLineF>
#include "egcformulaentity.h"
#include "formulamodificator.h"
#include "../visitor/formulascrvisitor.h"
#include "egcabstractformulaitem.h"

FormulaModificator::FormulaModificator(EgcFormulaEntity& formula) : m_formula{formula},
                                                                    m_iter(FormulaScrIter(m_formula, m_vector)),
                                                                    m_lastUnderlinedNode{nullptr}
{
        FormulaScrVisitor visitor = FormulaScrVisitor(m_formula, m_iter);
        visitor.updateVector();
}

FormulaModificator::~FormulaModificator()
{

}

void FormulaModificator::handleAction(const EgcAction& action)
{
        switch (action.m_op) {
        case EgcOperations::cursorForward:
                moveCursor(true);
                break;
        case EgcOperations::cursorBackward:
                moveCursor(false);
                break;
//        case EgcOperations::spacePressed:
//                markParent();
//                break;
        case EgcOperations::alnumKeyPressed:
                insertCharacter(action.m_character);
                break;
//        case EgcOperations::backspacePressed:
//                removeCharacter(true);
//                break;
//        case EgcOperations::delPressed:
//                removeCharacter(false);
//                break;
//        case EgcOperations::mathCharOperator:
//        case EgcOperations::mathFunction:
//        case EgcOperations::internalFunction:
//                insertOperation(action);
//                break;
//        case EgcOperations::homePressed:
//                if (m_scrIter) {
//                        m_scrIter->toFront();
//                        m_scrIter->resetUnderline();
//                        showCurrentCursor();
//                }
//                break;
//        case EgcOperations::endPressed:
//                if (m_scrIter) {
//                        m_scrIter->toBack();
//                        m_scrIter->resetUnderline();
//                        showCurrentCursor();
//                }
//                break;
//        case EgcOperations::createSubscript:
//                createSubId();
//                break;
        }
}

bool FormulaModificator::cursorAtBegin()
{
        if (!m_iter.hasPrevious())
                return true;

        return false;
}

bool FormulaModificator::cursorAtEnd()
{
        if (!m_iter.hasNext())
                return true;

        return false;
}

void FormulaModificator::insertCharacter(QChar character)
{
        FormulaScrElement el;
        el.m_value = character;

        m_iter.insert(el);
        reStructureTree();
}

void FormulaModificator::moveCursor(bool forward)
{
        if (!m_formula.getItem())
                return;
        if (forward) {
                if (m_iter.hasNext())
                        (void) m_iter.next();
                else
                        resetUnderline();
        } else {
                if (m_iter.hasPrevious())
                        (void) m_iter.previous();
                else
                        resetUnderline();
        }

        showCurrentCursor();
        if (isUnderlineActive())
                m_formula.getItem()->showUnderline(id());
}

void FormulaModificator::resetUnderline(void)
{
        m_lastUnderlinedNode = nullptr;
}

bool FormulaModificator::isUnderlineActive(void)
{
        if (m_lastUnderlinedNode)
                return true;
        else
                return false;
}

quint32 FormulaModificator::id(void) const
{
        const EgcMathmlLookup lookup = m_formula.getMathmlMappingCRef();
        return lookup.getIdFrame(nodeAtCursor());
}

void FormulaModificator::showCurrentCursor(void)
{
        bool rSide;
        EgcAbstractFormulaItem* item = m_formula.getItem();

        if (!item)
                return;

        rSide = rightSide();

        if (rSide && !m_iter.hasNext())
                return;
        if (!rSide && !m_iter.hasPrevious())
                return;

        TempDataScrIter tmp;
        if (rSide) {
                FormulaScrElement& el = m_iter.peekNext();
                tmp = el.lTemp;
        } else {
                FormulaScrElement& el = m_iter.peekPrevious();
                tmp = el.rTemp;
        }

        if (tmp.m_id == 0)
                return;

        QRectF rect = item->getElementRect(tmp.m_id, tmp.m_subpos);
        if (rect.isEmpty())
                return;

        QLineF cursor;
        if (tmp.m_left_side)
                cursor = QLineF(rect.topLeft(), rect.bottomLeft());
        else
                cursor = QLineF(rect.topRight(), rect.bottomRight());

        item->hideCursors();

        item->showCursor(cursor);
}

bool FormulaModificator::rightSide(void) const
{
        bool retval = true;

        if (!m_iter.hasPrevious()) { //is at front
                retval = true;
        } else {
                if (m_iter.hasNext()) {
                        FormulaScrElement& lel = m_iter.peekPrevious();
                        FormulaScrElement& rel = m_iter.peekNext();
                        if ((int)lel.m_cAdh < (int)rel.m_cAdh)
                                retval = true;
                        else
                                retval = false;
                } else {
                        retval = false;
                }
        }

        return retval;
}

EgcNode& FormulaModificator::nodeAtCursor(void) const
{
        EgcNode* node = nullptr;


        if (rightSide() && m_iter.hasNext())
                node = m_iter.peekNext().lTemp.m_node;
        else if (m_iter.hasPrevious())
                node = m_iter.peekPrevious().rTemp.m_node;

        if (!node)
                node = &m_formula.getBaseElement();


        return *node;
}

quint32 FormulaModificator::subPosition(void) const
{
        qint32 retval;

        if (rightSide())
                retval = m_iter.peekNext().lTemp.m_subpos;
        else
                retval = m_iter.peekPrevious().rTemp.m_subpos;

        return retval;
}

void FormulaModificator::reStructureTree()
{


        RestructParserProvider pp;
        ReStructureVisitor restructureVisitor(*this);
        QString result = restructureVisitor.getResult();
        NodeIterReStructData iterData;
        int errCode;
        EgcNode* tree = pp.getRestructParser()->restructureFormula(result, iterData, &errCode);
        if (tree) {
                setRootElement(tree);
                m_scrIter->invalidateCursor(getBaseElement());
                m_scrIter->updateRestructureData(iterData);
        }

}

