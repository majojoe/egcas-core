/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include <QScopedPointer>
#include "egcnodeiterator.h"
#include "../entities/egcformulaentity.h"
#include "../specialNodes/egcnode.h"
#include "../specialNodes/egcbinarynode.h"
#include "../specialNodes/egcunarynode.h"
#include "../specialNodes/egcbasenode.h"
#include "../specialNodes/egcflexnode.h"
#include "../egcnodecreator.h"

EgcNodeIterator::EgcNodeIterator(const EgcFormulaEntity& formula) :
        m_next(formula.getBaseElement().getChild(0)), m_previous(&formula.getBaseElement()),
        m_baseElement(&formula.getBaseElement()), m_state(EgcIteratorState::LeftIteration)
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

        while (   nextNode != &node
                & hasNext()) {
                nextNode = &next();
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

EgcNode* EgcNodeIterator::replaceBinEmptyNodeBy(EgcNodeType type)
{
        EgcNode* nodeToReplace = nullptr;

        if (m_previous->getNodeType() == EgcNodeType::BinEmptyNode)
                nodeToReplace = m_previous;
        if (m_next->getNodeType() == EgcNodeType::BinEmptyNode)
                nodeToReplace = m_next;

        if (nodeToReplace == nullptr)
                return nullptr;

        //check if new type is binary node
        QScopedPointer<EgcNode> node (EgcNodeCreator::create(type));
        if (!node.data())
                return nullptr;
        if (!node->isBinaryNode())
                return nullptr;

        return replace(*nodeToReplace, type);
}

EgcNode* EgcNodeIterator::insert(EgcNodeType type, bool insertBeforeChild)
{
        EgcNode* retval = nullptr;

        QScopedPointer<EgcNode> node (EgcNodeCreator::create(type));

        if (!node.data())
                return retval;

        // only containers can be inserted in the tree
        if (node->isContainer()) {
                EgcContainerNode* node_cont = static_cast<EgcContainerNode*>(node.data());
                quint32 nodeIndex = 0;
                quint32 nrChildNodes = node_cont->getNumberChildNodes();

                if (nrChildNodes > 1) {
                        quint32 n;
                        quint32 start;

                        if (!insertBeforeChild) {
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
                                        return nullptr;
                                node_cont->setChild(i, *(tempNode.take()));
                        }
                }

                //insert the container into the tree
                if (!m_next || !m_previous)
                        return retval;

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
                indexOk = parentNode->getIndexOfChild(*childNodePtr, parentIndex);
                if (parentNode)
                        childNode.reset(parentNode->takeOwnership(*childNodePtr));

                if (!parentNode || parentNode == childNode.data() || !indexOk)
                        return nullptr;

                //set the parent
                EgcContainerNode *nodePtr = static_cast<EgcContainerNode*>(node.data());

                if (parentNode->setChild(parentIndex, *(node.take())))
                        retval = nodePtr;

                //set the child if one
                if (childNode.data()) {
                        if (!nodePtr->setChild(nodeIndex, *(childNode.take())))
                                retval = nullptr;
                } else { // insert empty nodes if no child node is present
                        if (!nodePtr->setChild(nodeIndex, *EgcNodeCreator::create(EgcNodeType::EmptyNode)))
                                retval = nullptr;
                }

                //repair the node pointer organization data
                m_previous = nodePtr;
        }

        return retval;
}

void EgcNodeIterator::remove(bool deleteNext)
{
        EgcNode *toDelete;
        if (deleteNext)
                toDelete = m_next;
        else
                toDelete = m_previous;
        if (!toDelete) //if there is no history, we are going to delete the root node
                toDelete = m_baseElement->getChild(0);
        EgcContainerNode *parent = toDelete->getParent();
        if (!parent) //nullptr is not allowed
                return;
        quint32 index = 0;

        if (parent->isContainer()) { //must be true, only for case of error
                if (static_cast<EgcContainerNode*>(parent)->getIndexOfChild(*toDelete, index)) {
                        if (parent->isFlexNode() && (static_cast<EgcFlexNode*>(parent)->getNumberChildNodes() > 1))
                                static_cast<EgcFlexNode*>(parent)->remove(index);
                        else
                                static_cast<EgcContainerNode*>(parent)->
                                                       setChild(index,*EgcNodeCreator::create(EgcNodeType::EmptyNode));
                }
        }
        if (deleteNext) {
                m_next = parent->getChild(index);
                m_previous = parent;
        } else {
                m_next = parent;
                m_previous = parent->getChild(index);
        }

}

EgcNode* EgcNodeIterator::replace(EgcNode& node, EgcNodeType type)
{
        EgcNode *retval = nullptr;
        bool allOk = false;

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
                        if (replace_cont->transferProperties(*node_cont)) {
                                if (replacement.data())
                                        allOk = true;
                        }
                }

        } else if (!node.isContainer()) { //both are leafes (no container types)
                EgcNode *parent = node.getParent();

                if(parent) {
                        if (parent->isContainer()) {
                                EgcContainerNode* parentCont = static_cast<EgcContainerNode*>(parent);
                                quint32 index;

                                if (parentCont->getIndexOfChild(node, index)) {
                                        if (parentCont->setChild(index, *replacement.data())) {
                                                //the replaced node has already been deleted
                                                (void) theReplaced.take();
                                                if (replacement.data())
                                                        allOk = true;
                                        }
                                }
                        }
                }

                if (allOk && replacement.data()->isContainer()) { // allocate empty nodes for the childs
                        EgcContainerNode* replContainer = static_cast<EgcContainerNode*>(replacement.data());
                        quint32 nrChilds = replContainer->getNumberChildNodes();
                        for (int i = 0; i < nrChilds; i++) {
                                QScopedPointer<EgcNode> tempNode(EgcNodeCreator::
                                                                          create(EgcNodeType::EmptyNode));
                                if (tempNode.data() == nullptr)
                                        allOk = false;
                                replContainer->setChild(i, *(tempNode.take()));
                        }
                }
        }


        if (allOk)
                retval = replacement.take();

        if (retval) { //all other cases
                if (m_next == &node)
                        m_next = retval;
                if (m_previous == &node)
                        m_previous = retval;
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

EgcIteratorState EgcNodeIterator::getStateNextNode(void) const
{
        EgcNode* next = m_next;
        EgcNode* previous = m_previous;

        //if the base element has no child
        if (!m_baseElement->getChild(0)) {
                return EgcIteratorState::LeftIteration;
        }

        //if we are at the end and do a next
        if (m_next == m_baseElement) {
                next = m_baseElement->getChild(0);
                previous = m_baseElement;
        }

        if (previous == nullptr || next == nullptr)
                return EgcIteratorState::LeftIteration;

        return determineFollowingState(*previous, *next, true);
}

EgcIteratorState EgcNodeIterator::getStatePreviousNode(void) const
{
        EgcNode* next = m_next;
        EgcNode* previous = m_previous;

        //if the base element has no child
        if (!m_baseElement->getChild(0)) {
                return EgcIteratorState::RightIteration;
        }

        //if we are at the beginning and do a previous
        if (m_previous == m_baseElement) {
                next = m_baseElement;
                previous = m_baseElement->getChild(0);
        }

        if (previous == nullptr || next == nullptr)
                return EgcIteratorState::RightIteration;

        return determineFollowingState(*previous, *next, false);
}


bool EgcNodeIterator::insertChildSpace(EgcNodeType type)
{
        EgcFlexNode* node = nullptr;
        EgcNode* child = nullptr;
        bool forward;
        quint32 index;

        if (m_next->isFlexNode() && m_next->getNodeType() == type) {
                node = static_cast<EgcFlexNode*>(m_next);
                child = m_previous;
                forward = true;
        } else  if (m_previous->isFlexNode() && m_previous->getNodeType() == type) {
                node = static_cast<EgcFlexNode*>(m_previous);
                child = m_next;
                forward = false;
        }

        if (!node || !child)
                return false;

        if (!node->getIndexOfChild(*child, index))
                return false;

        if (forward) {
                QScopedPointer<EgcNode> tempNode(EgcNodeCreator::create(EgcNodeType::EmptyNode));
                index++;
                node->insert(index, *tempNode.take());
        } else {
                QScopedPointer<EgcNode> tempNode(EgcNodeCreator::create(EgcNodeType::EmptyNode));
                EgcNode* ptr = tempNode.data();
                node->insert(index, *tempNode.take());
                m_previous = ptr;
        }

        return true;
}

void EgcNodeIterator::invalidate(void)
{
        m_next = m_baseElement;
        m_previous = m_baseElement;
}
