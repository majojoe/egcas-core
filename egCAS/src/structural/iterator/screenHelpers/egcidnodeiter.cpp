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
#include "concreteNodes/egcbinemptynode.h"
#include "iterator/egcnodeiterator.h"
#include "structural/specialNodes/egcnode_gen.h"

EgcIdNodeIter::EgcIdNodeIter(EgcFormulaEntity& formula) : m_nodeIter{new EgcNodeIterator(formula)},
                                                                m_node{&formula.getBaseElement()},
                                                                m_iterPosAfterUpdate{nullptr},
                                                                m_atRightSideAfterUpdate{false},
                                                                m_isInsert{false},
                                                                m_formula(formula)
{
        toBack();
}

EgcIdNodeIter::~EgcIdNodeIter()
{
}

void EgcIdNodeIter::setAtNode(EgcNode& node, bool atRightSide, bool snapAtOmittedPositions)
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
                visible_node = gotoNodeWithId(true, &iter, node, false, snapAtOmittedPositions);
        else
                visible_node = gotoNodeWithId(false, &iter, node, false, snapAtOmittedPositions);

        if (visible_node) {
                m_node = visible_node;
                *m_nodeIter = iter;
        }
}

bool EgcIdNodeIter::hasNext(void) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* prev;

        prev = gotoNodeWithId(true, &tempIter, *m_node, true);

        if (prev)
                return true;
        return false;
}

bool EgcIdNodeIter::hasPrevious(void) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* next;

        next = gotoNodeWithId(false, &tempIter, *m_node, true);

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
                next = gotoNodeWithId(true, &tempIter, *m_node);
        } else {
                if (m_nodeIter->hasNext()) {
                        m_nodeIter->next();
                        tempIter = *m_nodeIter;
                        next = gotoNodeWithId(true, &tempIter, *m_node);
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
                previous = gotoNodeWithId(false, &tempIter, *m_node);
        } else {
                if (m_nodeIter->hasPrevious()) {
                        m_nodeIter->previous();
                        tempIter = *m_nodeIter;
                        previous = gotoNodeWithId(false, &tempIter, *m_node);
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
        m_node = &m_nodeIter->peekPrevious();
        EgcNodeIterator iter = *m_nodeIter;
        EgcNode* node;

        node = gotoNodeWithId(false, &iter, *m_node);

        if (node) {
                m_node = node;
                *m_nodeIter = iter;
        }
}

void EgcIdNodeIter::toFront(void)
{
        m_nodeIter->toFront();
        m_node = &m_nodeIter->peekNext();
        EgcNodeIterator iter = *m_nodeIter;
        EgcNode* node;

        node = gotoNodeWithId(true, &iter, *m_node);

        if (node) {
                m_node = node;
                *m_nodeIter = iter;
        }
}

EgcNode& EgcIdNodeIter::getNode(void)
{
        return *m_node;
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
                return m_formula.getMathmlMappingCRef().getIdFrame(*node);
        } else {  //EgcIteratorState::MiddleIteration
                QList<quint32> list = m_formula.getMathmlMappingCRef().getIdsNonFrame(*node);
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
                        id = m_formula.getMathmlMappingCRef().getIdFrame(*node);
                }
        }

        return id;
}

EgcNode* EgcIdNodeIter::nextNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, bool snapAtOmittedPositions) const
{
        EgcNode* retval = &currNode;
        EgcNodeIterator iter = *tempIter;
        bool visible;        
        EgcNode* nextNode = &iter.peekNext();
        EgcNode* prevNode = &iter.peekPrevious();

        if (    &currNode != nextNode
             && &currNode != prevNode)
                return nullptr;

        while (!(visible = nodeStateVisible(iter, *retval, snapAtOmittedPositions)) && iter.hasNext()) {
                if (retval == &iter.peekNext())
                        iter.next();
                else
                        retval = &iter.peekNext();
        };

        if (visible) {
                *tempIter = iter;
        } else {
                retval = nullptr;
        }

        return retval;
}

EgcNode* EgcIdNodeIter::prevNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, bool snapAtOmittedPositions) const
{
        EgcNode* retval = &currNode;
        EgcNodeIterator iter = *tempIter;
        bool visible;
        EgcNode* nextNode = &iter.peekNext();
        EgcNode* prevNode = &iter.peekPrevious();

        if (    &currNode != nextNode
             && &currNode != prevNode)
                return nullptr;

        while (!(visible = nodeStateVisible(iter, *retval, snapAtOmittedPositions)) && iter.hasPrevious()) {
                if (retval == &iter.peekPrevious())
                        iter.previous();
                else
                        retval = &iter.peekPrevious();
        };

        if (visible) {
                *tempIter = iter;
        } else {
                retval = nullptr;
        }

        return retval;
}

EgcNode* EgcIdNodeIter::gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, const EgcNode& node, bool checkFollowing, bool snapAtOmittedPositions) const
{
        EgcNode* curr = const_cast<EgcNode*>(&node);
        EgcNode* retval = curr;

        if (!tempIter)
                return nullptr;

        if (!m_node)
                return nullptr;

        if (forward) {
                if (checkFollowing) {
                        if (curr == &tempIter->peekNext() && tempIter->hasNext())
                                retval = &tempIter->next();
                        else
                                retval = &tempIter->peekNext();
                }

                retval = nextNodeWithId(*retval, tempIter, snapAtOmittedPositions);
        } else {
                if (checkFollowing) {
                        if (curr == &tempIter->peekPrevious() && tempIter->hasPrevious())
                                retval = &tempIter->previous();
                        else
                                retval = &tempIter->peekPrevious();
                }
                retval = prevNodeWithId(*retval, tempIter, snapAtOmittedPositions);
        }

        return retval;
}

bool EgcIdNodeIter::nodeStateVisible(const EgcNodeIterator &iter, EgcNode& nodeToTest, bool snapAtOmittedPositions) const
{
        EgcNode* nextNode = &iter.peekNext();
        EgcNode* prevNode = &iter.peekPrevious();
        EgcNode* node;
        EgcNode* otherNode;
        EgcIteratorState state;
        bool checkNext;

        if (    nextNode != &nodeToTest
             && prevNode != &nodeToTest)
                return false;

        if (nextNode == &nodeToTest) {
                node = nextNode;
                otherNode = prevNode;
                state = iter.getStateNextNode();
                checkNext = true;
        } else {
                node = prevNode;
                otherNode = nextNode;
                state = iter.getStatePreviousNode();
                checkNext = false;
        }

        if (!mathmlIdExisting(node, state, otherNode, checkNext))
                return false;
        if (omitFollowingNode(node, state, rightSide(const_cast<EgcNodeIterator&>(iter), *node), snapAtOmittedPositions))
                return false;

        return true;
}

bool EgcIdNodeIter::omitFollowingNode(EgcNode* node, EgcIteratorState stateToTest, bool atRightSide, bool snapAtOmittedPositions) const
{
        bool retval = true;

        if (!node)
                return true;

        //leafes are always considered as valid nodes
        if (!node->isContainer())
                return false;

        //normally this state/side combination is not needed
        if (stateToTest == EgcIteratorState::LeftIteration && atRightSide)
                return true;

        //normally this state/side combination is not needed
        if (stateToTest == EgcIteratorState::RightIteration && !atRightSide)
                return true;

        //while normal stepping several states and nodes must be omitted, but e.g. when marking parents, this is not desired
        if (snapAtOmittedPositions)
                return false;

        switch (node->getNodeType()) {
        case EgcNodeType::DivisionNode:
                if (    stateToTest == EgcIteratorState::LeftIteration
                     || stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::RootNode:
                if (    stateToTest == EgcIteratorState::LeftIteration
                     || stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::ParenthesisNode:
                        retval = false;
                break;
        case EgcNodeType::ExponentNode:
                if (stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::IntegralNode:
                if (    stateToTest == EgcIteratorState::LeftIteration
                     || stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::DifferentialNode:
                if (    stateToTest == EgcIteratorState::LeftIteration
                     || stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::FunctionNode:
                if (    stateToTest == EgcIteratorState::LeftIteration
                     || stateToTest == EgcIteratorState::RightIteration)
                        retval = false;
                break;
        case EgcNodeType::UnaryMinusNode:
                if (    stateToTest == EgcIteratorState::LeftIteration)
                        retval = false;
                break;
        default:
                break;
        }

        return retval;
}

EgcNode& EgcIdNodeIter::getOriginNodeToMark(const EgcNode& node) const
{
        EgcNode* nd = node.getParent();

        if (nd->getNodeType() == EgcNodeType::VariableNode)
                return *nd;

        return const_cast<EgcNode&>(node);
}

bool EgcIdNodeIter::rightSide(void) const
{
        return rightSide(*m_nodeIter, *m_node);
}

bool EgcIdNodeIter::rightSide(EgcNodeIterator& iter, EgcNode& node) const
{
        if (&node == &iter.peekPrevious())
                return true;
        else
                return false;
}

bool EgcIdNodeIter::insert(EgcNodeType type)
{
        EgcNode* node;
        bool right = rightSide();

        if(!m_nodeIter->insert(type))
                return false;
        
        //make the iterators valid again
        //it doesn't matter if we are at the right or left, we must always peek the previous node
        node = &m_nodeIter->peekPrevious();

        m_iterPosAfterUpdate = node;
        m_atRightSideAfterUpdate = right;
        m_isInsert = true;
                
        return true;
}

bool EgcIdNodeIter::remove(bool before)
{
        bool retval = false;
        EgcIteratorState state;
        EgcNode* node;

        node = getNodeToModify(before, state);
        if (!node)
                return false;

        if (!node->getParent())
                return false;

        if (node->isBinaryNode()) {
                retval = removeBinary(before, *node, state);
        } else if (node->isFlexNode()) {
                retval = removeFlex(before, *node, state);
        } else if (node->isUnaryNode()) {
                retval = removeUnary(before, *node, state);
        } else {
                retval = removeLeaf(before, *node, state);
        }

        return retval;
}

bool EgcIdNodeIter::removeBinary(bool before, EgcNode &node, EgcIteratorState state)
{
        bool deleteChild = false;
        bool deleteAll = false;
        bool removeType = false;
        EgcBinaryNode& bin = static_cast<EgcBinaryNode&>(node);
        quint32 index = 0;

        if (!node.getParent())
                return false;

        if (state == EgcIteratorState::LeftIteration || state == EgcIteratorState::RightIteration)
                deleteAll = true;
        else if (state == EgcIteratorState::MiddleIteration && node.getNodeType() != EgcNodeType::BinEmptyNode)
                removeType = true;
        else    //BinEmptyNode
                deleteChild = true;

        //search for the index of the child tree of the nodeToDelete we are currently in
        if (deleteAll) {
                EgcNode* parent = node.getParent();
                QScopedPointer<EgcNode> container(m_formula.cut(node));
                if (!container.isNull())
                        setAtNodeDelayed(*parent, before);
        } else if (removeType) {
                QScopedPointer<EgcBinaryNode> orig(&bin);
                QScopedPointer<EgcBinEmptyNode> empty(new EgcBinEmptyNode);
                if (empty.isNull())
                        return false;
                if (empty->transferProperties(*orig))
                        (void) empty.take();
                else
                        (void) orig.take();
        } else if(deleteChild) {
                if (!m_node)
                        return false;
                if (!bin.getChild(0))
                        return false;
                if (!bin.getChild(1))
                        return false;
                if (before)
                        index = 1;
                else
                        index = 0;

                EgcNode* tmpChild = bin.getChild(index);
                QScopedPointer<EgcNode> child(m_formula.cut(*tmpChild));
                if (child.isNull())
                        return false;
                m_formula.paste(*child.take(), node);

                setAtNodeDelayed(*tmpChild, !before);

        } else {
                return false;
        }

        return true;
}

bool EgcIdNodeIter::removeFlex(bool before, EgcNode &node, EgcIteratorState state)
{
        bool deleteChild = false;
        bool deleteAll = false;
        bool deleteContainer = false;
        quint32 nrChilds = node.nrSubindexes();
        EgcFlexNode& flex = static_cast<EgcFlexNode&>(node);
        quint32 index = 0;

        if (!node.getParent())
                return false;

        if (    (state == EgcIteratorState::LeftIteration || state == EgcIteratorState::RightIteration)
             && nrChilds == 1)
                deleteContainer = true;
        else if (    (state == EgcIteratorState::LeftIteration || state == EgcIteratorState::RightIteration)
                     && nrChilds != 1)
                deleteAll = true;
        else if (state == EgcIteratorState::MiddleIteration && flex.childsDeleteable())
                deleteChild = true;
        else
                return false;

        //search for the index of the child tree of the nodeToDelete we are currently in
        if (deleteAll) {
                EgcNode* parent = node.getParent();
                QScopedPointer<EgcNode> container(m_formula.cut(node));
                if (!container.isNull())
                        setAtNodeDelayed(*parent, before);
        } else if (deleteContainer) {
                if (!flex.getChild(0))
                        return false;
                QScopedPointer<EgcNode> child0(m_formula.cut(*flex.getChild(0)));
                if (child0.isNull())
                        return false;
                m_formula.paste(*child0, node);
                setAtNodeDelayed(*child0, before);
        } else if(deleteChild) {
                if (!m_node)
                        return false;
                if (!flex.hasSubNode(*m_node, index))
                        return false;
                if (!flex.getChild(index))
                        return false;

                flex.remove(index);
                EgcNode* child;
                //correct child indexes
                if (before) {
                        index--;
                        if (index >= nrChilds)
                                index = 0;
                }
                child = flex.getChild(index);
                if (!child)
                        child = &flex;

                setAtNodeDelayed(*child, before);

        } else {
                return false;
        }

        return true;
}

bool EgcIdNodeIter::removeUnary(bool before, EgcNode& node, EgcIteratorState state)
{
        EgcUnaryNode& unary = static_cast<EgcUnaryNode&>(node);

        if (!node.getParent())
                return false;

        EgcNode* tmpChild;
        //correct child indexes
        tmpChild = unary.getChild(0);
        if (!tmpChild)
                return false;

        QScopedPointer<EgcNode> child(m_formula.cut(*unary.getChild(0)));
        if (child.isNull())
                return false;
        m_formula.paste(*child.take(), node);
        setAtNodeDelayed(*tmpChild, before);

        return true;
}

bool EgcIdNodeIter::removeLeaf(bool before, EgcNode& node, EgcIteratorState state)
{
        EgcContainerNode *parent = node.getParent();
        if (!parent)
                return false;

        quint32 index;
        if (!parent->hasSubNode(node, index))
                return false;

        QScopedPointer<EgcNode> leaf(m_formula.cut(node));
        EgcNode* child = parent->getChild(index);
        if (child)
                setAtNodeDelayed(*child, before);

        return true;
}

EgcNode* EgcIdNodeIter::getNodeToModify(bool before, EgcIteratorState &state)
{
        EgcNode* nodeToModify = nullptr;
        EgcNodeIterator iter = *m_nodeIter;

        if (before) {
                if (rightSide())
                        nodeToModify = m_node;
                else
                        nodeToModify = gotoNodeWithId(false, &iter, *m_node, true, true);
        } else {
                if (!rightSide())
                        nodeToModify = m_node;
                else
                        nodeToModify = gotoNodeWithId(true, &iter, *m_node, true, true);
        }

        if (&iter.peekNext() == nodeToModify)
                state = iter.getStateNextNode();
        if (&iter.peekPrevious() == nodeToModify)
                state = iter.getStatePreviousNode();

        return nodeToModify;
}

bool EgcIdNodeIter::deleteTree(bool before)
{
        EgcNode* posAfterUpdate;
        EgcNode* nodeToDelete;
        bool atRightSide;

        if (before) {
                nodeToDelete = &m_nodeIter->peekPrevious();
                atRightSide = true;
        } else {
                nodeToDelete = &m_nodeIter->peekNext();
                atRightSide = false;
        }
        if (nodeToDelete && posAfterUpdate && nodeToDelete->getNodeType() != EgcNodeType::BaseNode) {                
                EgcNode* parent = nodeToDelete->getParent();
                if (!parent)
                        return false;
        
                EgcContainerNode* cParent = static_cast<EgcContainerNode*>(parent);
                quint32 index;
                bool isChild = cParent->getIndexOfChild(*nodeToDelete, index);
                if (!isChild)
                        return false;
                
                setAtNode(*nodeToDelete, true, true);
                m_nodeIter->remove();
                
                setAtNodeDelayed(*cParent->getChild(index), atRightSide);
        }

        return true;
}


bool EgcIdNodeIter::finishModOperation(void)
{
        if (m_iterPosAfterUpdate) {
                setAtNode(*m_iterPosAfterUpdate, m_atRightSideAfterUpdate);
                //correct cursor for remove operations
                if (!m_isInsert && m_iterPosAfterUpdate != m_node) {
                        if (m_atRightSideAfterUpdate)
                                previous();
                        else
                                next();
                }
                m_iterPosAfterUpdate = nullptr;
        }

        if (!m_atRightSideAfterUpdate && m_isInsert) {
                m_isInsert = false;
                return true;
        } else {
                return false;
        }
}

void EgcIdNodeIter::setAtNodeDelayed(EgcNode& node, bool atRightSide)
{
        m_iterPosAfterUpdate = &node;
        m_atRightSideAfterUpdate = atRightSide;
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
