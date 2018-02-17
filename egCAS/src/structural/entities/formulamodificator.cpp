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
#include <QStringBuilder>
#include "egcformulaentity.h"
#include "formulamodificator.h"
#include "../visitor/formulascrvisitor.h"
#include "egcabstractformulaitem.h"
#include "casKernel/parser/restructparserprovider.h"
#include "casKernel/parser/abstractkernelparser.h"
#include "../concreteNodes/egcalnumnode.h"

const char emptyElement[] = "_empty";
const char emptyBinElement[] = "_emptybinop";

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

        // check if there is an empty binary operation at the left or at the right
        if (m_iter.hasPrevious()) {
                EgcNode* l = m_iter.peekPrevious().m_node;
                if (l) {
                        if (l->getNodeType() == EgcNodeType::BinEmptyNode)
                                m_iter.remove(true);
                }
        }
        if (m_iter.hasNext()) {
                EgcNode* r = m_iter.peekNext().m_node;
                if (r) {
                        if (r->getNodeType() == EgcNodeType::BinEmptyNode)
                                m_iter.remove(false);
                }
        }

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

        // since a minus can also be a unary minus -> delete the empty node here
        if (op == QString("-")) {
                if (insertEmptyLeft)
                        insertEmptyLeft = false;
                else if (isEmptyElement(true))
                        m_iter.remove(true);
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

        m_iter.save();

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
        FormulaScrElement element;
        m_iter.save();

        if (isEmpty() || m_vector.size() == 1) {
                if (m_formula.getDocument()) {
                        m_formula.getDocument()->deleteEntity(&m_formula);
                }
                return;
        } else {
                bool segmented = false;
                if (previous && m_iter.hasPrevious()) {
                        FormulaScrElement& el = m_iter.peekPrevious();
                        element = el;
                        if (el.m_isSegmented)
                                segmented = true;
                }
                if (!previous && m_iter.hasNext()) {
                        FormulaScrElement& el = m_iter.peekNext();
                        element = el;
                        if (el.m_isSegmented)
                                segmented = true;
                }

                if (segmented)
                        rmSegmented(previous);
                else
                        m_iter.remove(previous);
                if (m_vector.isEmpty())
                        insertEmptyNode();
        }

        //sanitize formulas after removing elements
        sanitizeBinary();
        sanitizeUnary();
        sanitizeMisc();
        sanitizeWithEmptyBinaryOps();
        sanitizeFlex();
        sanitizeSpecials(element, previous);

        updateFormula();
}

void FormulaModificator::insertRedParenthesis(bool left)
{
        if (left) {
                insertUnaryElement("_red_parenth_l");
                if (!m_iter.hasNext())
                        insertEmptyNode();
        } else {
                if (!m_iter.hasPrevious())
                        insertEmptyNode();
                insertUnaryElement("_red_parenth_r", false);
        }

        m_iter.peekPrevious().m_isPositionMarker = true;
        bool pendantFound = false;

        if (!left) {
                (void) m_iter.previous();
                while(m_iter.hasPrevious()) {
                        FormulaScrElement& i = m_iter.previous();
                        if (!i.m_node)
                                continue;
                        EgcNodeType type = i.m_node->getNodeType();
                        if (type == EgcNodeType::LParenthesisNode) {
                                i.m_node = nullptr;
                                i.m_value = QString("(");
                                pendantFound = true;
                                break;
                        }
                }
        } else {
                while(m_iter.hasNext()) {
                        FormulaScrElement& i = m_iter.next();
                        if (!i.m_node)
                                continue;
                        EgcNodeType type = i.m_node->getNodeType();
                        if (type == EgcNodeType::RParenthesisNode) {
                                i.m_node = nullptr;
                                i.m_value = QString(")");
                                pendantFound = true;
                                break;
                        }
                }
        }

        m_iter.toFront();
        while(m_iter.hasNext()) {
                FormulaScrElement& i = m_iter.next();
                if (i.m_isPositionMarker) {
                        i.m_isPositionMarker = false;
                        if (pendantFound) {
                                if (!left)
                                        i.m_value = QString(")");
                                else
                                        i.m_value = QString("(");
                        }
                        break;
                }
        }

        updateFormula();
}

void FormulaModificator::insertUnaryElement(QString segment, bool left)
{
        FormulaScrElement el;
        el.m_value = segment;

        if (left) {
                if (isEmptyElement(true))
                        m_iter.remove(true);
        }
        if (!left) {
                if (isEmptyElement(false))
                        m_iter.remove(false);
        }

        m_iter.insert(el);
}

void FormulaModificator::insertUnaryOperation(QString segment)
{
        insertUnaryElement(segment);
        updateFormula();
}

void FormulaModificator::createSubscript()
{
        FormulaScrElement el;
        el.m_value = QString("_1");

        m_iter.insert(el);
        insertEmptyNode();
        updateFormula();
}

void FormulaModificator::toFront()
{
        resetUnderline();
        m_iter.toFront();
        showCurrentCursor();
}

void FormulaModificator::toBack()
{
        resetUnderline();
        m_iter.toBack();
        showCurrentCursor();
}

void FormulaModificator::insertOperation(EgcAction operation)
{
        // save iterator state
        m_iter.save();

        if (operation.m_op == EgcOperations::mathCharOperator) {
                if (    operation.m_character == '+'
                     || operation.m_character == '-'
                     || operation.m_character == '/'
                     || operation.m_character == '*'
                     || operation.m_character == ':'
                     || operation.m_character == '='
                     || operation.m_character == '^'
                     || operation.m_character == ','
                )
                        insertBinaryOperation(operation.m_character);

                if (operation.m_character == QChar(177))
                        insertUnaryOperation("-");
                if (operation.m_character == QChar(8730)) {
                        QString tmp = QString("_root(") % emptyElement % QString(",") % emptyElement % QString(")");
                        insertUnaryOperation(tmp);
                }
        }

        if (operation.m_character == '(')
                insertRedParenthesis(true);
        if (operation.m_character == ')')
                insertRedParenthesis(false);

        if (operation.m_op == EgcOperations::mathFunction) { // functions
                QString name;
                bool nameGiven = false;
                if (!operation.m_additionalData.isNull())  {
                        name = operation.m_additionalData.toString();
                }
                if (name.isEmpty() && !isEmptyElement()) {
                        name = emptyElement;
                        insertEmptyNode();
                }
                // insertUnaryElement is wrong here, since that would remove the empty node for the name
                FormulaScrElement el;
                el.m_value = "(";
                m_iter.insert(el);
                insertEmptyNode();
                insertUnaryElement(")", false);
                (void) m_iter.previous();
                if (!nameGiven) {
                        (void) m_iter.previous();
                        (void) m_iter.previous();
                }
                updateFormula();
        }

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
        el.m_value = emptyElement;
        m_iter.insert(el);
}

void FormulaModificator::insertBinEmptyNode(void)
{
        FormulaScrElement el;
        el.m_value = emptyBinElement;
        m_iter.insert(el);
}

bool FormulaModificator::isEmpty() const
{
        if (m_vector.size() == 1 && m_vector.at(0).m_value == QString(emptyElement))
                return true;

        return false;
}

bool FormulaModificator::isEmptyElement(bool previous) const
{
        return isSpecificElement(emptyElement, previous);
}

bool FormulaModificator::isVarsubscriptSeparator(bool previous) const
{
        return isSpecificElement("_1", previous);
}

bool FormulaModificator::isSpecificElement(QString element, bool previous) const
{
        bool retval = false;

        if (previous) {
                if (m_iter.hasPrevious()) {
                        if (m_iter.peekPrevious().m_value == QString(element))
                                retval = true;
                }
        } else {
                if (m_iter.hasNext()) {
                        if (m_iter.peekNext().m_value == QString(element))
                                retval = true;
                }
        }

        return retval;
}

void FormulaModificator::sanitizeBinary()
{
        // insert empty node if left and right nodes are binary nodes
        if (m_iter.hasPrevious() && m_iter.hasNext()) {
                EgcNode *lnode = m_iter.peekPrevious().m_node;
                EgcNode *rnode = m_iter.peekNext().m_node;
                if (lnode && rnode) {
                        if (lnode->isBinaryNode() && rnode->isBinaryNode()) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }

        // insert empty node if left node is a binary node and right does not exist
        if (m_iter.hasPrevious() && !m_iter.hasNext()) {
                FormulaScrElement& el = m_iter.peekPrevious();
                EgcNode *lnode = el.m_node;
                if (lnode) {
                        if (lnode->isBinaryNode() && el.m_sideNode == FormulaScrElement::nodeMiddle) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }

        // insert empty node if right node is a binary node and left does not exist
        if (!m_iter.hasPrevious() && m_iter.hasNext()) {
                FormulaScrElement& el = m_iter.peekNext();
                EgcNode *rnode = el.m_node;
                if (rnode) {
                        if (rnode->isBinaryNode() && el.m_sideNode == FormulaScrElement::nodeMiddle) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }
}

void FormulaModificator::sanitizeUnary()
{
        // insert empty node if left and right nodes are binary nodes
        if (m_iter.hasPrevious() && m_iter.hasNext()) {
                EgcNode *lnode = m_iter.peekPrevious().m_node;
                EgcNode *rnode = m_iter.peekNext().m_node;
                if (lnode && rnode) {
                        if (lnode->isUnaryNode() && rnode->isUnaryNode() && lnode == rnode) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }

        // insert empty node if left node is a unary node and right does not exist
        if (m_iter.hasPrevious() && !m_iter.hasNext()) {
                FormulaScrElement& el = m_iter.peekPrevious();
                EgcNode *lnode = el.m_node;
                if (lnode) {
                        if (lnode->isUnaryNode() && el.m_sideNode == FormulaScrElement::nodeLeftSide) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }

        // insert empty node if right node is a unary node and left does not exist
        if (!m_iter.hasPrevious() && m_iter.hasNext()) {
                FormulaScrElement& el = m_iter.peekNext();
                EgcNode *rnode = el.m_node;
                if (rnode) {
                        if (rnode->isUnaryNode() && el.m_sideNode == FormulaScrElement::nodeRightSide) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }
}

void FormulaModificator::sanitizeFlex()
{
        // insert empty node if left and right nodes are binary nodes
        if (m_iter.hasPrevious() && m_iter.hasNext()) {
                EgcNode *lnode = m_iter.peekPrevious().m_node;
                EgcNode *rnode = m_iter.peekNext().m_node;
                enum FormulaScrElement::SideNode lside = m_iter.peekPrevious().m_sideNode;
                enum FormulaScrElement::SideNode rside = m_iter.peekNext().m_sideNode;
                if (lnode && rnode) {
                        if (    lnode->isFlexNode() && rnode->isFlexNode()
                             && lnode == rnode
                             && lside == FormulaScrElement::nodeLeftSide && rside == FormulaScrElement::nodeRightSide) {
                                insertEmptyNode();
                                m_iter.previous();
                        }
                }
        }
}

void FormulaModificator::sanitizeWithEmptyBinaryOps()
{

        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                EgcNode *l = m_iter.peekPrevious().m_node;
                EgcNode *r = m_iter.peekNext().m_node;
                if (!l || !r)
                        return;

                EgcNodeType rtype = r->getNodeType();
                EgcNodeType ltype = l->getNodeType();

                // number nodes followed by variable nodes -> not allowed by parser
                if (    ltype == EgcNodeType::NumberNode
                     && (rtype == EgcNodeType::AlnumNode || rtype == EgcNodeType::VariableNode))
                                insertBinEmptyNode();

                // two unary nodes that are not nested and need therefore a "binding node"
                if (    !l->isBinaryNode() && !r->isBinaryNode()
                     && m_iter.peekPrevious().m_sideNode == FormulaScrElement::nodeRightSide
                     && m_iter.peekNext().m_sideNode == FormulaScrElement::nodeLeftSide)
                        insertBinEmptyNode();
        }
}

void FormulaModificator::sanitizeMisc()
{
        EgcNode* lnode = nullptr;
        EgcNode* rnode = nullptr;
        enum FormulaScrElement::SideNode rside;
        enum FormulaScrElement::SideNode lside;
        if (m_iter.hasNext()) {
                FormulaScrElement &el = m_iter.peekNext();
                rnode = el.m_node;
                rside = el.m_sideNode;
        }
        if (m_iter.hasPrevious()) {
                FormulaScrElement &el = m_iter.peekPrevious();
                lnode = el.m_node;
                lside = el.m_sideNode;
        }

        // remove double empty elements
        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                if (isEmptyElement(false) && isEmptyElement(true))
                        m_iter.remove(true);
        }

        // remove empty elements that follow direct after the right side of any operation
        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                if (isEmptyElement(false) && lnode) {
                        if (lnode->isOperation() && lside == FormulaScrElement::nodeRightSide)
                                m_iter.remove(false);
                }
        }

        // also remove empty elements that are followed direct after the left side of any operation
        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                if (isEmptyElement(true) && rnode) {
                        if (rnode->isOperation() && rside == FormulaScrElement::nodeLeftSide)
                                m_iter.remove(false);
                }
        }

        //sanitize empty elements that come direct after numbers or variables
        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                if (lnode && rnode) {
                        EgcNodeType ltype = lnode->getNodeType();
                        EgcNodeType rtype = rnode->getNodeType();
                        if (    (    ltype == EgcNodeType::NumberNode
                                  || ltype == EgcNodeType::VariableNode
                                  || ltype == EgcNodeType::AlnumNode)
                             && isEmptyElement(false))
                                m_iter.remove(false);
                        if (    (    rtype == EgcNodeType::NumberNode
                                  || rtype == EgcNodeType::VariableNode
                                  || rtype == EgcNodeType::AlnumNode)
                             && isEmptyElement(true))
                                m_iter.remove(true);

                }
        }

        //sanitize subscripts
        if (m_iter.hasPrevious()) {
                if (isVarsubscriptSeparator()) {
                        if (m_iter.hasNext()) {
                                EgcNode* n = m_iter.peekNext().m_node;
                                if (n) {
                                        if (    n->getNodeType() != EgcNodeType::VariableNode
                                                        && n->getNodeType() != EgcNodeType::AlnumNode)
                                                insertEmptyNode();
                                }
                        } else {
                                insertEmptyNode();
                                (void) m_iter.previous();
                        }
                }
        }
}

void FormulaModificator::sanitizeSpecials(FormulaScrElement el, bool previous)
{
        if (!el.m_node)
                return;

        EgcNodeType type = el.m_node->getNodeType();
        EgcNode* previousNode = nullptr;
        EgcNodeType previousType = EgcNodeType::NodeUndefined;
        if (m_iter.hasPrevious()) {
                previousNode = m_iter.peekPrevious().m_node;
                if (previousNode)
                        previousType = previousNode->getNodeType();
        }

        switch(type) {
        case EgcNodeType::FunctionNode:
                if (    (!m_iter.hasPrevious() || previousType != EgcNodeType::FunctionNode)
                     && (m_iter.hasNext())) {
                        if (m_iter.peekNext().m_value == "(") {
                                insertEmptyNode();
                                (void) m_iter.previous();
                        }
                }
                break;
        }
}

void FormulaModificator::rmSegmented(bool previous)
{
        FormulaScrElement *el = nullptr;
        bool deleteAll = false;  // if true all elements between segmented element nodes are deleted
        EgcNode *node = nullptr;

        if (previous && m_iter.hasPrevious()) {
                el = &m_iter.peekPrevious();
                if (!el->m_isSegmented)
                        return;
                el->m_isPositionMarker = true;
        } else if (!previous && m_iter.hasNext()) {
                el = &m_iter.peekNext();
                if (!el->m_isSegmented)
                        return;
                el->m_isPositionMarker = true;
        } else {
                return;
        }

        if (!el)
                return;
        if (    el->m_sideNode == FormulaScrElement::nodeLeftSide
             || el->m_sideNode == FormulaScrElement::nodeRightSide)
                deleteAll = true;

        node = el->m_node;
        if (!node)
                return;

        if (node->isUnaryNode())
                deleteAll = false;

        //remove all segmented elements
        m_iter.toFront();
        bool deletingInProgress = false;
        bool toDelete = false;
        while(m_iter.hasNext()) {
                toDelete = false;
                FormulaScrElement &i = m_iter.next();
                if (!deleteAll) {
                        if (i.m_node == node && i.m_isSegmented && !i.m_isPositionMarker)
                                m_iter.remove(true);
                } else {
                        if (i.m_node == node && i.m_sideNode == FormulaScrElement::nodeLeftSide)
                                deletingInProgress = true;
                        if (deletingInProgress && !i.m_isPositionMarker)
                                toDelete = true;
                        if (i.m_node == node && i.m_sideNode == FormulaScrElement::nodeRightSide) {
                                deletingInProgress = false;
                        }
                        if (toDelete)
                                m_iter.remove(true);
                }
        }

        //remove element with iterator marker
        m_iter.toFront();
        while(m_iter.hasNext()) {
                FormulaScrElement &i = m_iter.next();
                if (i.m_isPositionMarker) {
                        m_iter.remove(previous);
                        break;
                }
        }
}

