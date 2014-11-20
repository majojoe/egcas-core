#include "egcstepnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"

EgcStepNodeIterator::EgcStepNodeIterator(EgcFormulaExpression& formula) : EgcExpressionNodeIterator(formula),
        m_State(EgcStepIteratorState::LeftIteration)
{        
}

EgcStepNodeIterator::~EgcStepNodeIterator()
{

}

EgcExpressionNode & EgcStepNodeIterator::next(EgcStepIteratorState &state)
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        EgcExpressionNode& tempCursor = *m_cursor;
        state = m_State;

        m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_State);

        return tempCursor;

}

EgcExpressionNode & EgcStepNodeIterator::previous(EgcStepIteratorState &state)
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        m_cursor = &_getPreviousElement(&m_atBegin, nullptr, &m_State);

        return *m_cursor;
}

EgcExpressionNode& EgcStepNodeIterator::next(void)
{
        return *m_baseElement;
}

EgcExpressionNode& EgcStepNodeIterator::previous(void)
{
        return *m_baseElement;
}

EgcExpressionNode& EgcStepNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
{        
        return _getNextElement(atBeginning, atEnd, nullptr);
}

EgcExpressionNode& EgcStepNodeIterator::_getNextElement(bool* atBeginning, bool* atEnd, EgcStepIteratorState* state) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        EgcExpressionNode* rootElement = m_baseElement->getChild();
        bool beginning = false;
        bool end = false;
        EgcStepIteratorState localState = m_State;
        bool restart_at_begin = false;


        //if the base element has no child
        if (!rootElement)
                return *m_baseElement;

        if (   tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        if (localState == EgcStepIteratorState::LeftIteration) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;                                        
                                } else {
                                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                        if (tempPointer) {
                                                tempCursor = tempPointer;
                                        } else {
                                                tempPointer = tempCursor->getParent();
                                                if (tempPointer)
                                                        tempCursor = tempPointer;
                                                else
                                                        restart_at_begin = true;
                                        }
                                }
                        } else if (localState == EgcStepIteratorState::MiddleIteration) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else {
                                        tempPointer = tempCursor->getParent();
                                        if (tempPointer)
                                                tempCursor = tempPointer;
                                        else
                                                restart_at_begin = true;
                                }
                        } else {
                                tempPointer = tempCursor->getParent();
                                if (tempPointer)
                                        tempCursor = tempPointer;
                                else
                                        restart_at_begin = true;
                        }
                } else { // a unary expression
                        if (localState == EgcStepIteratorState::LeftIteration) {
                                tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else {
                                        tempPointer = tempCursor->getParent();
                                        if (tempPointer)
                                                tempCursor = tempPointer;
                                        else
                                                restart_at_begin = true;
                                }
                        } else {
                                tempPointer = tempCursor->getParent();
                                if (tempPointer)
                                        tempCursor = tempPointer;
                                else
                                        restart_at_begin = true;
                        }
                }
        } else { // a leaf
                tempPointer = tempCursor->getParent();
                if (tempPointer)
                        tempCursor = tempPointer;
                else
                        restart_at_begin = true;
        }

        if (    restart_at_begin
             || tempCursor == m_baseElement) {
                end = true;
                tempCursor = tempPointer = rootElement;
                localState = EgcStepIteratorState::LeftIteration;
        } else {
                localState = determineFollowingState(*m_cursor, *tempCursor);
        }

        //check if this is the end
        if (tempCursor == m_baseElement) {
                end = true;
        }

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

EgcExpressionNode& EgcStepNodeIterator::_getPreviousElement(bool* atBeginning, bool* atEnd, EgcStepIteratorState* state) const
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
        m_State = EgcStepIteratorState::RightIteration;
}

void EgcStepNodeIterator::toFront(void)
{
        EgcExpressionNodeIterator::toFront();
        m_State = EgcStepIteratorState::LeftIteration;
}

EgcStepIteratorState EgcStepNodeIterator::determineFollowingState(EgcExpressionNode &current, EgcExpressionNode &following) const
{
        EgcStepIteratorState localState = EgcStepIteratorState::LeftIteration;

        if (following.isContainer()) {
                if (following.isBinaryExpression()) {
                        if (isLeftChild(following, current))
                                localState = EgcStepIteratorState::MiddleIteration;
                        else if (isRightChild(following, current))
                                localState = EgcStepIteratorState::RightIteration;
                        else
                                localState = EgcStepIteratorState::LeftIteration;
                } else { //this must be a unary expression
                        if (following.getParent() == &current)
                                localState = EgcStepIteratorState::LeftIteration;
                        else
                                localState = EgcStepIteratorState::RightIteration;
                }
        } else {
                localState = EgcStepIteratorState::MiddleIteration;
        }

        return localState;
}
