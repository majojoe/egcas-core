#include <QScopedPointer>
#include "egcexpressionnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"
#include "../egcexpressionnodecreator.h"

EgcExpressionNodeIterator::EgcExpressionNodeIterator(const EgcFormulaExpression& formula) :
        m_cursor(formula.getBaseElement().getChild()), m_baseElement(&formula.getBaseElement()),
        m_atBegin(true), m_atEnd(false), m_history(m_baseElement), m_State(EgcNodeIteratorState::LeftIteration),
        m_forward(true), m_previousCursor(m_baseElement)
{
}

EgcExpressionNodeIterator::EgcExpressionNodeIterator(const EgcExpressionNode & node)
{        
        EgcExpressionNode* tempNode = const_cast<EgcExpressionNode*>(&node);
        EgcExpressionNode* parent = tempNode->getParent();
        while (parent) {
                tempNode = parent;
                parent = tempNode->getParent();
        };

        m_cursor = const_cast<EgcExpressionNode*>(&node);
        m_baseElement = static_cast<EgcBaseExpressionNode*>(tempNode);
        m_history = m_baseElement;
        m_State = EgcNodeIteratorState::LeftIteration;
        m_atBegin = true;
        m_atEnd = false;
        m_forward = true;
        m_previousCursor = m_baseElement;
        EgcExpressionNode *nextNode = m_baseElement;
        EgcNodeIteratorState state;

        while (nextNode != &node) {
                *nextNode = next(state);
        }
}

EgcExpressionNodeIterator::~EgcExpressionNodeIterator()
{
}

EgcExpressionNode & EgcExpressionNodeIterator::next(EgcNodeIteratorState &state)
{
        //if the base element has no child
        if (!m_baseElement->getChild()) {
                m_history = m_baseElement;
                return *m_baseElement;
        }

        EgcExpressionNode* tempCursor = m_cursor;
        m_previousCursor = m_cursor;

        if (!m_forward) {
                m_previousCursor = &getPreviousElement(nullptr, nullptr, &m_State);
                state = m_State = determineFollowingState(*m_previousCursor, *m_cursor, true);
                tempCursor = m_previousCursor = m_cursor;
                m_cursor = &getNextElement(nullptr, &m_atEnd, &m_State);
                m_atBegin = false;
                m_forward = true;
        } else {
                state = m_State;
                m_cursor = &getNextElement(nullptr, &m_atEnd, &m_State);
                if (m_cursor != tempCursor)
                        m_atBegin = false;
        }

        m_history = tempCursor;

        return *tempCursor;

}

EgcExpressionNode & EgcExpressionNodeIterator::previous(EgcNodeIteratorState &state)
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
                m_cursor = &getPreviousElement(&m_atBegin, nullptr, &m_State);
                state = m_State;
                if (m_cursor != m_history)
                        m_atEnd = false;
        }

        m_history = m_cursor;

        return *m_cursor;
}

bool EgcExpressionNodeIterator::findNext(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator *iter = this;
        EgcExpressionNode* node;
        bool found = false;
        EgcNodeIteratorState state;

        while (iter->hasNext()) {
                node = &(iter->next(state));
                if (node->getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        if (!found)
                iter->toBack();

        return found;
}

bool EgcExpressionNodeIterator::findPrevious(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator *iter = this;
        EgcExpressionNode* node;
        bool found = false;
        EgcNodeIteratorState state;

        while (iter->hasPrevious()) {
                node = &(iter->previous(state));
                if (node->getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        if (!found)
                iter->toFront();

        return found;
}

bool EgcExpressionNodeIterator::hasNext(void) const
{
        if (!m_baseElement->getChild())
                return false;

        return !m_atEnd;
}

bool EgcExpressionNodeIterator::hasPrevious(void) const
{
        if (!m_baseElement->getChild())
                return false;

        return !m_atBegin;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekNext(void) const
{
        if (!m_baseElement->getChild())
                return *m_baseElement;

        return *m_cursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekPrevious(void) const
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        return getPreviousElement(nullptr, nullptr, nullptr);
}

void EgcExpressionNodeIterator::toBack(void)
{
        m_atBegin = false;
        m_atEnd = true;
        m_cursor = m_baseElement;
        m_State = EgcNodeIteratorState::RightIteration;
        m_forward = false;
        m_history = m_baseElement;
}

void EgcExpressionNodeIterator::toFront(void)
{
        m_cursor = m_baseElement->getChild();
        m_atBegin = true;
        m_atEnd = false;
        m_State = EgcNodeIteratorState::LeftIteration;
        m_history = m_baseElement;
        m_forward = true;
}

EgcExpressionNode& EgcExpressionNodeIterator::getNextElement(bool* atBeginning, bool* atEnd, EgcNodeIteratorState* state) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        EgcExpressionNode* rootElement = m_baseElement->getChild();
        bool beginning = false;
        bool end = false;
        EgcNodeIteratorState localState = m_State;
        bool restart_at_begin = false;

        //if the base element has no child
        if (!rootElement)
                return *m_baseElement;

        if (   tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        if (localState == EgcNodeIteratorState::LeftIteration) {
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
                        } else if (localState == EgcNodeIteratorState::MiddleIteration) {
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
                        if (localState == EgcNodeIteratorState::LeftIteration) {
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
                localState = EgcNodeIteratorState::LeftIteration;
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

EgcExpressionNode& EgcExpressionNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd, EgcNodeIteratorState* state) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        EgcExpressionNode* rootElement = m_baseElement->getChild();
        bool beginning = false;
        bool end = false;
        EgcNodeIteratorState localState = m_State;
        bool restart_at_end = false;

        //if the base element has no child
        if (!rootElement)
                return *m_baseElement;

        if (   tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        if (localState == EgcNodeIteratorState::RightIteration) {
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
                        } else if (localState == EgcNodeIteratorState::MiddleIteration) {
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
                        if (localState == EgcNodeIteratorState::RightIteration) {
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
                localState = EgcNodeIteratorState::RightIteration;
        } else {
                localState = determineFollowingState(*m_cursor, *tempCursor, false);
        }

        //check if this is the beginning
        if (    (tempCursor == rootElement)
             && (   ((localState == EgcNodeIteratorState::LeftIteration) && (rootElement->isContainer()) )
                 || ((localState == EgcNodeIteratorState::MiddleIteration) && !(rootElement->isContainer()) ) ) )
        {
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

bool EgcExpressionNodeIterator::insert(EgcExpressionNodeType type)
{
        bool retval = false;

        QScopedPointer<EgcExpressionNode> node (EgcExpressionNodeCreator::create(type));

        if (!node.data())
                return retval;

        // only containers can be inserted in the tree
        if (node->isContainer()) {
                retval = true;
                if (node->isBinaryExpression()) {
                        // set the right child to a empty child
                        QScopedPointer<EgcExpressionNode> tempNode(EgcExpressionNodeCreator::
                                                                  create(EgcExpressionNodeType::EmptyNode));
                        if (m_forward)
                                static_cast<EgcBinaryExpressionNode*>(node.data())->setRightChild(*(tempNode.take()));
                        else
                                static_cast<EgcBinaryExpressionNode*>(node.data())->setLeftChild(*(tempNode.take()));
                }
                //insert the container into the tree
                EgcExpressionNode &nextNode = peekNext();
                EgcExpressionNode &previousNode = peekPrevious();
                QScopedPointer<EgcExpressionNode> childNode;
                EgcExpressionNode *parentNode;
                EgcContainerNode* container;
                if (nextNode.getParent() == &previousNode) {
                        parentNode = &previousNode;
                        container = static_cast<EgcContainerNode*>(parentNode);
                        childNode.reset(container->takeOwnership(nextNode));
                } else {
                        parentNode = previousNode.getParent();
                        container = static_cast<EgcContainerNode*>(parentNode);
                        childNode.reset(container->takeOwnership(previousNode));
                }
                if (parentNode == nullptr || parentNode == childNode.data()) {
                        parentNode = m_baseElement;
                        container = static_cast<EgcContainerNode*>(parentNode);
                        if (m_baseElement->getChild())
                                childNode.reset(container->takeOwnership(*(m_baseElement->getChild())));
                }

                //set the parent
                EgcExpressionNode *nodePtr = node.data();
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
                        if (nodePtr->isBinaryExpression()) {
                                if (m_forward)
                                        static_cast<EgcBinaryExpressionNode*>(nodePtr)->setLeftChild(*(childNode.take()));
                                else
                                        static_cast<EgcBinaryExpressionNode*>(nodePtr)->setRightChild(*(childNode.take()));
                        } else {
                                static_cast<EgcUnaryExpressionNode*>(nodePtr)->setChild(*(childNode.take()));
                        }
                } else { // insert empty nodes if no child node is present
                        if (nodePtr->isBinaryExpression()) {
                                if (m_forward)
                                        static_cast<EgcBinaryExpressionNode*>(nodePtr)->
                                                setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                                else
                                        static_cast<EgcBinaryExpressionNode*>(nodePtr)->
                                                setRightChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                        } else {
                                static_cast<EgcUnaryExpressionNode*>(nodePtr)->
                                        setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                        }

                }

                //repair the node pointer organization data
                m_cursor = nodePtr;
                m_State = determineFollowingState(*m_history, *m_cursor, m_forward);
        }

        return retval;
}

void EgcExpressionNodeIterator::remove()
{
        //the last node jumped over is in m_history
        EgcExpressionNode *history = m_history;
        EgcExpressionNode *parent = &nextParent();
        //jump back again
        EgcNodeIteratorState state;
        (void) previous(state);
        if (parent->isBinaryExpression()) {
                if (isRightChild(*parent, *history))
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setRightChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                else
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));

        } else {
                static_cast<EgcUnaryExpressionNode*>(parent)->
                                setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
        }        
        m_history = m_baseElement;
        if (   m_cursor == m_baseElement->getChild()
            && m_State == EgcNodeIteratorState::RightIteration) {
                m_atBegin = true;
                m_atEnd = false;
        } else {
                m_atBegin = false;
                m_atEnd = false;
        }
}

EgcExpressionNode& EgcExpressionNodeIterator::nextParent(void)
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
                m_State = EgcNodeIteratorState::LeftIteration;
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
                        m_State = EgcNodeIteratorState::MiddleIteration;
                } else {
                        m_State = EgcNodeIteratorState::RightIteration;
                }
        } else { //must be a unary expression
                m_State = EgcNodeIteratorState::RightIteration;
        }

        // we need to increment by one to jump over the element to return
        EgcNodeIteratorState tempState;
        (void) next(tempState);

        return *tempNode;
}

EgcExpressionNode* EgcExpressionNodeIterator::replace(EgcExpressionNode& node, EgcExpressionNodeType type)
{
        EgcExpressionNode *retval = nullptr;

        QScopedPointer<EgcExpressionNode> replacement(EgcExpressionNodeCreator::create(type));
        QScopedPointer<EgcExpressionNode> theReplaced(&node);

        if (node.isBinaryExpression() && replacement.data()->isBinaryExpression()) {
                EgcBinaryExpressionNode &bin_node = static_cast<EgcBinaryExpressionNode&>(node);
                if (bin_node.transferPropertiesTo(*static_cast<EgcBinaryExpressionNode*>(replacement.data()))) {
                        retval = replacement.take();
                } else {
                        //leave the tree as it is
                        (void) theReplaced.take();
                }
        } else if (node.isUnaryExpression() && replacement.data()->isUnaryExpression()) {
                EgcUnaryExpressionNode &una_node = static_cast<EgcUnaryExpressionNode&>(node);
                if (una_node.transferPropertiesTo(*static_cast<EgcUnaryExpressionNode*>(replacement.data()))){
                        retval = replacement.take();
                } else {
                        //leave the tree as it is
                        (void) theReplaced.take();
                }
        } else {
                EgcExpressionNode *parent = node.getParent();

                if(parent) {
                        if (parent->isBinaryExpression()) {
                                if (isLeftChild(*parent, node))
                                        static_cast<EgcBinaryExpressionNode*>(parent)
                                                ->setLeftChild(*replacement.data());
                                else
                                        static_cast<EgcBinaryExpressionNode*>(parent)
                                                ->setRightChild(*replacement.data());
                        } else {
                                static_cast<EgcUnaryExpressionNode*>(parent)->setChild(*replacement.data());
                        }
                        //the replaced node has already been deleted
                        (void) theReplaced.take();
                        retval = replacement.take();
                }
        }

        if (retval) {
                if (m_cursor == &node)
                        m_cursor = replacement.data();
                if (m_history == &node)
                        m_history = replacement.data();                
                if (m_previousCursor == &node)
                        m_previousCursor = replacement.data();
        }

        return retval;
}

EgcNodeIteratorState EgcExpressionNodeIterator::determineFollowingState(EgcExpressionNode &current,
                                                                        EgcExpressionNode &following,
                                                                        bool forwardDirection) const
{
        EgcNodeIteratorState localState;

        if (forwardDirection) {
                localState = EgcNodeIteratorState::LeftIteration;
                if (following.isContainer()) {
                        if (following.isBinaryExpression()) {
                                if (isLeftChild(following, current))
                                        localState = EgcNodeIteratorState::MiddleIteration;
                                else if (isRightChild(following, current))
                                        localState = EgcNodeIteratorState::RightIteration;
                                else
                                        localState = EgcNodeIteratorState::LeftIteration;
                        } else { //this must be a unary expression
                                if (following.getParent() == &current)
                                        localState = EgcNodeIteratorState::LeftIteration;
                                else
                                        localState = EgcNodeIteratorState::RightIteration;
                        }
                } else {
                        localState = EgcNodeIteratorState::MiddleIteration;
                }
        } else {
                localState = EgcNodeIteratorState::RightIteration;
                if (following.isContainer()) {
                        if (following.isBinaryExpression()) {
                                if (isRightChild(following, current))
                                        localState = EgcNodeIteratorState::MiddleIteration;
                                else if (isLeftChild(following, current))
                                        localState = EgcNodeIteratorState::LeftIteration;
                                else
                                        localState = EgcNodeIteratorState::RightIteration;
                        } else { //this must be a unary expression
                                if (following.getParent() == &current)
                                        localState = EgcNodeIteratorState::RightIteration;
                                else
                                        localState = EgcNodeIteratorState::LeftIteration;
                        }
                } else {
                        localState = EgcNodeIteratorState::MiddleIteration;
                }
        }

        return localState;
}

