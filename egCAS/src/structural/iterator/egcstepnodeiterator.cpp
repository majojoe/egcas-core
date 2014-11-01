#include "egcstepnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"

EgcStepNodeIterator::EgcStepNodeIterator(EgcFormulaExpression& formula) : EgcExpressionNodeIterator(formula),
        m_previousCursor{nullptr, nullptr}, m_internalState(internalIteratorState::gotoLeft)
{        
}

EgcStepNodeIterator::~EgcStepNodeIterator()
{

}

EgcStepIteratorState EgcStepNodeIterator::getIterationState(void)
{
        EgcStepIteratorState iteratorState = EgcStepIteratorState::LeftIteration;

        if (m_previousCursor[0] || m_previousCursor[1]) {
                if ( isLeftChild(*(m_previousCursor[0]),  *(m_previousCursor[1])) )
                        iteratorState = EgcStepIteratorState::MiddleIteration;
                else if( isRightChild(*(m_previousCursor[0]),  *(m_previousCursor[1])) )
                        iteratorState = EgcStepIteratorState::RightIteration;
        }

        //all leaf type traversals are middle iterations
        if (m_previousCursor[0]) {
                if (!m_previousCursor[0]->isContainer())
                        iteratorState = EgcStepIteratorState::MiddleIteration;
        }

        return iteratorState;
}

EgcExpressionNode& EgcStepNodeIterator::next(void)
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        m_previousCursor[1] = m_previousCursor[0];
        m_previousCursor[0] = m_cursor;

        if (m_internalState == internalIteratorState::gotoStart) {
                toFront();                        
        } else if (    m_previousCursor[0] == m_baseElement->getChild()) {
                if (m_previousCursor[1] == getSecondToLast()) {
                        m_internalState = internalIteratorState::gotoStart;
                        m_atEnd = true;
                }
        }

        EgcExpressionNode& tempCursor = *m_cursor;

        m_cursor = &_getNextElement(nullptr, nullptr, &m_internalState);

        return tempCursor;
}

EgcExpressionNode& EgcStepNodeIterator::previous(void)
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        m_previousCursor[1] = m_cursor;
        m_cursor = &_getPreviousElement(&m_atBegin, nullptr, &m_internalState);
        m_previousCursor[0] = m_cursor;

        return *m_cursor;
}

EgcExpressionNode& EgcStepNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
{        
        return _getNextElement(atBeginning, atEnd, nullptr);
}

EgcExpressionNode& EgcStepNodeIterator::_getNextElement(bool* atBeginning, bool* atEnd, internalIteratorState* state) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool searchUpwards = true;
        bool beginning = false;
        bool end = false;
        internalIteratorState localState = m_internalState;

        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        if (   tempCursor->isContainer()
            && (localState == internalIteratorState::gotoLeft || localState == internalIteratorState::gotoRight)) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        if (localState == internalIteratorState::gotoRight) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                        } else {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                        }
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        }

                        if (!tempPointer && localState == internalIteratorState::gotoLeft) {
                                //the left child is null, maybe the right child is not null
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

        if (!searchUpwards) {
                localState = internalIteratorState::gotoLeft;
        } else {
                //search for a binary containter whose node has not been visited yet,
                //since only that one can have another unvisited node.
                EgcExpressionNode* parent = tempCursor->getParent();
                if (parent) {
                        if (parent->getNodeType() != EgcExpressionNodeType::BaseNode) {
                                if (isLeftChild(*parent, *tempCursor))
                                        localState = internalIteratorState::gotoRight;
                                else
                                        localState = internalIteratorState::gotoParent;

                                tempCursor = parent;
                        } else { //this is the base element so return the root element
                                tempCursor = m_baseElement->getChild();
                        }
                } else { // if the parent pointer is null, this is the base element or the node is invalid
                        //return the root element since the rest of the tree does not really exist or it is
                        //already the root element.
                        tempCursor = m_baseElement->getChild();
                }
        }
        if (tempCursor == m_baseElement->getChild())
                end = true;

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;        
        if (state)
                *state = localState;

        return *tempCursor;
}

EgcExpressionNode& EgcStepNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd) const
{
        return _getPreviousElement(atBeginning, atEnd, nullptr);
}

EgcExpressionNode& EgcStepNodeIterator::_getPreviousElement(bool* atBeginning, bool* atEnd, internalIteratorState* state) const
{
        (void) atBeginning;
        (void) atEnd;
        (void) state;
        *atBeginning = true;
        *atEnd = false;

        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        return *m_baseElement->getChild();
}

void EgcStepNodeIterator::toBack(void)
{
        EgcExpressionNodeIterator::toBack();
        m_previousCursor[0] = nullptr;
        m_previousCursor[1] = nullptr;
        m_internalState = internalIteratorState::gotoRight;
}

void EgcStepNodeIterator::toFront(void)
{
        EgcExpressionNodeIterator::toFront();
        m_previousCursor[0] = nullptr;
        m_previousCursor[1] = nullptr;
        m_internalState = internalIteratorState::gotoLeft;
}

EgcExpressionNode* EgcStepNodeIterator::getSecondToLast(void)
{
        EgcExpressionNode* retval = nullptr;

        EgcExpressionNode *rootNode = m_baseElement->getChild();
        //if the base element has no child
        if (!rootNode)
                return m_baseElement;

        if (rootNode->isContainer()) {
                if (rootNode->isBinaryExpression()) {
                        retval = static_cast<EgcBinaryExpressionNode*>(rootNode)->getRightChild();
                        if (!retval) {
                                retval = static_cast<EgcBinaryExpressionNode*>(rootNode)->getLeftChild();
                        }
                } else {
                        retval = static_cast<EgcUnaryExpressionNode*>(rootNode)->getChild();
                }
        }

        return retval;
}
