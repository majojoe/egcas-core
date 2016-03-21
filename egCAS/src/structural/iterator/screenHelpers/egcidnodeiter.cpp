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


EgcIdNodeIter::EgcIdNodeIter(const EgcFormulaEntity& formula) : m_nodeIter{new EgcNodeIterator(formula)},                                                                
                                                                m_lookup(formula.getMathmlMappingCRef()), //gcc bug
                                                                m_node{nullptr}
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

        EgcNode* visible_node;

        if (atRightSide)
                visible_node = gotoNodeWithId(true, &iter);
         else
                visible_node = gotoNodeWithId(false, &iter);

        if (visible_node) {
                m_node = visible_node;
                *m_nodeIter = iter;
        }
}

bool EgcIdNodeIter::hasNext(void) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* prev;

        prev = gotoNodeWithId(true, &tempIter, true);

        if (prev)
                return true;
        return false;
}

bool EgcIdNodeIter::hasPrevious(void) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* next;

        next = gotoNodeWithId(false, &tempIter, true);

        if (next)
                return true;
        return false;
}

EgcNode &  EgcIdNodeIter::next(void)
{
        EgcNode* old;
        EgcNode* next;
        EgcNodeIterator tempIter = *m_nodeIter;

        old = m_node;
        if (m_node == &m_nodeIter->peekPrevious()) {
                m_node = &m_nodeIter->peekNext();
                next = gotoNodeWithId(true, &tempIter);
        } else {
                if (m_nodeIter->hasNext()) {
                        m_nodeIter->next();
                        next = gotoNodeWithId(true, &tempIter);
                }
        }

        if (next) {
                m_node = next;
                *m_nodeIter = tempIter;
        } else {
                m_node = old;
        }

        return *m_node;
}

EgcNode & EgcIdNodeIter::previous(void)
{
        EgcNode* old;
        EgcNode* previous;
        EgcNodeIterator tempIter = *m_nodeIter;

        old = m_node;
        if (m_node == &m_nodeIter->peekNext()) {
                m_node = &m_nodeIter->peekPrevious();
                previous = gotoNodeWithId(false, &tempIter);
        } else {
                if (m_nodeIter->hasPrevious()) {
                        m_nodeIter->previous();
                        previous = gotoNodeWithId(false, &tempIter);
                }
        }

        if (previous) {
                m_node = previous;
                *m_nodeIter = tempIter;
        } else {
                m_node = old;
        }

        return *m_node;
}

void EgcIdNodeIter::toBack(void)
{
        m_nodeIter->toBack();
        EgcNodeIterator iter = *m_nodeIter;
        EgcNode* node;

        node = gotoNodeWithId(false, &iter);

        if (node) {
                m_node = node;
                *m_nodeIter = iter;
        }
}

void EgcIdNodeIter::toFront(void)
{
        m_nodeIter->toFront();
        EgcNodeIterator iter = *m_nodeIter;
        EgcNode* node;

        node = gotoNodeWithId(true, &iter);

        if (node) {
                m_node = node;
                *m_nodeIter = iter;
        }
}

quint32 EgcIdNodeIter::id(void) const
{
        if (m_node == &m_nodeIter->peekNext())
                return getMathmlId(m_node, getState(), &m_nodeIter->peekPrevious(), nullptr);
        else
                return getMathmlId(m_node, getState(), nullptr, &m_nodeIter->peekNext());
}

EgcIteratorState EgcIdNodeIter::getState(void) const
{
        if (&m_nodeIter->peekNext() == m_node)
                return m_nodeIter->getStateNextNode();
        else
                return m_nodeIter->getStatePreviousNode();
}

bool EgcIdNodeIter::mathmlIdExisting(EgcNode* node, EgcIteratorState state, EgcNode* otherNode, bool nextNode) const
{
        if (nextNode) {
                if (getMathmlId(node, state, nullptr, otherNode))
                        return true;
        } else {
                if (getMathmlId(node, state, otherNode, nullptr))
                        return true;
        }

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

EgcNode* EgcIdNodeIter::nextNodeWithId(bool testForNextNode, EgcNodeIterator* tempIter) const
{
        EgcNode* retval;
        EgcNodeIterator iter = *tempIter;
        bool visible;

        while (!(visible = nodeStateVisible(iter, testForNextNode)) && m_nodeIter->hasNext()) {
                if (testForNextNode) {
                        iter.next();
                        testForNextNode = false;
                } else {
                        testForNextNode = true;
                }
        };

        if (visible) {
                *tempIter = iter;
                if (testForNextNode) {
                        retval = &iter.peekNext();
                } else {
                        retval = &iter.peekPrevious();
                }
        } else {
                retval = nullptr;
        }

        return retval;
}

EgcNode* EgcIdNodeIter::prevNodeWithId(bool testForPrevNode, EgcNodeIterator* tempIter) const
{
        EgcNode* retval;
        EgcNodeIterator iter = *tempIter;
        bool visible;

        while (!(visible = nodeStateVisible(iter, testForPrevNode)) && m_nodeIter->hasPrevious()) {
                if (testForPrevNode) {
                        iter.previous();
                        testForPrevNode = false;
                } else {
                        testForPrevNode = true;
                }
        };

        if (visible) {
                *tempIter = iter;
                if (testForPrevNode) {
                        retval = &iter.peekPrevious();
                } else {
                        retval = &iter.peekNext();
                }
        } else {
                retval = nullptr;
        }

        return retval;
}

EgcNode* EgcIdNodeIter::gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, bool checkFollowing) const
{
        EgcNode* retval = nullptr;
        EgcNode* curr = m_node;
        bool test = false;

        if (!tempIter)
                return nullptr;

        *tempIter = *m_nodeIter;

        if (forward) {
                if (curr == &m_nodeIter->peekNext())
                        test = true;
                if (checkFollowing) {
                        if (test && m_nodeIter->hasNext())
                                (void) m_nodeIter->next();
                        else
                                test = true;
                }

                retval = nextNodeWithId(test, tempIter);
        } else {
                if (curr == &m_nodeIter->peekPrevious())
                        test = true;
                if (checkFollowing) {
                        if (test && m_nodeIter->hasPrevious())
                                (void) m_nodeIter->previous();
                        else
                                test = true;
                }
                retval = prevNodeWithId(test, tempIter);
        }

        return retval;
}

bool EgcIdNodeIter::nodeStateVisible(const EgcNodeIterator &iter, bool forwardDirection) const
{
        EgcNode* node;
        EgcNode* otherNode;
        EgcIteratorState state;

        if (forwardDirection) {
                node = &iter.peekNext();
                otherNode = &iter.peekPrevious();
                state = iter.getStateNextNode();
        } else {
                node = &iter.peekPrevious();
                otherNode = &iter.peekNext();
                state = iter.getStatePreviousNode();
        }

        if (!mathmlIdExisting(node, state, otherNode, forwardDirection))
                return false;
        if (omitFollowingNode(node, state))
                return false;

        return true;
}

bool EgcIdNodeIter::omitFollowingNode(EgcNode* followingNode, EgcIteratorState followingState) const
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

EgcNode& EgcIdNodeIter::getOriginNodeToMark(const EgcNode& node)
{
        EgcNode* nd = node.getParent();

        if (nd->getNodeType() == EgcNodeType::VariableNode)
                return *nd;

        return const_cast<EgcNode&>(node);
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
