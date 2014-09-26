#include "egcexpressionnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"

#include <QDebug>

EgcExpressionNodeIterator::EgcExpressionNodeIterator(EgcFormulaExpression& formula) :
        m_cursor(&formula.getRootElement()), m_rootElement(formula.getRootElement())
{
}

EgcExpressionNodeIterator::~EgcExpressionNodeIterator()
{
}

bool EgcExpressionNodeIterator::findNext(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode& node = iter.peekNext();
        bool found = false;

        while (iter.hasNext()) {
                node = iter.next();
                if (node.getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        return found;
}

bool EgcExpressionNodeIterator::findPrevious(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode& node = iter.peekPrevious();
        bool found = false;

        while (iter.hasPrevious()) {
                node = iter.previous();
                if (node.getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        return found;
}

bool EgcExpressionNodeIterator::hasNext(void) const
{
        if (&getNextElement() != &m_rootElement)
                return true;
        else
                return false;
}

bool EgcExpressionNodeIterator::hasPrevious(void) const
{
        if (&getPreviousElement() != &m_rootElement)
                return true;
        else
                return false;
}

EgcExpressionNode& EgcExpressionNodeIterator::next(void)
{
        m_cursor = &getNextElement();
        return *m_cursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekNext(void) const
{
        return getNextElement();
}

EgcExpressionNode& EgcExpressionNodeIterator::peekPrevious(void) const
{
        return getPreviousElement();
}

EgcExpressionNode& EgcExpressionNodeIterator::previous(void)
{
        m_cursor = &getPreviousElement();
        return *m_cursor;
}

void EgcExpressionNodeIterator::toBack(void)
{
        m_cursor = &m_rootElement;
}

void EgcExpressionNodeIterator::toFront(void)
{
        m_cursor = &m_rootElement;
}

EgcExpressionNodeIterator& EgcExpressionNodeIterator::operator=(const EgcFormulaExpression & tree)
{
        m_rootElement = tree.getRootElement();
        m_cursor = &m_rootElement;

        return *this;
}

EgcExpressionNode& EgcExpressionNodeIterator::getNextElement(void) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool searchUpwards = true;

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
                                        if (static_cast<EgcBinaryExpressionNode*>(parent)->getLeftChild() == tempCursor)
                                                tempCursor = static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild();
                                        break; //next element found
                                }
                                tempCursor = parent;
                        } else { // if the parent pointer is null, this is the root element or the node is invalid
                                //return the root element since the rest of the tree does not really exist or it is
                                //already the root element.
                                tempCursor = &m_rootElement;
                                break;
                        }
                } while (tempCursor != &m_rootElement);
        }

        return *tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::getPreviousElement(void) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool searchUpwards = true;

        if (tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        } else { //the left child is null, maybe the right child is not null
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
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
                                        if (static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild() == tempCursor)
                                                tempCursor = static_cast<EgcBinaryExpressionNode*>(parent)->getLeftChild();
                                        break; //next element found
                                }
                                tempCursor = parent;
                        } else { // if the parent pointer is null, this is the root element or the node is invalid
                                //return the root element since the rest of the tree does not really exist or it is
                                //already the root element.
                                tempCursor = &m_rootElement;
                                break;
                        }
                } while (tempCursor != &m_rootElement);
        }

        return *tempCursor;
}
