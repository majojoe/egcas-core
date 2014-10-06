#include "egcexpressionnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"

#include <QDebug>

EgcExpressionNodeIterator::EgcExpressionNodeIterator(EgcFormulaExpression& formula) :
        m_cursor(&formula.getRootElement()), m_rootElement(&formula.getRootElement()),
        m_atBegin(true), m_atEnd(false)
{
}

EgcExpressionNodeIterator::~EgcExpressionNodeIterator()
{
}

bool EgcExpressionNodeIterator::findNext(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode* node;
        bool found = false;

        while (iter.hasNext()) {
                node = &(iter.next());
                if (node->getNodeType() == type) {
                        found = true;
                        m_cursor = iter.m_cursor;
                        break;
                }
        }

        if (!found)
                this->toBack();

        return found;
}

bool EgcExpressionNodeIterator::findPrevious(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode* node;
        bool found = false;

        while (iter.hasPrevious()) {
                node = &(iter.previous());
                if (node->getNodeType() == type) {
                        found = true;
                        m_cursor = iter.m_cursor;
                        break;
                }
        }

        if (!found)
                this->toFront();

        return found;
}

bool EgcExpressionNodeIterator::hasNext(void) const
{
        if (m_rootElement->isContainer()) {
                if (m_rootElement->isBinaryExpression()) {
                        EgcBinaryExpressionNode* binary = static_cast<EgcBinaryExpressionNode*>(m_rootElement);
                        if (    (binary->getLeftChild() == nullptr)
                             && (binary->getRightChild() == nullptr))
                                return false;
                } else { //unary container
                        if (static_cast<EgcUnaryExpressionNode*>(m_rootElement)->getChild() == nullptr)
                                return false;
                }
        } else {
                return false;
        }

        return !m_atEnd;
}

bool EgcExpressionNodeIterator::hasPrevious(void) const
{
        if (m_rootElement->isContainer()) {
                if (m_rootElement->isBinaryExpression()) {
                        EgcBinaryExpressionNode* binary = static_cast<EgcBinaryExpressionNode*>(m_rootElement);
                        if (    (binary->getLeftChild() == nullptr)
                             && (binary->getRightChild() == nullptr))
                                return false;
                } else { //unary container
                        if (static_cast<EgcUnaryExpressionNode*>(m_rootElement)->getChild() == nullptr)
                                return false;
                }
        } else {
                return false;
        }

        return !m_atBegin;
}

EgcExpressionNode& EgcExpressionNodeIterator::next(void)
{
        EgcExpressionNode& tempCursor = *m_cursor;
        m_cursor = &getNextElement(nullptr, &m_atEnd);

        return tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekNext(void) const
{
        return *m_cursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekPrevious(void) const
{
        return getPreviousElement(nullptr, nullptr);
}

EgcExpressionNode& EgcExpressionNodeIterator::previous(void)
{
        m_cursor = &getPreviousElement(&m_atBegin, nullptr);
        return *m_cursor;
}

void EgcExpressionNodeIterator::toBack(void)
{
        m_cursor = m_rootElement;
        m_atBegin = false;
        m_atEnd = true;
}

void EgcExpressionNodeIterator::toFront(void)
{
        m_cursor = m_rootElement;
        m_atBegin = true;
        m_atEnd = false;
}

EgcExpressionNodeIterator& EgcExpressionNodeIterator::operator=(const EgcFormulaExpression & tree)
{
        m_rootElement = &tree.getRootElement();
        m_cursor = m_rootElement;

        return *this;
}

EgcExpressionNodeIterator& EgcExpressionNodeIterator::operator=(const EgcExpressionNode & node)
{

        EgcExpressionNode* tempNode = const_cast<EgcExpressionNode*>(&node);
        EgcExpressionNode* parent = tempNode->getParent();
        while (parent) {
                tempNode = parent;
                parent = tempNode->getParent();
        };

        m_rootElement = tempNode;
        m_cursor = m_rootElement;

        return *this;
}

EgcExpressionNode& EgcExpressionNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool searchUpwards = true;
        bool beginning = false;
        bool end = false;

        if (tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        } else { //the left child is null, maybe the right child is not null
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                        searchUpwards = false;
                                }
                        }
                 } else {  //this is a unary container
                        tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        }
                }
        }

        if (searchUpwards) {
                //search for a binary containter whose node has not been visited yet,
                //since only that one can have another unvisited node.
                do {
                        EgcExpressionNode* parent = tempCursor->getParent();
                        if (parent) {
                                if (parent->isBinaryExpression()) {
                                        if (static_cast<EgcBinaryExpressionNode*>(parent)->getLeftChild() == tempCursor) {
                                                if (static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild()) {
                                                        tempCursor = static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild();
                                                        break; //next element found
                                                }
                                        }
                                }
                                tempCursor = parent;
                        } else { // if the parent pointer is null, this is the root element or the node is invalid
                                //return the root element since the rest of the tree does not really exist or it is
                                //already the root element.
                                tempCursor = m_rootElement;                                
                                break;
                        }
                } while (tempCursor != m_rootElement);
        }
        if (tempCursor == m_rootElement)
                end = true;

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;

        return *tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool beginning = false;
        bool end = false;

        tempPointer = m_cursor->getParent();
        if (tempPointer) {
                if (tempPointer->isUnaryExpression()) {
                        tempCursor = tempPointer;
                } else { // is a binary expression
                        if (isRightChild(*tempPointer, *tempCursor)) { // is a right child
                                //check left child
                                EgcBinaryExpressionNode* parent = static_cast<EgcBinaryExpressionNode*>(tempPointer);
                                tempPointer = parent->getLeftChild();
                                if (tempPointer) {
                                        tempCursor = &findNextRightMostLeaf(*tempPointer);
                                } else {
                                        tempCursor = parent;
                                }
                        } else { //is a left child
                                tempCursor = tempPointer;
                        }
                }
        } else {
                //this seems to be the root node, so jump to the back
                tempCursor = &findNextRightMostLeaf(*m_cursor);
        }

        if (tempCursor->getParent() == nullptr)
                beginning = true;

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;

        return *tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::findNextRightMostLeaf(EgcExpressionNode& start) const
{
        EgcExpressionNode* tempCursor = &start;
        EgcExpressionNode* tempPointer = nullptr;

        do {
                if (tempCursor->isContainer()) {
                        if (tempCursor->isBinaryExpression()) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else { // check the left child also, maybe this is not null
                                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                        if (tempPointer) {
                                                tempCursor = tempPointer;
                                        } else { // this binary container has no childs, so this is also a leaf.
                                                break;
                                        }
                                }
                        } else {
                                tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else { // we found a unary container without child, so this is a leaf
                                        break;
                                }
                        }
                } else { // we found a leaf, so return
                        break;
                }
        } while (tempPointer);

        return *tempCursor;
}

bool EgcExpressionNodeIterator::isRightChild(EgcExpressionNode& parent, EgcExpressionNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryExpressionNode&>(parent).getRightChild() == &child)
                        return true;
        }

        return false;
}

bool EgcExpressionNodeIterator::isLeftChild(EgcExpressionNode& parent, EgcExpressionNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryExpressionNode&>(parent).getLeftChild() == &child)
                        return true;
        }

        return false;
}
