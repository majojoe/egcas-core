#include <QScopedPointer>
#include "egcstepnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"
#include "../egcexpressionnodecreator.h"

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
        if (!m_baseElement->getChild()) {
                m_history = m_baseElement;
                return *m_baseElement;
        }

        EgcExpressionNode& tempCursor = *m_cursor;
        m_previousCursor = m_cursor;

        if (!m_forward) {
                m_previousCursor = &_getPreviousElement(nullptr, nullptr, &m_State);
                state = m_State = determineFollowingState(*m_previousCursor, *m_cursor, true);
                tempCursor = *m_previousCursor = *m_cursor;
                m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_State);
                m_atBegin = false;
                m_forward = true;
        } else {
                state = m_State;

                m_cursor = &_getNextElement(nullptr, &m_atEnd, &m_State);
        }

        m_history = &tempCursor;

        return tempCursor;

}

EgcExpressionNode & EgcStepNodeIterator::previous(EgcStepIteratorState &state)
{
        //if the base element has no child
        if (!m_baseElement->getChild()) {
                m_history = m_baseElement;
                return *m_baseElement;
        }

        if (m_forward) {
                state = m_State = determineFollowingState(*m_cursor, *m_previousCursor, false);
                EgcExpressionNode *tempCursor = m_cursor;
                m_cursor = m_previousCursor;
                m_previousCursor = tempCursor;                
                if (m_atBegin) { //if we are already at the beginning: we are at the end then
                        m_atEnd = true;
                        m_atBegin = false;
                } else {
                        m_atEnd = false;
                        if (m_cursor->getParent() == m_baseElement)
                                m_atBegin = true;
                }
                m_forward = false;
        } else {
                m_previousCursor = m_cursor;
                m_cursor = &_getPreviousElement(&m_atBegin, nullptr, &m_State);
                state = m_State;
        }

        m_history = m_cursor;

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

        //check if this is the beginning
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
        m_forward = false;
        m_history = m_baseElement;
}

void EgcStepNodeIterator::toFront(void)
{
        EgcExpressionNodeIterator::toFront();
        m_State = EgcStepIteratorState::LeftIteration;
        m_history = m_baseElement;
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


bool EgcStepNodeIterator::insert(EgcExpressionNodeType type)
{
        bool retval = false;

        QScopedPointer<EgcExpressionNode> node (EgcExpressionNodeCreator::create(type));

        // only containers can be inserted in the tree
        if (node->isContainer()) {
                retval = true;
                if (node->isBinaryExpression()) {
                        // set the right child to a empty child
                        QScopedPointer<EgcExpressionNode> tempNode(EgcExpressionNodeCreator::
                                                                  create(EgcExpressionNodeType::EmptyNode));
                        static_cast<EgcBinaryExpressionNode*>(node.data())->setRightChild(*(tempNode.take()));
                }
                //insert the container into the tree
                EgcExpressionNode &nextNode = peekNext();
                EgcExpressionNode &previousNode = peekPrevious();
                QScopedPointer<EgcExpressionNode> childNode;
                EgcExpressionNode *parentNode;
                if (nextNode.getParent() == &previousNode) {
                        childNode.reset(&nextNode);
                        parentNode = &previousNode;
                } else {
                        parentNode = previousNode.getParent();
                        childNode.reset(&previousNode);
                }
                if (parentNode == nullptr || parentNode == childNode.data()) {
                        parentNode = m_baseElement;
                }

                //set the parent
                if (parentNode->isBinaryExpression()) {
                        if (isRightChild(*parentNode, *childNode)) {
                                static_cast<EgcBinaryExpressionNode*>(parentNode)->setRightChild(*(node.take()));
                        } else {
                                static_cast<EgcBinaryExpressionNode*>(parentNode)->setLeftChild(*(node.take()));
                        }

                } else {
                        static_cast<EgcUnaryExpressionNode*>(parentNode)->setChild(*(node.take()));
                }

                //set the child if one
                if (childNode.data()) {
                        if (node->isBinaryExpression()) {
                                if (m_forward)
                                        static_cast<EgcBinaryExpressionNode*>(node.data())->setLeftChild(*(childNode.take()));
                                else
                                        static_cast<EgcBinaryExpressionNode*>(node.data())->setRightChild(*(childNode.take()));
                        } else {
                                static_cast<EgcUnaryExpressionNode*>(node.data())->setChild(*(childNode.take()));
                        }
                } else { // insert empty nodes if no child node is present
                        if (node->isBinaryExpression()) {
                                if (m_forward)
                                        static_cast<EgcBinaryExpressionNode*>(node.data())->
                                                setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                                else
                                        static_cast<EgcBinaryExpressionNode*>(node.data())->
                                                setRightChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                        } else {
                                static_cast<EgcUnaryExpressionNode*>(node.data())->
                                        setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                        }

                }

        }

        //repair the node pointer organization data
        m_cursor = node.data();

        return retval;
}

void EgcStepNodeIterator::remove()
{
        //the last node jumped over is in m_history
        EgcExpressionNode *parent = &nextParent();
        if (parent->isBinaryExpression()) {
                if (isRightChild(*parent, *m_history))
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setRightChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                else
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));

        } else {
                static_cast<EgcUnaryExpressionNode*>(parent)->
                                setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
        }
        delete(m_history);
        m_history = m_baseElement;
        if (m_cursor == m_baseElement->getChild()) {
                m_atBegin = true;
                m_atEnd = false;
        } else {
                m_atBegin = false;
                m_atEnd = false;
        }
}

EgcExpressionNode& EgcStepNodeIterator::nextParent(void)
{
        EgcExpressionNode* tempNode;

        if (   m_history == m_baseElement->getChild()
            || m_history == m_baseElement)
                tempNode = m_baseElement->getChild();
        else
                tempNode = m_history->getParent();

        if (tempNode == nullptr) {
                m_cursor = m_baseElement;
                m_atBegin = true;
                m_atEnd = false;
                m_history = m_baseElement;
                m_previousCursor = m_baseElement;
                m_forward = true;
                m_State = EgcStepIteratorState::LeftIteration;
                return *m_baseElement;
        }

        m_cursor = tempNode;
        m_forward = true;
        m_previousCursor = m_history;
        m_atBegin = false;
        if (m_cursor == m_baseElement->getChild())
                m_atEnd = true;
        else
                m_atEnd = false;

        if (tempNode->isBinaryExpression()) {
                if (isLeftChild(*tempNode, *m_history)) {
                        m_State = EgcStepIteratorState::MiddleIteration;
                } else {
                        m_State = EgcStepIteratorState::RightIteration;
                }
        } else { //must be a unary expression
                m_State = EgcStepIteratorState::RightIteration;
        }

        // we need to increment by one to jump over the element to return
        EgcStepIteratorState tempState;
        (void) next(tempState);

        return *tempNode;
}

