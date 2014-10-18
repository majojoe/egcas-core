#include "egcstepnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"

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
        m_previousCursor[1] = m_previousCursor[0];
        m_previousCursor[0] = m_cursor;
        EgcExpressionNode& tempCursor = *m_cursor;
        m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_internalState);

        return tempCursor;
}

EgcExpressionNode& EgcStepNodeIterator::previous(void)
{
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
                        if (isLeftChild(*parent, *tempCursor))
                                localState = internalIteratorState::gotoRight;
                        else
                                localState = internalIteratorState::gotoParent;

                        tempCursor = parent;
                } else { // if the parent pointer is null, this is the root element or the node is invalid
                        //return the root element since the rest of the tree does not really exist or it is
                        //already the root element.
                        tempCursor = m_rootElement;
                }
        }
        if (tempCursor == m_rootElement)
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

        return *m_rootElement;
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
