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
#include <QtMultimedia/QSound>
#include "egcformulaentity.h"
#include "formulamodificator.h"
#include "../visitor/formulascrvisitor.h"
#include "egcabstractformulaitem.h"
#include "casKernel/parser/restructparserprovider.h"
#include "casKernel/parser/abstractkernelparser.h"
#include "../concreteNodes/egcalnumnode.h"

/**
 * @brief The NodeIterReStructData class is intended for use during restructering a formula -> state does not change but
 * node pointers
 */
class NodeIterReStructData {
public:
        NodeIterReStructData() : m_node{nullptr} {}
        EgcNode* m_node;
        EgcNode* m_iterPosAfterUpdate;
};


FormulaModificator::FormulaModificator(EgcFormulaEntity& formula) : m_formula{formula},
                                                                    m_iter(FormulaScrIter(m_formula, m_vector)),
                                                                    m_lastUnderlinedNode{nullptr}, m_changeAwaited{false}
{
        FormulaScrVisitor visitor = FormulaScrVisitor(m_formula, m_iter);
        visitor.updateVector();
}

FormulaModificator::~FormulaModificator()
{

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

void FormulaModificator::viewHasChanged()
{
        if (m_changeAwaited) {
                m_changeAwaited = false;
                m_iter.update();
                showCurrentCursor();
        }
}

void FormulaModificator::insertBinaryOperation(QString op)
{
        bool insertEmptyLeft = false;
        bool insertEmptyRight = false;
        FormulaScrElement el;
        el.m_value = op;

        if (!m_iter.hasPrevious()) {
                insertEmptyLeft = true;
        } else {
                FormulaScrElement &l_el = m_iter.peekPrevious();
                EgcNode* lNode = l_el.m_node;
                if (lNode) {
                        if (    lNode->isOperation()
                             && (    l_el.m_sideNode == FormulaScrElement::nodeLeftSide
                                  || l_el.m_sideNode == FormulaScrElement::nodeMiddle))
                                insertEmptyLeft = true;
                }
        }
        if (!m_iter.hasNext()) {
                insertEmptyRight = true;
        } else {
                FormulaScrElement &r_el = m_iter.peekNext();
                EgcNode* rNode = r_el.m_node;
                if (rNode) {
                        if (    rNode->isOperation()
                             && (    r_el.m_sideNode == FormulaScrElement::nodeRightSide
                                  || r_el.m_sideNode == FormulaScrElement::nodeMiddle))
                                insertEmptyRight = true;
                }
        }


        if (insertEmptyLeft)
                insertEmptyNode();
        m_iter.insert(el);
        if (insertEmptyRight)
                insertEmptyNode();

        updateFormula();
}

void FormulaModificator::insertCharacter(QChar character)
{
        FormulaScrElement el;
        el.m_value = EgcAlnumNode::encode(character);

        if (isEmptyElement(true)) {
                m_iter.remove(true);
        }
        if (isEmptyElement(false)) {
                m_iter.remove(false);
        }

        m_iter.insert(el);
        updateFormula();
}

void FormulaModificator::removeElement(bool previous)
{
        if (isEmpty() || m_vector.size() == 1) {
                if (m_formula.getDocument()) {
                        m_formula.getDocument()->deleteEntity(&m_formula);
                }
        } else {
                m_iter.remove(previous);
                if (m_vector.isEmpty())
                        insertEmptyNode();
        }

        updateFormula();
}

void FormulaModificator::insertOperation(EgcAction operation)
{
        if (operation.m_op == EgcOperations::mathCharOperator) {
                if (    operation.m_character == '+'
                     || operation.m_character == '-'
                     || operation.m_character == '/'
                     || operation.m_character == '*'
                     || operation.m_character == ':'
                     || operation.m_character == '=')
                        insertBinaryOperation(operation.m_character);
        }


//                if (    operations.m_character == '('
//                                || operations.m_character == ')') {
//                        if (    operations.m_character == '(')
//                                return createAndInsertOp(EgcNodeType::LParenthesisNode);
//                        if (    operations.m_character == ')')
//                                return createAndInsertOp(EgcNodeType::RParenthesisNode);
//                }
//                if (operations.m_character == QChar(177))
//                        return createAndInsertOp(EgcNodeType::UnaryMinusNode);
//                if (operations.m_character == QChar(8730))
//                        return createAndInsertOp(EgcNodeType::RootNode);
//                if (operations.m_character == QChar('^'))
//                        return createAndInsertOp(EgcNodeType::ExponentNode);
//                if (operations.m_character == QChar(','))
//                        return insertFunctionContainer();
//        } else if (operations.m_op == EgcOperations::mathFunction) { // functions
//                EgcFunctionNode* fnc = dynamic_cast<EgcFunctionNode*>(createAndInsertOperation(EgcNodeType::FunctionNode));
//                if (!fnc)
//                        return false;
//                if (!operations.m_additionalData.isNull()) {
//                        QString name = operations.m_additionalData.toString();
//                        if (!name.isEmpty()) {
//                                fnc->setName(name);
//                                m_scrIter->setCursorAtDelayed(fnc->getChild(0), true);
//                        } else {
//                                m_scrIter->setCursorAtDelayed(fnc, false);
//                        }
//                }
//        } else if (operations.m_op == EgcOperations::internalFunction) { // internal functions
//                if (operations.m_intType == InternalFunctionType::natLogarithm
//                     || operations.m_intType == InternalFunctionType::logarithm) {
//                        if (operations.m_intType == InternalFunctionType::natLogarithm)
//                                retval = createAndInsertOp(EgcNodeType::NatLogNode);
//                        else if (operations.m_intType == InternalFunctionType::logarithm)
//                                retval = createAndInsertOp(EgcNodeType::LogNode);

//                        if (retval) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        m_scrIter->setCursorAtDelayed(const_cast<EgcNode*>(nd), true);
//                                }
//                        }
//                } else if (operations.m_intType == InternalFunctionType::integral) {
//                        bool ret = createAndInsertOp(EgcNodeType::IntegralNode);
//                        if (ret) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        EgcContainerNode *par = nullptr;
//                                        par = nd->getParent();
//                                        if (par->getNodeType() == EgcNodeType::IntegralNode) {
//                                                static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                if (operations.m_OpModificators == OpModificators::definiteIntegral) {
//                                                        static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                        static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                }
//                                        }
//                                }
//                        }
//                        return ret;
//                } else if (operations.m_intType == InternalFunctionType::differential) {
//                        bool ret = createAndInsertOp(EgcNodeType::DifferentialNode);
//                        if (ret) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        EgcContainerNode *par = nullptr;
//                                        par = nd->getParent();
//                                        if (par->getNodeType() == EgcNodeType::DifferentialNode) {
//                                                EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(par);
//                                                static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_1)
//                                                        diff->setNrDerivative(1);
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_2)
//                                                        diff->setNrDerivative(2);
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_3)
//                                                        diff->setNrDerivative(3);
//                                        }
//                                }
//                        }
//                        return ret;
//                }

//        }
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

bool FormulaModificator::reStructureTree()
{
        bool retval = true;

        RestructParserProvider pp;
        FormulaScrVisitor restructVisitor(m_formula, m_iter);
        QString result = restructVisitor.getResult();
        int errCode;
        NodeIterReStructData iterData;
        EgcNode* tree = pp.getRestructParser()->restructureFormula(result, iterData, &errCode);
        if (tree) {
                m_formula.setRootElement(tree);
                m_formula.updateView();
        } else {
                retval = false;
        }

        return retval;
}

bool FormulaModificator::updateFormula(void)
{
        bool retval = true;

        if (!reStructureTree()) {
                retval = false;
                m_iter.revert();
                QSound::play(":/res/sound/error.wav");
        } else {
                m_changeAwaited = true;
                m_formula.updateView();
        }

        return retval;
}

void FormulaModificator::insertEmptyNode(void)
{
        FormulaScrElement el;
        el.m_value = "_empty";
        m_iter.insert(el);
}

bool FormulaModificator::isEmpty() const
{
        if (m_vector.size() == 1 && m_vector.at(0).m_value == QString("_empty"))
                return true;

        return false;
}

bool FormulaModificator::isEmptyElement(bool previous) const
{
        bool retval = false;

        if (previous) {
                if (m_iter.hasPrevious()) {
                        if (m_iter.peekPrevious().m_value == QString("_empty"))
                                retval = true;
                }
        } else {
                if (m_iter.hasNext()) {
                        if (m_iter.peekNext().m_value == QString("_empty"))
                                retval = true;
                }
        }

        return retval;
}

