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
        m_baseElement(&formula.getBaseElement()), m_state(EgcIteratorState::LeftIteration), m_history(nullptr)
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
        if (m_next == m_baseElement) {
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
        m_history = tempNode;

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
        if (m_previous == m_baseElement) {
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
        m_history = tempNode;

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

        if (m_next == m_baseElement)
                return false;
        else
                return true;
}

bool EgcNodeIterator::hasPrevious(void) const
{
        if (!m_baseElement->getChild(0))
                return false;

        if (m_previous == m_baseElement)
                return false;
        else
                return true;
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
        m_next = m_baseElement;
        m_previous = m_baseElement->getChild(0);
}

void EgcNodeIterator::toFront(void)
{
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

        QScopedPointer<EgcNode> node (EgcNodeCreator::create(type));

        if (!node.data())
                return retval;

        // only containers can be inserted in the tree
        if (node->isContainer()) {
                EgcContainerNode* node_cont = static_cast<EgcContainerNode*>(node.data());
                quint32 nodeIndex = 0;
                quint32 nrChildNodes = node_cont->getNumberChildNodes();
                bool forward = true;
                if (m_history == m_next)
                        forward = false;

                if (nrChildNodes > 1) {
                        quint32 n;
                        quint32 start;

                        if (forward) {
                                start = 1;
                                n = nrChildNodes;
                                nodeIndex = 0;
                        } else {
                                start = 0;
                                n = nrChildNodes - 1;
                                nodeIndex = nrChildNodes - 1;
                        }

                        for (int i = start; i < n; i++) {
                                QScopedPointer<EgcNode> tempNode(EgcNodeCreator::
                                                                          create(EgcNodeType::EmptyNode));
                                if (tempNode.data() == nullptr)
                                        return false;

                                node_cont->setChild(i, *(tempNode.take()));
                        }
                }

                //insert the container into the tree
                if (!m_next || !m_previous)
                        return false;

                QScopedPointer<EgcNode> childNode;
                EgcNode* childNodePtr;
                EgcContainerNode *parentNode;
                quint32 parentIndex;
                bool indexOk;

                if (m_next->getParent() == m_previous) {
                        parentNode = static_cast<EgcContainerNode*>(m_previous);
                        childNodePtr = m_next;
                } else {
                        parentNode = static_cast<EgcContainerNode*>(m_previous->getParent());
                        childNodePtr = m_previous;
                }
                indexOk = parentNode->getIndexChild(*childNodePtr, parentIndex);
                if (parentNode)
                        childNode.reset(parentNode->takeOwnership(*childNodePtr));

                if (!parentNode || parentNode == childNode.data() || !indexOk)
                        return false;

                //set the parent
                EgcContainerNode *nodePtr = static_cast<EgcContainerNode*>(node.data());

                if (parentNode->setChild(parentIndex, *(node.take())))
                        retval = true;

                //set the child if one
                if (childNode.data()) {
                        if (!nodePtr->setChild(nodeIndex, *(childNode.take())))
                                retval = false;
                } else { // insert empty nodes if no child node is present
                        if (!nodePtr->setChild(nodeIndex, *EgcNodeCreator::create(EgcNodeType::EmptyNode)))
                                retval = false;
                }

                //repair the node pointer organization data
                m_previous = m_history = nodePtr;
        }

        return retval;
}

void EgcNodeIterator::remove()
{
        //the last node jumped over is in m_history
        EgcNode *history = m_history;
        if (!history) //if there is no history, we are going to delete the root node
                history = m_baseElement->getChild(0);
        EgcNode *parent = history->getParent();
        if (!parent) //nullptr is not allowed
                return;
        quint32 index = 0;
        m_next = parent;
        m_previous = history;
        m_history = nullptr;
        (void) next();

        if (parent->isContainer()) { //must be true, only for case of error
                if (static_cast<EgcContainerNode*>(parent)->getIndexChild(*history, index)) {
                        static_cast<EgcContainerNode*>(parent)->
                                                       setChild(index,*EgcNodeCreator::create(EgcNodeType::EmptyNode));
                }
        }
        //jump back again
        (void) previous();
        //correct the history
        if (parent->isContainer()) //must be true, only for case of error
                m_history = static_cast<EgcContainerNode*>(parent)->getChild(index);
}

EgcNode* EgcNodeIterator::replace(EgcNode& node, EgcNodeType type)
{
        EgcNode *retval = nullptr;

        QScopedPointer<EgcNode> replacement(EgcNodeCreator::create(type));
        QScopedPointer<EgcNode> theReplaced(&node);

        if (!replacement.data())
                return nullptr;

        if (node.isContainer() && replacement.data()->isContainer()) {
                //both are container types and have the same number of childs
                EgcContainerNode* node_cont = static_cast<EgcContainerNode*>(&node);
                EgcContainerNode* replace_cont = static_cast<EgcContainerNode*>(replacement.data());

                if (node_cont->getNumberChildNodes() == replace_cont->getNumberChildNodes()) {
                        // the number of child nodes is equal, so we can transfer the childs
                        if (node_cont->transferPropertiesTo(*replace_cont))
                                retval = replacement.take();
                }

        } else if (!node.isContainer() && !replacement.data()->isContainer()) { //both are leafes (no container types)
                EgcNode *parent = node.getParent();

                if(parent) {
                        if (parent->isContainer()) {
                                EgcContainerNode* parentCont = static_cast<EgcContainerNode*>(parent);
                                quint32 index;

                                if (parentCont->getIndexChild(node, index)) {
                                        parentCont->setChild(index, *replacement.data());
                                        //the replaced node has already been deleted
                                        (void) theReplaced.take();
                                        retval = replacement.take();
                                }
                        }
                }

        }

        if (retval) { //all other cases
                if (m_next == &node)
                        m_next = replacement.data();
                if (m_history == &node)
                        m_history = replacement.data();
                if (m_previous == &node)
                        m_previous = replacement.data();
        } else {
                //leave the tree as it is
                (void) theReplaced.take();
        }

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

