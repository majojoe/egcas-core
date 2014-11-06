#include "egcnaturalnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"


EgcNaturalNodeIterator::EgcNaturalNodeIterator(EgcFormulaExpression& formula) : EgcExpressionNodeIterator(formula)
{
        updateStartNode();
        //m_startNode has now the starting point
        m_cursor = m_startNode;
}

EgcNaturalNodeIterator::~EgcNaturalNodeIterator()
{
}

EgcExpressionNode& EgcNaturalNodeIterator::findNextLeftMostLeaf(EgcExpressionNode& start) const
{
        EgcExpressionNode* tempCursor = &start;
        EgcExpressionNode* tempPointer = nullptr;

        do {
                if (tempCursor->isContainer()) {
                        if (tempCursor->isBinaryExpression()) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else { // check the left child also, maybe this is not null
                                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
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

void EgcNaturalNodeIterator::toBack(void)
{
        m_cursor = &findNextLeftMostLeaf(*m_baseElement);
        m_atBegin = false;
        m_atEnd = true;
}

void EgcNaturalNodeIterator::toFront(void)
{
        m_cursor = &findNextLeftMostLeaf(*m_baseElement);
        m_atBegin = true;
        m_atEnd = false;
}

EgcExpressionNode& EgcNaturalNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool beginning = false;
        bool end = false;

        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;


        if (m_cursor->isContainer()) {
                if (m_cursor->isUnaryExpression()) {
                        tempPointer = m_cursor->getParent();
                        if (tempPointer)
                                tempCursor = tempPointer;
                } else { //must be binary expression                        
                        //traverse right child
                        EgcBinaryExpressionNode *temp = static_cast<EgcBinaryExpressionNode*>(m_cursor);
                        if (temp->getRightChild()) {
                                tempCursor = tempPointer = &findNextLeftMostLeaf(*(temp->getRightChild()));
                        } else {
                                //find the parent of the first left child
                                tempPointer = tempCursor = &findFirstLeftChildParent(*m_cursor);
                        }
                }
        } else { //is a leaf
                tempPointer = tempCursor = &findFirstLeftChildParent(*m_cursor);
        }

        //if the tempPointer is null, set the base element as next element
        if (!tempPointer)
                tempCursor = m_baseElement;

        //jump over to the beginning if already at the end
        if (m_cursor == m_endNode) {
                if (atBeginning)
                        *atBeginning = true;
                tempCursor = m_startNode;
        }

        if (m_cursor == m_endNode) {
                end = true;
        }

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;

        return *tempCursor;

}

EgcExpressionNode& EgcNaturalNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd) const
{

}

void EgcNaturalNodeIterator::updateStartNode(void)
{
        m_startNode = &findNextLeftMostLeaf(*m_baseElement);
        m_endNode = m_baseElement;
}

EgcExpressionNode& EgcNaturalNodeIterator::findFirstLeftChildParent(EgcExpressionNode& startNode) const
{
        EgcExpressionNode *node = &startNode;
        EgcExpressionNode *parent;
        EgcExpressionNode *retval = m_baseElement;


        do {
                parent = node->getParent();
                if (parent) {
                        if (parent->isBinaryExpression()) {
                                if (isLeftChild(*parent, *node)) { //we found the correct node
                                        retval = parent;
                                        break;
                                } else { //search must go on
                                        node = parent;
                                        continue;
                                }
                        } else { // if it is a unary expression, we have found the next node already
                                retval = parent;
                                break;
                        }
                } else {
                        break;
                }
        } while (1);

        return *retval;
}
