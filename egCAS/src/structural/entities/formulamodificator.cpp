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
#include "../specialNodes/egcbinarynode.h"
#include "../specialNodes/egcunarynode.h"
#include "../specialNodes/egcflexnode.h"
#include "../visitor/formulascrvisitor.h"
#include "egcabstractformulaitem.h"
#include "casKernel/parser/restructparserprovider.h"
#include "casKernel/parser/abstractkernelparser.h"
#include "../concreteNodes/egcalnumnode.h"

const char emptyElement[] = "_empty";
const char emptyBinElement[] = "_emptybinop";



// macro for function arguments for inserting functions
#define FNC_ARG(x) do{ if (arg##x.isEmpty()) {                                          \
                            el.m_value = ")";                                           \
                            m_iter.insert(el);                                          \
                            return;                                                     \
                    } else {                                                            \
                            el.m_value = ",";                                           \
                            m_iter.insert(el);                                          \
                            if (nodeToSurround && stdPos == x) {                        \
                                    moveCursorToRightVisibleMargin(*nodeToSurround);    \
                            } else {                                                    \
                                    el.m_value = arg##x;                                \
                                    m_iter.insert(el);                                  \
                            }                                                           \
                            if (stdPos == x)                                            \
                                    saveCursorPosition();                               \
                   }}while(0)



FormulaModificator::FormulaModificator(EgcFormulaEntity& formula) : m_formula{formula},
                                                                    m_iter(FormulaScrIter(m_formula, m_vector)),
                                                                    m_underlinedNode{nullptr},
                                                                    m_startUnderlinedNode{nullptr},
                                                                    m_changeAwaited{false},
                                                                    m_underlineCursorLeft{false},
                                                                    m_cursorPos{0},
                                                                    m_cursorPosSaved{false},
                                                                    m_insRightPtrAtNodeBegin{false}
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

        if (!m_underlinedNode) {
                if (forward) {
                        if (m_iter.hasNext())
                                (void) m_iter.next();
                } else {
                        if (m_iter.hasPrevious())
                                (void) m_iter.previous();
                }
        } else {
                if (forward && m_underlineCursorLeft) {
                        moveCursorToRightVisibleMargin(*m_underlinedNode);
                        m_underlineCursorLeft = false;
                } else if (!forward && !m_underlineCursorLeft) {
                        moveCursorToLeftVisibleMargin(*m_underlinedNode);
                        m_underlineCursorLeft = true;
                } else {
                        resetUnderline();
                }
        }

        showCurrentCursor();
        if (isUnderlineActive())
                m_formula.getItem()->showUnderline(id(m_underlinedNode));
}

void FormulaModificator::resetUnderline(void)
{
        m_underlinedNode = nullptr;
}

bool FormulaModificator::isUnderlineActive(void)
{
        if (m_underlinedNode)
                return true;
        else
                return false;
}

quint32 FormulaModificator::id(EgcNode* node) const
{
        const EgcMathmlLookup lookup = m_formula.getMathmlMappingCRef();
        if (node)
                return lookup.getIdFrame(*node);
        else
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
                restoreCursorPosition(m_tempIterData);
                showCurrentCursor();
        }
}

void FormulaModificator::removeBinEmptyNodes(void)
{
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
}

bool FormulaModificator::isEmptyChildNeeded4Binary(bool leftChild)
{
        bool retval = false;

        if (leftChild) {
                if (!m_iter.hasPrevious()) {
                        retval = true;
                } else {
                        FormulaScrElement &l_el = m_iter.peekPrevious();
                        EgcNode* lNode = l_el.m_node;
                        if (lNode) {
                                if (    lNode->isOperation()
                                                && (    l_el.m_sideNode == FormulaScrElement::nodeLeftSide
                                                        || l_el.m_sideNode == FormulaScrElement::nodeMiddle))
                                        retval = true;
                        }
                }
        } else {
                if (!m_iter.hasNext()) {
                        retval = true;
                } else {
                        FormulaScrElement &r_el = m_iter.peekNext();
                        EgcNode* rNode = r_el.m_node;
                        if (rNode) {
                                if (    rNode->isOperation()
                                     && (    r_el.m_sideNode == FormulaScrElement::nodeRightSide
                                          || r_el.m_sideNode == FormulaScrElement::nodeMiddle))
                                        retval = true;
                        }
                }
        }

        return retval;
}

void FormulaModificator::insertBinaryOperation(QString op, QString left, QString right, bool insertLRAlways)
{
        bool insertEmptyLeft = false;
        bool insertEmptyRight = false;
        FormulaScrElement el;
        el.m_value = op;
        EgcNode* splitNode = nullptr;
        bool integrateInLeftChild = true;

        // remove binary empty nodes at right or left side of the current cursor
        removeBinEmptyNodes();

        insertEmptyLeft = isEmptyChildNeeded4Binary(true);
        insertEmptyRight = isEmptyChildNeeded4Binary(false);

        if (    (!insertEmptyLeft || !insertEmptyRight)
             && (!left.isEmpty() && !right.isEmpty()) ) { //a splitting node is needed
                if (m_underlinedNode) {
                        splitNode = m_underlinedNode;
                        if (m_underlineCursorLeft)
                                integrateInLeftChild = false;
                } else { // no node is underlined
                        if (m_iter.hasPrevious()) {
                                splitNode = m_iter.peekPrevious().m_node;
                        } else {
                                splitNode = m_iter.peekNext().m_node;
                                integrateInLeftChild = false;
                        }
                }
        }

        if (insertEmptyLeft) {
                if (insertLRAlways)
                        insertEl(left);
                insertEmptyNode();
                saveCursorPosition();
                if (insertLRAlways)
                        insertEl(right);
        } else {
                insertEl(right);
        }

        m_iter.insert(el);

        if (!insertEmptyLeft && !insertEmptyRight)
                saveCursorPosition();

        if (!insertEmptyRight) {
                insertEl(left);
        } else {
                if (insertLRAlways)
                        insertEl(left);
                insertEmptyNode();
                if (!insertEmptyLeft) {
                        saveCursorPosition();
                }
                if (insertLRAlways)
                        insertEl(right);
        }

        //insert the child nodes if any nodes to insert
        if (splitNode) {
                if (integrateInLeftChild) {
                        moveCursorToLeftVisibleMargin(*splitNode);
                        insertEl(left);
                } else {
                        moveCursorToRightVisibleMargin(*splitNode);
                        insertEl(right);
                }
        }
}

void FormulaModificator::insertUnaryOperation(QString left, QString right)
{
        FormulaScrElement lel;
        FormulaScrElement rel;
        bool both = false;
        bool leftOnly = false;
        bool rightOnly = false;

        if (!left.isEmpty() && !right.isEmpty())
                both = true;
        if (!left.isEmpty() && right.isEmpty())
                leftOnly = true;
        if (left.isEmpty() && !right.isEmpty())
                rightOnly = true;

        if (!m_underlinedNode) {
                if (!left.isEmpty()) {
                        lel.m_value = left;
                        if (isEmptyElement(true))
                                m_iter.remove(true);
                } else if (!right.isEmpty()) {
                        rel.m_value = right;
                        if (isEmptyElement(false))
                                m_iter.remove(false);
                }

                if (!left.isEmpty()) {
                        m_iter.insert(lel);
                        if (leftOnly)
                                saveCursorPosition();
                }
                if(both) {
                        insertEmptyNode();
                        saveCursorPosition();
                }
                if (!right.isEmpty()) {
                        m_iter.insert(rel);
                        if (rightOnly)
                                saveCursorPosition();
                }
        } else {
                if(left.isEmpty() || right.isEmpty())
                        return; //this is not allowed since it produces unexpected behaviour

                if (!m_underlineCursorLeft)
                        moveCursorToLeftVisibleMargin(*m_underlinedNode);
                insertEl(left);
                if (m_underlineCursorLeft)
                        saveCursorPosition();
                moveCursorToRightVisibleMargin(*m_underlinedNode);
                insertEl(right);
                if (!m_underlineCursorLeft)
                        saveCursorPosition();
        }
}

bool FormulaModificator::isNumberOrVariable(bool right)
{
        bool retval = false;

        if (right) {
                if (m_iter.hasNext()) {
                        EgcNode *nd = m_iter.peekNext().m_node;
                        if (nd) {
                                if (    nd->getNodeType() == EgcNodeType::NumberNode
                                     || nd->getNodeType() == EgcNodeType::VariableNode)
                                        retval = true;
                        }
                }
        } else {
                if (m_iter.hasPrevious()) {
                        EgcNode *nd = m_iter.peekPrevious().m_node;
                        if (nd) {
                                if (    nd->getNodeType() == EgcNodeType::NumberNode
                                     || nd->getNodeType() == EgcNodeType::VariableNode)
                                        retval = true;
                        }
                }
        }

        return retval;
}

void FormulaModificator::insertFunction(QString name, quint32 stdPos, QString arg1, QString arg2, QString arg3, QString arg4, QString arg5, QString arg6)
{
        EgcNode* nodeToSurround = nullptr;

        //get the node to surround with the function if one
        if (m_underlinedNode) {
                nodeToSurround = m_underlinedNode;
        } else {
                if (m_iter.hasNext()) {
                        if (isNumberOrVariable(true)) {
                                if (m_iter.peekNext().m_node)
                                        nodeToSurround = m_iter.peekNext().m_node;
                        }
                }
                if (m_iter.hasPrevious()) {
                        if (isNumberOrVariable(false)) {
                                if (m_iter.peekPrevious().m_node)
                                        nodeToSurround = m_iter.peekPrevious().m_node;
                        }
                }
        }

        if (nodeToSurround)
                moveCursorToLeftVisibleMargin(*nodeToSurround);


        if (isEmptyElement(true)) {
                m_iter.remove(true);
        }
        if (isEmptyElement(false)) {
                m_iter.remove(false);
        }

        FormulaScrElement el;
        el.m_value = name;
        m_iter.insert(el);
        if (stdPos == 0)
                saveCursorPosition();
        el.m_value = "(";
        m_iter.insert(el);

        if (nodeToSurround && stdPos == 1) {
                moveCursorToRightVisibleMargin(*nodeToSurround);
        } else {
                el.m_value = arg1;
                m_iter.insert(el);
        }
        if (stdPos == 1)
                saveCursorPosition();


        FNC_ARG(2);
        FNC_ARG(3);
        FNC_ARG(4);
        FNC_ARG(5);
        FNC_ARG(6);

        el.m_value = ")";
        m_iter.insert(el);

}

void FormulaModificator::insertCharacter(QChar character)
{
        bool insertBinaryEmptyNode = false;
        FormulaScrElement el;
        el.m_value = EgcAlnumNode::encode(character);

        //checks for special cursor conditions when inserting characters
        checkForSpecialCursorConditions();

        m_iter.save();
        resetUnderline();

        if (isEmptyElement(true)) {
                m_iter.remove(true);
        }
        if (isEmptyElement(false)) {
                m_iter.remove(false);
        }

        if (m_iter.hasNext()) {
                if (m_iter.peekNext().m_node) {
                        if (m_iter.peekNext().m_node->getNodeType() == EgcNodeType::VariableNode)
                                insertBinaryEmptyNode = true;
                }
        }

        m_iter.insert(el);
        if (insertBinaryEmptyNode) {
                insertBinEmptyNode();
                (void) m_iter.previous();
        }
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

        m_iter.peekPrevious().markPosition();
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
                if (i.hasPositionMarker()) {
                        i.resetPositionMarker();
                        if (pendantFound) {
                                if (!left)
                                        i.m_value = QString(")");
                                else
                                        i.m_value = QString("(");
                        }
                        break;
                }
        }
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

void FormulaModificator::convertVarToFunc(void)
{
        FormulaScrElement el;
        el.m_value = "(";
        m_iter.insert(el);
        insertEmptyNode();
        el.m_value = ")";
        m_iter.insert(el);
        m_iter.previous();
        m_iter.previous();
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

bool FormulaModificator::rightSide(void) const
{
        bool retval = true;

        if (!m_iter.hasPrevious()) { //is at front
                retval = true;
        } else {
                if (m_iter.hasNext()) {
                        FormulaScrElement& lel = m_iter.peekPrevious();
                        FormulaScrElement& rel = m_iter.peekNext();
                        if (static_cast<int>(lel.m_cAdh) < static_cast<int>(rel.m_cAdh))
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

FormulaScrElement& FormulaModificator::elementAtCursor() const
{
        if (rightSide() && m_iter.hasNext())
                return m_iter.peekNext();
        else if (m_iter.hasPrevious())
                return m_iter.peekPrevious();
}

quint32 FormulaModificator::subPosition(void) const
{
        quint32 retval;

        if (rightSide())
                retval = m_iter.peekNext().lTemp.m_subpos;
        else
                retval = m_iter.peekPrevious().rTemp.m_subpos;

        return retval;
}

void FormulaModificator::saveCursorPosition(void)
{
        FormulaScrElement* lel = nullptr;
        FormulaScrElement* rel = nullptr;
        if (m_iter.hasPrevious())
                lel = &m_iter.peekPrevious();
        if (m_iter.hasNext())
                rel = &m_iter.peekNext();

        m_cursorPos = 0;

        if (rel && !lel) {
                insertRightPointer();
        } else {
                insertLeftPointer();
        }

/*        if (rightPtrAtNodeBeginToBeInserted()) {
                // a right pointer has to be inserted at node begin
                m_cursorPos = findAlnumBegin();
                insertRightPointer();
                //move the cursor back to the old position
                for (quint32 i = 0; i < m_cursorPos; i++)
                        (void) m_iter.next();
        } else if (rel && !lel) {
                insertRightPointer();
        } else if (lel && !rel) {
                //m_cursorPos = findAlnumBegin();
                insertLeftPointer();
        } else if (lel && rel) {
                if (rel->m_node && rel->m_sideNode == FormulaScrElement::nodeLeftSide) {
                        insertRightPointer();
                } else if (lel->m_node && lel->m_sideNode == FormulaScrElement::nodeRightSide) {
                        //m_cursorPos = findAlnumBegin();
                        insertLeftPointer();
                } else if (isAlnum(lel->m_value) && !isAlnum(rel->m_value)) {
                        insertLeftPointer();
                } else if (!isAlnum(lel->m_value) && isAlnum(rel->m_value)) {
                        insertRightPointer();
                } else if (isAlnum(lel->m_value) && isAlnum(rel->m_value)) {
                        m_cursorPos = findAlnumBegin();
                        insertRightPointer();
                        //move the cursor back to the old position
                        for (quint32 i = 0; i < m_cursorPos; i++)
                                (void) m_iter.next();
                } else if (isEmptyElement(false)) {
                        insertRightPointer();
                } else if (isEmptyElement(true)) {
                        insertLeftPointer();
                } else if (!lel->m_node) {
                        insertLeftPointer();
                } else if (!rel->m_node) {
                        insertRightPointer();
                }
        } else {
                insertRightPointer();
        }*/
}

void FormulaModificator::insertLeftPointer()
{
        m_iter.insert(FormulaScrElement(QString("_<L")));
}

void FormulaModificator::insertRightPointer()
{
        m_iter.insert(FormulaScrElement(QString("_>R")));
}

bool FormulaModificator::isAlnum(QString val) const
{
        if (EgcAlnumNode::isAlnum(val))
                return true;
        if (val == ".")
                return true;

        return false;
}

quint32 FormulaModificator::findAlnumBegin()
{
        FormulaScrIter iter = m_iter;
        quint32 i = 0;

        if (!iter.hasPrevious())
                return i;

        if (iter.hasPrevious()) {
                if (!isAlnum(iter.peekPrevious().m_value))
                        return i;
        }

        while(iter.hasPrevious()) {
                (void) iter.previous();
                i++;
                if (!isAlnum(iter.peekPrevious().m_value))
                        break;
        }

        m_iter = iter;

        return i;
}

void FormulaModificator::restoreCursorPosition(NodeIterReStructData& iterData)
{
        FormulaScrIter iter = m_iter;
        bool searchFromBack = false;

        if (iterData.m_node && !iterData.m_isLeftPointer && iterData.m_offset == 0) {
                if (!iterData.m_node->isContainer())
                        searchFromBack = true;
        }

        if (searchFromBack) { //search for right side number and variable nodes only
                iter.toBack();
                do {
                        if (iter.hasPrevious()) {
                                FormulaScrElement &el = iter.peekPrevious();
                                if (el.m_node == iterData.m_node && !iterData.m_isLeftPointer) {
                                                break;
                                }
                        }

                        (void) iter.previous();
                } while(iter.hasPrevious());

        } else {
                iter.toFront();
                do {
                        if (iter.hasNext()) {
                                FormulaScrElement &el = iter.peekNext();
                                if (el.m_node == iterData.m_node && iterData.m_isLeftPointer) {
                                        if (el.m_sideNode == FormulaScrElement::nodeLeftSide)
                                                break;
                                        else if (el.m_sideNode == FormulaScrElement::nodeMiddle && el.m_node) {
                                                if (!el.m_node->isContainer())
                                                        break;
                                        }
                                }
                        }
                        if (iter.hasPrevious()) {
                                FormulaScrElement &el = iter.peekPrevious();
                                if (el.m_node == iterData.m_node && !iterData.m_isLeftPointer) {
                                        if (el.m_sideNode == FormulaScrElement::nodeRightSide)
                                                break;
                                        else if (el.m_sideNode == FormulaScrElement::nodeMiddle && el.m_node) {
                                                if (!el.m_node->isContainer())
                                                        break;
                                        }
                                }
                        }

                        (void) iter.next();
                } while(iter.hasNext());

                // go to the element position (inside number or variable name) saved
                quint32 i;
                for (i = 0; i < iterData.m_offset; i++) {
                        if (iter.hasNext())
                                (void) iter.next();
                }
        }

        m_iter = iter;
}

void FormulaModificator::sanitizeEmptyCursorPos()
{
        FormulaScrIter iter = m_iter;
        iter.toFront();
        while(iter.hasNext()) {
                FormulaScrElement &el = iter.next();
                if (el.isEmptyElement() && iter.hasNext()) {
                        if (iter.peekNext().isLeftCursorPointer()) {
                                iter.remove(false);
                                iter.previous();
                                FormulaScrElement rPtr;
                                rPtr.m_value = QString("_>R");
                                iter.insert(rPtr);
                                break;
                        }
                }
        }
}

void FormulaModificator::setRightPtrAtNodeBeginToBeInserted(void)
{
        m_insRightPtrAtNodeBegin = true;
}

bool FormulaModificator::rightPtrAtNodeBeginToBeInserted()
{
        bool temp = m_insRightPtrAtNodeBegin;
        m_insRightPtrAtNodeBegin = false;

        return temp;
}

void FormulaModificator::checkForSpecialCursorConditions()
{
        // check if we are in the middle or at the end of a function node name
        if (m_iter.hasNext() && m_iter.hasPrevious()) {
                FormulaScrElement rel = m_iter.peekNext();
                FormulaScrElement lel = m_iter.peekPrevious();
                if (     rel.m_node == lel.m_node
                     && lel.m_sideNode == FormulaScrElement::nodeLeftSide
                     && rel.m_sideNode == FormulaScrElement::nodeLeftSide) {
                        if (lel.m_node) {
                                if (lel.m_node->getNodeType() == EgcNodeType::FunctionNode)
                                        setRightPtrAtNodeBeginToBeInserted();
                        }
                }
        } else if (m_iter.hasNext() && !m_iter.hasPrevious()) {
                FormulaScrElement rel = m_iter.peekNext();
                if (rel.m_sideNode == FormulaScrElement::nodeLeftSide) {
                        if (rel.m_node) {
                                if (rel.m_node->getNodeType() == EgcNodeType::FunctionNode)
                                        setRightPtrAtNodeBeginToBeInserted();
                        }
                }
        }

}

bool FormulaModificator::reStructureTree()
{
        bool retval = true;

        RestructParserProvider pp;
        FormulaScrVisitor restructVisitor(m_formula, m_iter);
        QString result = restructVisitor.getResult();
        int errCode;
        EgcNode* tree = pp.getRestructParser()->restructureFormula(result, m_tempIterData, &errCode);
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

        saveCursorPosition();

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

bool FormulaModificator::searchCursorPos(FormulaScrIter& iter, EgcNode& node, quint32 subPos, bool rightSide,
                                         bool sideMatters, FormulaScrElement::SideNode side, bool subposRelevant)
{
        while(iter.hasNext()) {
                FormulaScrElement& el = iter.next();
                if (el.m_node == &node) {
                        if (sideMatters && el.m_sideNode != side)
                                continue;
                        if (subposRelevant) {
                                if (subPos < el.m_subpos_min || subPos > el.m_subpos_max)
                                        continue;
                                if (subPos != 0) { // e.g. for builtin function names
                                        if (el.m_subpos_min != el.m_subpos_max) {
                                                quint32 mid = subPos - el.m_subpos_min;
                                                if (mid > (el.m_subpos_max - el.m_subpos_min) / 2 )
                                                        rightSide = true;
                                                else
                                                        rightSide = false;
                                        }
                                }
                        }
                        if (!rightSide)
                                iter.previous();
                        m_iter = iter;
                        return true;
                }
        }

        return false;
}

EgcNode* FormulaModificator::getParent(EgcNode& node)
{
        EgcNode* parent = & node;
        do {
                parent = parent->getParent();
                if (!parent)
                        return nullptr;
        } while (!m_iter.contains(*parent));

        return parent;
}

bool FormulaModificator::isCursorNearLeftSideParent(EgcNode& node) const
{
        bool leftSide = true;
        EgcNode& curr = nodeAtCursor();
        EgcNode* child = &curr;

        EgcContainerNode* parent = curr.getParent();
        if (!parent)
                return leftSide;

        while(parent != nullptr) {
                if (parent->isBinaryNode()) {
                        if (parent->isLastChild(*child))
                                leftSide = false;
                } else if (parent->isFlexNode()) {
                        EgcFlexNode& flex = static_cast<EgcFlexNode&>(*parent);
                        quint32 index = 0;
                        quint32 nr_childs;
                        (void) flex.getIndexOfChild(*child, index);
                        nr_childs = flex.getNumberChildNodes();
                        if (index >= nr_childs/2)
                                leftSide = false;
                } else if (parent->isUnaryNode()) {
                        if (elementAtCursor().m_sideNode == FormulaScrElement::nodeRightSide)
                                leftSide = false;
                } else {
                        quint32 pos = subPosition();
                        quint32 subind = static_cast<quint32>(curr.nrSubindexes());

                        if (pos >= (subind/2))
                                leftSide = false;
                }
                if (parent == &node)
                        break;
                child = parent;
                parent = child->getParent();
        }

        return leftSide;
}

void FormulaModificator::moveCursorToRightVisibleMargin(EgcNode& node)
{
        FormulaScrIter iter = m_iter;
        findRightVisibleMargin(node, iter);
        m_iter = iter;
}

void FormulaModificator::moveCursorToLeftVisibleMargin(EgcNode& node)
{
        FormulaScrIter iter = m_iter;
        findLeftVisibleMargin(node, iter);
        m_iter = iter;
}

void FormulaModificator::findLeftVisibleMargin(EgcNode& node, FormulaScrIter& iter)
{
        EgcNode* child = nullptr;
        EgcNode* nd = &node;

        while(!isLeftSideVisible(*nd, iter)) {
                if (!node.isContainer())
                        return;
                EgcContainerNode* container = static_cast<EgcContainerNode*>(nd);
                child = container->getChild(0);
                if (!child)
                        return;
                nd = child;
        }
}

void FormulaModificator::findRightVisibleMargin(EgcNode& node, FormulaScrIter& iter)
{
        EgcNode* child = nullptr;
        EgcNode* nd = &node;

        while(!isRightSideVisible(*nd, iter)) {
                if (!node.isContainer())
                        return;
                EgcContainerNode* container = static_cast<EgcContainerNode*>(nd);
                quint32 nrChilds = container->getNumberChildNodes();
                if (nrChilds)
                        child = container->getChild(container->getNumberChildNodes() - 1);
                if (!child)
                        return;
                nd = child;
        }
}

bool FormulaModificator::isRightSideVisible(EgcNode& node, FormulaScrIter& iter)
{
        bool found = false;
        iter = m_iter;
        iter.toBack();
        while (iter.hasPrevious()) {
                FormulaScrElement& el = iter.previous();
                if (el.m_node == &node) {
                        if (node.isContainer() && el.m_sideNode != FormulaScrElement::nodeRightSide)
                                break;
                        (void) iter.next();
                        found = true;
                        break;
                }
        }

        return found;
}

bool FormulaModificator::isLeftSideVisible(EgcNode& node, FormulaScrIter& iter)
{
        bool found = false;
        iter = m_iter;
        iter.toFront();
        while (iter.hasNext()) {
                FormulaScrElement& el = iter.next();
                if (el.m_node == &node) {
                        if (node.isContainer() && el.m_sideNode != FormulaScrElement::nodeLeftSide)
                                break;
                        (void) iter.previous();
                        found = true;
                        break;
                }
        }

        return found;
}

QList<FormulaScrVector> FormulaModificator::split(const FormulaScrIter& leftIter, const FormulaScrIter& rightIter) const
{
        FormulaScrVector left;
        FormulaScrVector middle;
        FormulaScrVector right;

        left = leftIter.split();
        right = rightIter.split(false);

        int leftSize = left.size();
        int rightSize = right.size();
        int middleSize = m_vector.size() - (leftSize + rightSize);
        middleSize = qMax(0, middleSize);

        middle = m_vector.mid(leftSize, middleSize);

        QList<FormulaScrVector> list;

        list.append(left);
        list.append(middle);
        list.append(right);

        return list;
}

void FormulaModificator::setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide)
{
        bool sideMatters = false;
        FormulaScrElement::SideNode sideNode = FormulaScrElement::nodeMiddle;
        bool found = false;
        const EgcMathmlLookup lookup = m_formula.getMathmlMappingCRef();
        EgcNode* node = lookup.findNode(nodeId);
        if (!node)
                return;
        if (   subPos == 0
            && (node->isBinaryNode() || node->isUnaryNode() || node->isFlexNode())) {
                sideMatters = true;
                if (rightSide)
                        sideNode = FormulaScrElement::nodeRightSide;
                else
                        sideNode = FormulaScrElement::nodeLeftSide;
        }
        FormulaScrIter iter = m_iter;
        iter.toFront();
        found = searchCursorPos(iter, *node, subPos, rightSide, sideMatters, sideNode);

        //element not found so far, so use other position
        if (found)
                return;
        iter.toFront();
        if (node->isBinaryNode() || node->isFlexNode()) {
                (void) searchCursorPos(iter, *node, subPos, rightSide, false, sideNode, false);
        }
}

void FormulaModificator::markParent()
{
        EgcNode* node;

        if (!m_formula.getItem())
                return;

        if (rightSide())
                node = m_iter.peekNext().m_node;
        else
                node = m_iter.peekPrevious().m_node;

        if (!node)
                return;

        if (!m_underlinedNode) {
                m_underlinedNode = node;
                m_startUnderlinedNode = node;
        } else {
                m_underlinedNode = getParent(*m_underlinedNode);
                if (!m_underlinedNode)
                        m_underlinedNode = m_startUnderlinedNode;
        }

        if(isCursorNearLeftSideParent(*m_underlinedNode)) {
                moveCursorToLeftVisibleMargin(*m_underlinedNode);
                m_underlineCursorLeft = true;
        } else {
                moveCursorToRightVisibleMargin(*m_underlinedNode);
                m_underlineCursorLeft = false;
        }

        showCurrentCursor();
        if (isUnderlineActive()) {
                const EgcMathmlLookup lookup = m_formula.getMathmlMappingCRef();
                quint32 id = lookup.getIdFrame(*m_underlinedNode);
                m_formula.getItem()->showUnderline(id);
        }
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

void FormulaModificator::insertEl(QString el)
{
        if (el.isEmpty())
                return;
        FormulaScrElement element;        
        element.m_value = el;
        m_iter.insert(element);
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
                        if (    lnode->isBinaryNode() && rnode->isBinaryNode()
                             && m_iter.peekNext().m_sideNode != FormulaScrElement::nodeLeftSide) {
                                insertEmptyNode();
                                m_iter.previous();
                        } else if (    (lnode->isUnaryNode() ||  lnode->isFlexNode())
                                    && rnode->isBinaryNode()
                                    && m_iter.peekNext().m_sideNode == FormulaScrElement::nodeMiddle
                                    && m_iter.peekPrevious().m_sideNode == FormulaScrElement::nodeLeftSide) {
                                insertEmptyNode();
                                m_iter.previous();
                        } else if (    (rnode->isUnaryNode() || rnode->isFlexNode())
                                       && lnode->isBinaryNode()
                                       && m_iter.peekPrevious().m_sideNode == FormulaScrElement::nodeMiddle
                                       && m_iter.peekNext().m_sideNode == FormulaScrElement::nodeRightSide) {
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

void FormulaModificator::sanitizeFlexFncs(FormulaScrElement el, bool previous)
{
        (void) el;
        (void) previous;

        if (m_iter.hasPrevious() && m_iter.hasNext()) {
                EgcNode *lnode = m_iter.peekPrevious().m_node;
                EgcNode *rnode = m_iter.peekNext().m_node;
                enum FormulaScrElement::SideNode lside = m_iter.peekPrevious().m_sideNode;
                enum FormulaScrElement::SideNode rside = m_iter.peekNext().m_sideNode;
                if (lnode && rnode) {
                        if (    lnode->isFlexNode()
                             && rnode->isFlexNode()
                             && lnode == rnode
                             && (    (lside == FormulaScrElement::nodeLeftSide && rside == FormulaScrElement::nodeMiddle)
                                  || (lside == FormulaScrElement::nodeMiddle && rside == FormulaScrElement::nodeRightSide))) {
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
        enum FormulaScrElement::SideNode rside = FormulaScrElement::nodeMiddle;
        enum FormulaScrElement::SideNode lside = FormulaScrElement::nodeMiddle;

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
        (void) (previous);

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

        if (type == EgcNodeType::FunctionNode) {
                if (    (!m_iter.hasPrevious() || previousType != EgcNodeType::FunctionNode)
                     && (m_iter.hasNext())) {
                        if (m_iter.peekNext().m_value == "(") {
                                insertEmptyNode();
                                (void) m_iter.previous();
                        }
                }
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
                el->markPosition();
        } else if (!previous && m_iter.hasNext()) {
                el = &m_iter.peekNext();
                if (!el->m_isSegmented)
                        return;
                el->markPosition();
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
                        if (i.m_node == node && i.m_isSegmented && !i.hasPositionMarker())
                                m_iter.remove(true);
                } else {
                        if (i.m_node == node && i.m_sideNode == FormulaScrElement::nodeLeftSide)
                                deletingInProgress = true;
                        if (deletingInProgress && !i.hasPositionMarker())
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
                if (i.hasPositionMarker()) {
                        m_iter.remove(true);
                        break;
                }
        }
}

void FormulaModificator::removeElement(bool previous)
{
        FormulaScrElement element;

        //checks for special cursor conditions when removing characters
        checkForSpecialCursorConditions();

        m_iter.save();
        resetUnderline();

        if (isEmpty() || m_vector.size() == 1) {
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
        sanitizeFlexFncs(element, previous);

        updateFormula();
}

void FormulaModificator::insertOperation(EgcAction operation)
{
        // save iterator state
        m_iter.save();

        if (operation.m_op == EgcOperations::mathCharOperator) {
                if (operation.m_character == '-') {
                        bool unaryMinus = false;

                        // since a minus can also be a unary minus -> delete the empty node here
                        if (isEmptyChildNeeded4Binary(true)) // if a empty child would be needed for a binary minus
                                unaryMinus = true;
                        else if (isEmptyElement(true))
                                m_iter.remove(true);
                        if (unaryMinus)
                                insertUnaryOperation("-", "");
                        else
                                insertBinaryOperation("-");

                } else if (    operation.m_character == '+'
                            || operation.m_character == '*'
                            || operation.m_character == ':'
                            || operation.m_character == '='
                            || operation.m_character == '^'
                            || operation.m_character == ','
                               ) {
                        if (m_underlinedNode)
                                insertBinaryOperation(operation.m_character, "(", ")");
                        else
                                insertBinaryOperation(operation.m_character);
                } else if (operation.m_character == '/') {
                        if (m_underlinedNode)
                                insertBinaryOperation(operation.m_character, "_{", "_}", true);
                        else
                                insertBinaryOperation(operation.m_character);
                } else if (operation.m_character == QChar(177)) {
                        insertUnaryOperation("-", "");
                } else if (operation.m_character == QChar(8730)) {
                        insertFunction("_root", 2, emptyElement, emptyElement);
                } else if (operation.m_character == '(' && !m_underlinedNode) {
                        if (m_iter.peekPrevious().m_node->getNodeType() == EgcNodeType::VariableNode) {
                                convertVarToFunc();
                        } else {
                                insertRedParenthesis(true);
                        }
                } else if (operation.m_character == ')' && !m_underlinedNode) {
                        insertRedParenthesis(false);
                } else if (    ((operation.m_character == '(' && m_underlineCursorLeft)
                            || (operation.m_character == ')' && !m_underlineCursorLeft))
                            && m_underlinedNode) {
                        insertUnaryOperation("(", ")");
                }
        } else if (operation.m_op == EgcOperations::mathFunction) { // functions
                QString name;
                quint32 pos = 1;
                if (!operation.m_additionalData.isNull())  {
                        name = operation.m_additionalData.toString();
                }
                if (name.isEmpty()) {
                        name = emptyElement;
                        pos = 0;
                }
                insertFunction(name, pos, emptyElement);
        } else if (operation.m_op == EgcOperations::internalFunction) {
                if (operation.m_intType == InternalFunctionType::natLogarithm
                     || operation.m_intType == InternalFunctionType::logarithm) {
                        if (operation.m_intType == InternalFunctionType::natLogarithm)
                                insertFunction("_ln", 1, emptyElement);
                        else if (operation.m_intType == InternalFunctionType::logarithm)
                                insertFunction("_log", 1, emptyElement);
                } else if (operation.m_intType == InternalFunctionType::integral) {
                        if (operation.m_OpModificators == OpModificators::definiteIntegral) {
                                insertFunction("_integrate", 3, emptyElement, emptyElement, emptyElement, emptyElement);
                        } else {
                                insertFunction("_integrate", 1, emptyElement, emptyElement);
                        }
                } else if (operation.m_intType == InternalFunctionType::differential) {
                        if (operation.m_lookModificatiors == LookModificators::differential_lagrange_notation_1)
                                insertFunction("_diff", 2, QString("1"), emptyElement, emptyElement);
                        else if (operation.m_lookModificatiors == LookModificators::differential_lagrange_notation_2)
                                insertFunction("_diff", 2, QString("2"), emptyElement, emptyElement);
                        else if (operation.m_lookModificatiors == LookModificators::differential_lagrange_notation_3)
                                insertFunction("_diff", 2, QString("3"), emptyElement, emptyElement);
                        else
                                insertFunction("_diff", 2, QString("0"), emptyElement, emptyElement, emptyElement);
                }
        }

        sanitizeEmptyCursorPos();
        updateFormula();
        resetUnderline();
}

bool FormulaModificator::aboutToBeDeleted() const
{
        if (    isEmpty()
             || m_vector.size() == 1)
                return true;

        return false;
}
