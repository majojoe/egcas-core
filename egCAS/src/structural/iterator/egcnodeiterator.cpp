#include <QScopedPointer>
#include "egcnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../specialNodes/egcnode.h"
#include "../specialNodes/egcbinarynode.h"
#include "../specialNodes/egcunarynode.h"
#include "../specialNodes/egcbasenode.h"
#include "../egcnodecreator.h"

EgcNodeIterator::EgcNodeIterator(const EgcFormulaExpression& formula) :
        m_next(formula.getBaseElement().getChild()), m_previous(&formula.getBaseElement()),
        m_baseElement(&formula.getBaseElement()), m_atBegin(true), m_atEnd(false),
        m_state(EgcIteratorState::LeftIteration)
{
}

EgcNodeIterator::EgcNodeIterator(const EgcNode & node)
{        
        EgcNode* tempNode = const_cast<EgcNode*>(&node);
        EgcNode* parent = tempNode->getParent();
        while (parent) {
                tempNode = parent;
                parent = tempNode->getParent();
        };

        m_next = static_cast<EgcBaseNode*>(tempNode)->getChild(0);
        m_previous = tempNode;
        m_baseElement = static_cast<EgcBaseNode*>(tempNode);
        m_atBegin = true;
        m_atEnd = false;
        m_state = EgcIteratorState::LeftIteration;
        EgcNode *nextNode = m_baseElement;

        while (nextNode != &node) {
                *nextNode = next();
        }
}

EgcNodeIterator::~EgcNodeIterator()
{
}

EgcNode & EgcNodeIterator::next(void)
{
        bool errorRestart;

        //if the base element has no child
        if (!m_baseElement->getChild(0)) {
                return *m_baseElement;
        }

        //if we are at the end and do a next
        if (m_atEnd) {
                m_next = m_baseElement->getChild(0);
                m_previous = m_baseElement;
        }

        EgcNode* tempNode = m_next;

        m_state = determineFollowingState(*m_previous, *m_next, true);

        m_next = &getNextElement(*m_next, *m_previous, errorRestart);
        m_previous = tempNode;
        if (errorRestart) {
                m_next = m_baseElement->getChild(0);
                m_previous = m_baseElement;
        }

        if (m_previous == m_baseElement)
                m_atBegin = true;
        else
                m_atBegin = false;

        if (m_next == m_baseElement)
                m_atEnd = true;
        else
                m_atEnd = false;

        return *tempNode;
}

EgcNode & EgcNodeIterator::previous(void)
{
        bool errorRestart;

        //if the base element has no child
        if (!m_baseElement->getChild(0)) {
                return *m_baseElement;
        }

        //if we are at the beginning and do a previous
        if (m_atBegin) {
                m_next = m_baseElement;
                m_previous = m_baseElement->getChild(0);
        }

        EgcNode* tempNode = m_previous;

        m_state = determineFollowingState(*m_previous, *m_next, false);

        m_previous = &getPreviousElement(*m_previous, *m_next, errorRestart);
        m_next = tempNode;
        if (errorRestart) {
                m_next = m_baseElement;
                m_previous = m_baseElement->getChild(0);
        }

        if (m_next == m_baseElement)
                m_atEnd = true;
        else
                m_atEnd = false;

        if (m_previous == m_baseElement)
                m_atBegin = true;
        else
                m_atBegin = false;

        return *tempNode;
}

EgcIteratorState EgcNodeIterator::getLastState(void) const
{
        return m_state;
}

bool EgcNodeIterator::findNext(EgcNodeType type)
{
        EgcNodeIterator *iter = this;
        EgcNode* node;
        bool found = false;

        while (iter->hasNext()) {
                node = &(iter->next());
                if (node->getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        if (!found)
                iter->toBack();

        return found;
}

bool EgcNodeIterator::findPrevious(EgcNodeType type)
{
        EgcNodeIterator *iter = this;
        EgcNode* node;
        bool found = false;

        while (iter->hasPrevious()) {
                node = &(iter->previous());
                if (node->getNodeType() == type) {
                        found = true;
                        break;
                }
        }

        if (!found)
                iter->toFront();

        return found;
}

bool EgcNodeIterator::hasNext(void) const
{
        if (!m_baseElement->getChild(0))
                return false;

        return !m_atEnd;
}

bool EgcNodeIterator::hasPrevious(void) const
{
        if (!m_baseElement->getChild(0))
                return false;

        return !m_atBegin;
}

EgcNode& EgcNodeIterator::peekNext(void) const
{
        if (!m_baseElement->getChild(0))
                return *m_baseElement;

        return *m_next;
}

EgcNode& EgcNodeIterator::peekPrevious(void) const
{
        //if the base element has no child
        if (!m_baseElement->getChild(0))
                return *m_baseElement;

        return *m_previous;
}

void EgcNodeIterator::toBack(void)
{
        m_atBegin = false;
        m_atEnd = true;
        m_next = m_baseElement;
        m_previous = m_baseElement->getChild(0);
}

void EgcNodeIterator::toFront(void)
{
        m_atBegin = true;
        m_atEnd = false;
        m_next = m_baseElement->getChild(0);
        m_previous = m_baseElement;
}

EgcNode& EgcNodeIterator::getNextElement(EgcNode& currentNext, EgcNode& currentPrev, bool& restart) const
{
        EgcNode* next = nullptr;
        restart = false;

        if (currentNext.isContainer()) {
                bool isChild = currentNext.isChild(currentPrev);

                if (isChild) {
                        next = static_cast<EgcContainerNode&>(currentNext).getChild(0);
                }
                if (!isChild || next == nullptr) {
                        next = static_cast<EgcContainerNode&>(currentNext).incrementToNextChild(currentPrev);
                }
                if (next == nullptr || static_cast<EgcContainerNode&>(currentNext).isLastChild(currentPrev)) {
                        next = currentNext.getParent();
                }
        } else {
                next = &currentPrev;
        }

        // in case the tree is defect or if we jump over the end of the tree
        if (next == nullptr) {
                next = m_baseElement->getChild(0);
                restart = true;
        }

        return *next;
}

EgcNode& EgcNodeIterator::getPreviousElement(EgcNode& currentPrev, EgcNode& currentNext, bool& restart) const
{
        EgcNode* prev = nullptr;
        restart = false;

        if (currentPrev.isContainer()) {
                bool isChild = currentPrev.isChild(currentNext);

                if (isChild) {
                        EgcContainerNode* tempContainer = static_cast<EgcContainerNode*>(&currentPrev);
                        prev = tempContainer->getChild(tempContainer->getNumberChildNodes() - 1);
                }
                if (!isChild || prev == nullptr) {
                        prev = static_cast<EgcContainerNode&>(currentPrev).decrementToPrevChild(currentNext);
                }
                if (prev == nullptr || static_cast<EgcContainerNode&>(currentPrev).isFirstChild(currentNext)) {
                        prev = currentPrev.getParent();
                }
        } else {
                prev = &currentNext;
        }

        // in case the tree is defect or if we jump over the end of the tree
        if (prev == nullptr) {
                prev = m_baseElement->getChild(0);
                restart = true;
        }

        return *prev;
}

bool EgcNodeIterator::isRightChild(EgcNode& parent, EgcNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryNode&>(parent).getChild(1) == &child)
                        return true;
        }

        return false;
}

bool EgcNodeIterator::isLeftChild(EgcNode& parent, EgcNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryNode&>(parent).getChild(0) == &child)
                        return true;
        }

        return false;
}

bool EgcNodeIterator::insert(EgcNodeType type)
{
        bool retval = false;

//        QScopedPointer<EgcNode> node (EgcNodeCreator::create(type));

//        if (!node.data())
//                return retval;

//        // only containers can be inserted in the tree
//        if (node->isContainer()) {
//                retval = true;
//                if (node->isBinaryExpression()) {
//                        // set the right child to a empty child
//                        QScopedPointer<EgcNode> tempNode(EgcNodeCreator::
//                                                                  create(EgcNodeType::EmptyNode));
//                        if (m_forward)
//                                static_cast<EgcBinaryNode*>(node.data())->setRightChild(*(tempNode.take()));
//                        else
//                                static_cast<EgcBinaryNode*>(node.data())->setLeftChild(*(tempNode.take()));
//                }
//                //insert the container into the tree
//                EgcNode &nextNode = peekNext();
//                EgcNode &previousNode = peekPrevious();
//                QScopedPointer<EgcNode> childNode;
//                EgcNode *parentNode;
//                EgcContainerNode* container;
//                if (nextNode.getParent() == &previousNode) {
//                        parentNode = &previousNode;
//                        container = static_cast<EgcContainerNode*>(parentNode);
//                        childNode.reset(container->takeOwnership(nextNode));
//                } else {
//                        parentNode = previousNode.getParent();
//                        container = static_cast<EgcContainerNode*>(parentNode);
//                        childNode.reset(container->takeOwnership(previousNode));
//                }
//                if (parentNode == nullptr || parentNode == childNode.data()) {
//                        parentNode = m_baseElement;
//                        container = static_cast<EgcContainerNode*>(parentNode);
//                        if (m_baseElement->getChild())
//                                childNode.reset(container->takeOwnership(*(m_baseElement->getChild())));
//                }

//                //set the parent
//                EgcNode *nodePtr = node.data();
//                if (parentNode->isBinaryExpression()) {
//                        if (isRightChild(*parentNode, *childNode)) {
//                                static_cast<EgcBinaryNode*>(parentNode)->setRightChild(*(node.take()));
//                        } else {
//                                static_cast<EgcBinaryNode*>(parentNode)->setLeftChild(*(node.take()));
//                        }

//                } else {
//                        static_cast<EgcUnaryNode*>(parentNode)->setChild(*(node.take()));
//                }

//                //set the child if one
//                if (childNode.data()) {
//                        if (nodePtr->isBinaryExpression()) {
//                                if (m_forward)
//                                        static_cast<EgcBinaryNode*>(nodePtr)->setLeftChild(*(childNode.take()));
//                                else
//                                        static_cast<EgcBinaryNode*>(nodePtr)->setRightChild(*(childNode.take()));
//                        } else {
//                                static_cast<EgcUnaryNode*>(nodePtr)->setChild(*(childNode.take()));
//                        }
//                } else { // insert empty nodes if no child node is present
//                        if (nodePtr->isBinaryExpression()) {
//                                if (m_forward)
//                                        static_cast<EgcBinaryNode*>(nodePtr)->
//                                                setLeftChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));
//                                else
//                                        static_cast<EgcBinaryNode*>(nodePtr)->
//                                                setRightChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));
//                        } else {
//                                static_cast<EgcUnaryNode*>(nodePtr)->
//                                        setChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));
//                        }

//                }

//                //repair the node pointer organization data
//                m_cursor = nodePtr;
//                m_state = determineFollowingState(*m_history, *m_cursor, m_forward);
//        }

        return retval;
}

void EgcNodeIterator::remove()
{
//        //the last node jumped over is in m_history
//        EgcNode *history = m_history;
//        EgcNode *parent = &nextParent();
//        //jump back again
//        (void) previous();
//        if (parent->isBinaryExpression()) {
//                if (isRightChild(*parent, *history))
//                        static_cast<EgcBinaryNode*>(parent)->
//                                setRightChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));
//                else
//                        static_cast<EgcBinaryNode*>(parent)->
//                                setLeftChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));

//        } else {
//                static_cast<EgcUnaryNode*>(parent)->
//                                setChild(*EgcNodeCreator::create(EgcNodeType::EmptyNode));
//        }
//        m_history = m_baseElement;
//        if (   m_cursor == m_baseElement->getChild()
//            && m_state == EgcIteratorState::RightIteration) {
//                m_atBegin = true;
//                m_atEnd = false;
//        } else {
//                m_atBegin = false;
//                m_atEnd = false;
//        }
}

EgcNode& EgcNodeIterator::nextParent(void)
{
        EgcNode* tempNode;

//        if (   m_history == m_baseElement->getChild()
//            || m_history == m_baseElement)
//                tempNode = m_baseElement->getChild();
//        else
//                tempNode = m_history->getParent();

//        if (tempNode == nullptr) {
//                m_cursor = m_baseElement;
//                m_atBegin = true;
//                m_atEnd = false;
//                m_history = m_baseElement;
//                m_previousCursor = m_baseElement;
//                m_forward = true;
//                m_state = EgcIteratorState::LeftIteration;
//                return *m_baseElement;
//        }

//        m_cursor = tempNode;
//        m_forward = true;
//        m_previousCursor = m_history;
//        m_atBegin = false;
//        if (m_cursor == m_baseElement->getChild())
//                m_atEnd = true;
//        else
//                m_atEnd = false;

//        if (tempNode->isBinaryExpression()) {
//                if (isLeftChild(*tempNode, *m_history)) {
//                        m_state = EgcIteratorState::MiddleIteration;
//                } else {
//                        m_state = EgcIteratorState::RightIteration;
//                }
//        } else { //must be a unary expression
//                m_state = EgcIteratorState::RightIteration;
//        }

//        // we need to increment by one to jump over the element to return
//        (void) next();

        return *tempNode;
}

EgcNode* EgcNodeIterator::replace(EgcNode& node, EgcNodeType type)
{
        EgcNode *retval = nullptr;

//        QScopedPointer<EgcNode> replacement(EgcNodeCreator::create(type));
//        QScopedPointer<EgcNode> theReplaced(&node);

//        if (node.isBinaryExpression() && replacement.data()->isBinaryExpression()) {
//                EgcBinaryNode &bin_node = static_cast<EgcBinaryNode&>(node);
//                if (bin_node.transferPropertiesTo(*static_cast<EgcBinaryNode*>(replacement.data()))) {
//                        retval = replacement.take();
//                } else {
//                        //leave the tree as it is
//                        (void) theReplaced.take();
//                }
//        } else if (node.isUnaryExpression() && replacement.data()->isUnaryExpression()) {
//                EgcUnaryNode &una_node = static_cast<EgcUnaryNode&>(node);
//                if (una_node.transferPropertiesTo(*static_cast<EgcUnaryNode*>(replacement.data()))){
//                        retval = replacement.take();
//                } else {
//                        //leave the tree as it is
//                        (void) theReplaced.take();
//                }
//        } else {
//                EgcNode *parent = node.getParent();

//                if(parent) {
//                        if (parent->isBinaryExpression()) {
//                                if (isLeftChild(*parent, node))
//                                        static_cast<EgcBinaryNode*>(parent)
//                                                ->setLeftChild(*replacement.data());
//                                else
//                                        static_cast<EgcBinaryNode*>(parent)
//                                                ->setRightChild(*replacement.data());
//                        } else {
//                                static_cast<EgcUnaryNode*>(parent)->setChild(*replacement.data());
//                        }
//                        //the replaced node has already been deleted
//                        (void) theReplaced.take();
//                        retval = replacement.take();
//                }
//        }

//        if (retval) {
//                if (m_cursor == &node)
//                        m_cursor = replacement.data();
//                if (m_history == &node)
//                        m_history = replacement.data();
//                if (m_previousCursor == &node)
//                        m_previousCursor = replacement.data();
//        }

        return retval;
}

EgcIteratorState EgcNodeIterator::determineFollowingState(EgcNode &previous, EgcNode &next, bool forward) const
{
        EgcIteratorState localState;

        localState = EgcIteratorState::LeftIteration;
        bool containerCheck;
        if (forward)
                containerCheck = next.isContainer();
        else
                containerCheck = previous.isContainer();
        if (containerCheck) {
                if (forward) {
                        if (next.getParent() == &previous)
                                localState = EgcIteratorState::LeftIteration;
                        else if (static_cast<EgcContainerNode&>(next).isLastChild(previous))
                                localState = EgcIteratorState::RightIteration;
                        else
                                localState = EgcIteratorState::MiddleIteration;
                } else {
                        if (previous.getParent() == &next)
                                localState = EgcIteratorState::RightIteration;
                        else if (static_cast<EgcContainerNode&>(previous).isFirstChild(next))
                                localState = EgcIteratorState::LeftIteration;
                        else
                                localState = EgcIteratorState::MiddleIteration;
                }
        } else {
                localState = EgcIteratorState::MiddleIteration;
        }

        return localState;
}

