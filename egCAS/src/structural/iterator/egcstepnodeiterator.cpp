#include "egcstepnodeiterator.h"

EgcStepNodeIterator::EgcStepNodeIterator(EgcFormulaExpression& formula) : EgcExpressionNodeIterator(formula)
{
}

EgcStepNodeIterator::~EgcStepNodeIterator()
{

}

EgcExpressionNode& EgcStepNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
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

EgcExpressionNode& EgcStepNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd) const
{

}
