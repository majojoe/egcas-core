/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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


#include "egcidnodeiter.h"
#include "entities/egcformulaentity.h"
#include "specialNodes/egcnode.h"
#include "egcscreenpos.h"
#include "concreteNodes/egcnumbernode.h"
#include "concreteNodes/egcvariablenode.h"
#include "specialNodes/egcflexnode.h"
#include "iterator/egcnodeiterator.h"
#include "structural/specialNodes/egcnode_gen.h"

EgcIdNodeIter::EgcIdNodeIter(const EgcFormulaEntity& formula) : m_nodeIterNext{new EgcNodeIterator(formula)},
                                                                m_nodeIterPrev{new EgcNodeIterator(formula)},
                                                                m_histState{EgcIteratorState::LeftIteration},
                                                                m_lookup(formula.getMathmlMappingCRef()), //gcc bug
                                                                m_histId{0}
{
        toBack();
}

EgcIdNodeIter::~EgcIdNodeIter()
{
}

void EgcIdNodeIter::setAtNode(EgcNode& node, bool atRightSide = true)
{
        EgcNodeIterator iter(node);

        //iter has state EgcIteratorState::LeftIteration or EgcIteratorState::MiddleIteration now
        //iter has jumped over the first the searched element already, but on the left side of the container
        //so we must jump back
        if (iter.hasPrevious())
                (void) iter.previous();

        EgcIteratorState state;

        if (atRightSide) {
                if (node.isContainer())
                        state = EgcIteratorState::RightIteration;
                else
                        state = EgcIteratorState::MiddleIteration;

                //if we want the right side of the node, we need to iterate to the right side
                while(    iter.hasNext() && !(iter.getLastState() == state && &iter.peekPrevious() == &node)) {
                        iter.next();
                };
        }

        *m_nodeIterNext = iter;
        *m_nodeIterPrev = iter;

        if (atRightSide) {
                if (    !mathmlIdExisting(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode(), &m_nodeIterNext->peekPrevious(), nullptr)
                     || omitFollowingNode(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode()))
                        nextNodeWithId();
        } else {
                if (    !mathmlIdExisting(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode(), nullptr, &m_nodeIterPrev->peekNext())
                     || omitFollowingNode(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode()))
                        prevNodeWithId();
        }

        //set histId and histState
        if (atRightSide) {
                if (node.isContainer())
                        m_histState = EgcIteratorState::RightIteration;
                else
                        m_histState = EgcIteratorState::MiddleIteration;
                m_histId = getMathmlId(&node, m_histState, nullptr, &m_nodeIterPrev->peekNext());
        } else {
                if (node.isContainer())
                        m_histState = EgcIteratorState::LeftIteration;
                else
                        m_histState = EgcIteratorState::MiddleIteration;
                m_histId = getMathmlId(&node, m_histState, &m_nodeIterNext->peekPrevious(), nullptr);
        }
}

bool EgcIdNodeIter::hasNext(void) const
{
        return m_nodeIterNext->hasNext();
}

bool EgcIdNodeIter::hasPrevious(void) const
{
        return m_nodeIterPrev->hasPrevious();
}

EgcNode &  EgcIdNodeIter::next(void)
{
        EgcNode* next;
        EgcNodeIterator tempIter = *m_nodeIterNext;

        next = &nextNodeWithId();
        *m_nodeIterPrev = tempIter;
        if (m_nodeIterPrev->hasNext())
                m_nodeIterPrev->next();

        return *next;
}

EgcNode & EgcIdNodeIter::previous(void)
{
        EgcNode* prev;
        EgcNodeIterator tempIter = *m_nodeIterPrev;

        prev = &prevNodeWithId();
        *m_nodeIterNext = tempIter;
        if (m_nodeIterNext->hasPrevious())
                m_nodeIterNext->previous();

        return *prev;
}

EgcNode& EgcIdNodeIter::peekNext(void) const
{
        return m_nodeIterNext->peekNext();
}

EgcNode& EgcIdNodeIter::peekPrevious(void) const
{
        return m_nodeIterPrev->peekPrevious();
}

void EgcIdNodeIter::toBack(void)
{
        m_nodeIterNext->toBack();
        m_nodeIterPrev->toBack();
        if (    !mathmlIdExisting(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode(), nullptr, &m_nodeIterPrev->peekNext())
             || omitFollowingNode(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode()))
                prevNodeWithId();
        if (m_nodeIterPrev->peekPrevious().isContainer())
                m_histState = EgcIteratorState::RightIteration;
        else
                m_histState = EgcIteratorState::MiddleIteration;
        m_histId = getMathmlId(&m_nodeIterPrev->peekPrevious(), m_histState, nullptr, &m_nodeIterPrev->peekNext());
}

void EgcIdNodeIter::toFront(void)
{
        m_nodeIterNext->toFront();
        m_nodeIterPrev->toFront();
        if (    !mathmlIdExisting(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode(), &m_nodeIterNext->peekPrevious(), nullptr)
             || omitFollowingNode(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode()))
                nextNodeWithId();
        if (m_nodeIterNext->peekNext().isContainer())
                m_histState = EgcIteratorState::LeftIteration;
        else
                m_histState = EgcIteratorState::MiddleIteration;
        m_histId = getMathmlId(&m_nodeIterNext->peekNext(), m_histState, &m_nodeIterNext->peekPrevious(), nullptr);
}

const quint32& EgcIdNodeIter::id(void)
{
        return m_histId;
}

EgcIteratorState EgcIdNodeIter::getLastState(void) const
{
        return m_histState;
}

bool EgcIdNodeIter::mathmlIdExisting(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const
{
        if (getMathmlId(node, state, previousNode, nextNode))
                return true;
        else
                return false;
}

quint32 EgcIdNodeIter::getMathmlId(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const
{
        quint32 id = 0;
        
        if (!node)
                return id;
        
        if (    state == EgcIteratorState::LeftIteration
             || state == EgcIteratorState::RightIteration) {
                return m_lookup.getIdFrame(*node);
        } else {  //EgcIteratorState::MiddleIteration
                QList<quint32> list = m_lookup.getIdsNonFrame(*node);
                quint32 childIndex = 0;

                if (node->isContainer()) {
                        if (previousNode) {
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousNode, childIndex))
                                        childIndex = 0;
                        } else if (nextNode) {
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*nextNode, childIndex)) {
                                        childIndex = 0;
                                } else {
                                        if (childIndex)
                                                childIndex--;
                                }
                        }
                        if (list.size() > childIndex) {
                                id = list.at(childIndex);
                        }
                } else { // is not a container, so it has no further frames
                        id = m_lookup.getIdFrame(*node);
                }
        }

        return id;
}

EgcNode& EgcIdNodeIter::prevNodeWithId(void)
{
        EgcNode* prevNode;
        EgcNode* retval;
        EgcNode* node = nullptr;
        EgcNode* next;
        bool firstRun = true;
        bool rollover;

        do {
                rollover = false;
                if (firstRun)
                        next = &m_nodeIterPrev->peekNext();
                if (m_nodeIterPrev->hasPrevious())
                        node = &m_nodeIterPrev->previous();
                if (firstRun) {
                        retval = node;
                        m_histState = m_nodeIterPrev->getLastState();
                        m_histId = getMathmlId(node, m_histState, nullptr, next);
                        firstRun = false;
                }
                prevNode = &m_nodeIterPrev->peekPrevious();

                //if a child is not visible, omit that middle iteration before the invisible child
                if (retval == prevNode && m_histState == EgcIteratorState::MiddleIteration)
                        firstRun = true;
                if (retval == prevNode && m_nodeIterPrev->getLastState() == EgcIteratorState::MiddleIteration)
                        rollover = true;

        } while (    (   !mathmlIdExisting(prevNode, m_nodeIterPrev->getStatePreviousNode(), nullptr, node)
                      || omitFollowingNode(prevNode, m_nodeIterPrev->getStatePreviousNode())
                      || firstRun
                      || rollover)
                  && m_nodeIterPrev->hasPrevious());

        if (!retval)
                retval = &m_nodeIterPrev->peekPrevious();

        return *retval;
}

EgcNode& EgcIdNodeIter::nextNodeWithId(void)
{
        EgcNode* nextNode;
        EgcNode* retval;
        EgcNode* node = nullptr;
        EgcNode* prev;
        bool firstRun = true;
        bool rollover;

        do {
                rollover = false;
                if (firstRun)
                        prev = &m_nodeIterNext->peekPrevious();
                if (m_nodeIterNext->hasNext())
                        node = &m_nodeIterNext->next();
                if (firstRun) {
                        retval = node;
                        m_histState = m_nodeIterNext->getLastState();
                        m_histId = getMathmlId(node, m_histState, prev, nullptr);
                        firstRun = false;
                }
                nextNode = &m_nodeIterNext->peekNext();

                //if a child is not visible, omit that middle iteration before the invisible child
                if (retval == nextNode && m_histState == EgcIteratorState::MiddleIteration)
                        firstRun = true;
                if (retval == nextNode && m_nodeIterNext->getLastState() == EgcIteratorState::MiddleIteration)
                        rollover = true;

        } while (    (    !mathmlIdExisting(nextNode, m_nodeIterNext->getStateNextNode(), node, nullptr)
                       || omitFollowingNode(nextNode, m_nodeIterNext->getStateNextNode())
                       || firstRun
                       || rollover )
                  && m_nodeIterNext->hasNext());

        if (!retval)
                retval = &m_nodeIterNext->peekNext();

        return *retval;
}

bool EgcIdNodeIter::omitFollowingNode(EgcNode* followingNode, EgcIteratorState followingState)
{
        bool retval = true;

        if (!followingNode)
                return true;

        //leafes are always considered as valid nodes
        if (!followingNode->isContainer())
                return false;

        switch (followingNode->getNodeType()) {
        case EgcNodeType::DivisionNode:
                if (    followingState == EgcIteratorState::LeftIteration
                     || followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::RootNode:
                if (    followingState == EgcIteratorState::LeftIteration
                     || followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::ParenthesisNode:
                        retval = false;
                break;
        case EgcNodeType::ExponentNode:
                if (followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::IntegralNode:
                if (    followingState == EgcIteratorState::LeftIteration
                     || followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::DifferentialNode:
                if (    followingState == EgcIteratorState::LeftIteration
                     || followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::FunctionNode:
                if (    followingState == EgcIteratorState::LeftIteration
                     || followingState == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::UnaryMinusNode:
                if (    followingState == EgcIteratorState::LeftIteration)
                        retval = false;
                break;
        default:
                break;
        }

        return retval;
}

quint32 EgcIdNodeIter::peekNextId(void) const
{
        return getMathmlId(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode(), &m_nodeIterNext->peekPrevious(), nullptr);
}
quint32 EgcIdNodeIter::peekPreviousId(void) const
{
        return getMathmlId(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode(), nullptr, &m_nodeIterPrev->peekNext());
}

EgcNode& EgcIdNodeIter::getOriginNodeToMark(const EgcNode& node)
{
        EgcNode* nd = node.getParent();

        if (nd->getNodeType() == EgcNodeType::VariableNode)
                return *nd;

        return const_cast<EgcNode&>(node);
}

EgcIteratorState EgcIdNodeIter::getStateNextNode(void) const
{
        return m_nodeIterNext->getStateNextNode();
}

EgcIteratorState EgcIdNodeIter::getStatePreviousNode(void) const
{
        return m_nodeIterPrev->getStatePreviousNode();
}

bool EgcIdNodeIter::insert(EgcNodeType type)
{
        bool retval;

        retval = m_nodeIterNext->insert(type);
        setAtNode(m_nodeIterNext->peekPrevious(), true);

        return retval;
}

//check if the given node is a result node (activate this if insert and remove have been defined)
//bool EgcIdNodeIter::isResultNode(EgcNode& node)
//{
//        //if the current node is a result, the result shall not be able to be changed
//        if (node.getParent()->getNodeType() == EgcNodeType::EqualNode) {
//                quint32 index;
//                (void) node.getParent()->getIndexOfChild(node, index);
//                if (index == 1)
//                        return true;
//        }
//
//        return false;
//}
