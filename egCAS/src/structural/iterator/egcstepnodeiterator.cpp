#include "egcstepnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"

EgcStepNodeIterator::EgcStepNodeIterator(EgcFormulaExpression& formula) : EgcExpressionNodeIterator(formula),
        m_State(EgcStepIteratorState::LeftIteration), m_forward(true), m_previousCursor(m_baseElement)
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
        m_previousCursor = m_cursor;

        if (!m_forward) {
                m_previousCursor = &_getPreviousElement(nullptr, nullptr, &m_State);
                state = m_State = determineFollowingState(*m_previousCursor, *m_cursor, true);
                tempCursor = *m_previousCursor = *m_cursor;
                m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_State);
                m_forward = true;
        } else {
                state = m_State;

                m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_State);
        }

        return tempCursor;

}

EgcExpressionNode & EgcStepNodeIterator::previous(EgcStepIteratorState &state)
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        if (m_forward) {
                state = m_State = determineFollowingState(*m_cursor, *m_previousCursor, false);
                EgcExpressionNode *tempCursor = m_cursor;
                m_cursor = m_previousCursor;
                m_previousCursor = tempCursor;
                m_forward = false;
        } else {
                m_previousCursor = m_cursor;
                m_cursor = &_getPreviousElement(&m_atBegin, nullptr, &m_State);
                state = m_State;
        }

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
                localState = determineFollowingState(*m_cursor, *tempCursor, true);
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
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        EgcExpressionNode* rootElement = m_baseElement->getChild();
        bool beginning = false;
        bool end = false;
        EgcStepIteratorState localState = m_State;
        bool restart_at_end = false;

        //if the base element has no child
        if (!rootElement)
                return *m_baseElement;

        if (   tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        if (localState == EgcStepIteratorState::RightIteration) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else {
                                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                        if (tempPointer) {
                                                tempCursor = tempPointer;
                                        } else {
                                                tempPointer = tempCursor->getParent();
                                                if (tempPointer)
                                                        tempCursor = tempPointer;
                                                else
                                                        restart_at_end = true;
                                        }
                                }
                        } else if (localState == EgcStepIteratorState::MiddleIteration) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else {
                                        tempPointer = tempCursor->getParent();
                                        if (tempPointer)
                                                tempCursor = tempPointer;
                                        else
                                                restart_at_end = true;
                                }
                        } else {
                                tempPointer = tempCursor->getParent();
                                if (tempPointer)
                                        tempCursor = tempPointer;
                                else
                                        restart_at_end = true;
                        }
                } else { // a unary expression
                        if (localState == EgcStepIteratorState::RightIteration) {
                                tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else {
                                        tempPointer = tempCursor->getParent();
                                        if (tempPointer)
                                                tempCursor = tempPointer;
                                        else
                                                restart_at_end = true;
                                }
                        } else {
                                tempPointer = tempCursor->getParent();
                                if (tempPointer)
                                        tempCursor = tempPointer;
                                else
                                        restart_at_end = true;
                        }
                }
        } else { // a leaf
                tempPointer = tempCursor->getParent();
                if (tempPointer)
                        tempCursor = tempPointer;
                else
                        restart_at_end = true;
        }

        if (    restart_at_end
             || tempCursor == m_baseElement) {
                beginning = false;
                tempCursor = tempPointer = rootElement;
                localState = EgcStepIteratorState::RightIteration;
        } else {
                localState = determineFollowingState(*m_cursor, *tempCursor, false);
        }

        //check if this is the end
        if (    (tempCursor == rootElement)
             && (localState == EgcStepIteratorState::LeftIteration)) {
                beginning = true;
        }

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;
        if (state)
                *state = localState;

        return *tempCursor;
}

void EgcStepNodeIterator::toBack(void)
{
        EgcExpressionNodeIterator::toBack();
        m_cursor = m_baseElement;
        m_State = EgcStepIteratorState::RightIteration;
}

void EgcStepNodeIterator::toFront(void)
{
        EgcExpressionNodeIterator::toFront();
        m_State = EgcStepIteratorState::LeftIteration;
}

EgcStepIteratorState EgcStepNodeIterator::determineFollowingState(EgcExpressionNode &current, EgcExpressionNode &following, bool forwardDirection) const
{
        EgcStepIteratorState localState;

        if (forwardDirection) {
                localState = EgcStepIteratorState::LeftIteration;
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
        } else {
                localState = EgcStepIteratorState::RightIteration;
                if (following.isContainer()) {
                        if (following.isBinaryExpression()) {
                                if (isRightChild(following, current))
                                        localState = EgcStepIteratorState::MiddleIteration;
                                else if (isLeftChild(following, current))
                                        localState = EgcStepIteratorState::LeftIteration;
                                else
                                        localState = EgcStepIteratorState::RightIteration;
                        } else { //this must be a unary expression
                                if (following.getParent() == &current)
                                        localState = EgcStepIteratorState::RightIteration;
                                else
                                        localState = EgcStepIteratorState::LeftIteration;
                        }
                } else {
                        localState = EgcStepIteratorState::MiddleIteration;
                }
        }

        return localState;
}
