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
#include "specialNodes/egcflexnode.h"
#include "concreteNodes/egcbinemptynode.h"
#include "iterator/egcnodeiterator.h"
#include "structural/specialNodes/egcnode_gen.h"
#include "structural/egcnodecreator.h"

EgcIdNodeIter::EgcIdNodeIter(EgcFormulaEntity& formula) : m_nodeIter{new EgcNodeIterator(formula)},
                                                                m_node{&formula.getBaseElement()},
                                                                m_iterPosAfterUpdate{nullptr},
                                                                m_atRightSideAfterUpdate{false},
                                                                m_isInsert{false},
                                                                m_formula(formula),
                                                                m_lockDelayedUpdate(false)
{
        toBack();
}

EgcIdNodeIter::~EgcIdNodeIter()
{
}

EgcIdNodeIter::EgcIdNodeIter(const EgcIdNodeIter& orig) : m_nodeIter{new EgcNodeIterator(*orig.m_nodeIter)},
        m_node{orig.m_node},
        m_iterPosAfterUpdate{orig.m_iterPosAfterUpdate},
        m_atRightSideAfterUpdate{orig.m_atRightSideAfterUpdate},
        m_isInsert{orig.m_isInsert},
        m_formula(orig.m_formula),
        m_lockDelayedUpdate{orig.m_lockDelayedUpdate}
{

}

void EgcIdNodeIter::setAtNode(EgcNode& node, bool atRightSide, EgcSnapProperty snapProperty)
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

        if (!m_node)
                m_node = &node;

        if (atRightSide) {
                visible_node = gotoNodeWithId(false, &iter, node, false, snapProperty);
        } else {
                visible_node = gotoNodeWithId(true, &iter, node, false, snapProperty);
        }

        if (visible_node) {
                m_node = visible_node;
                *m_nodeIter = iter;
        }
}

bool EgcIdNodeIter::hasNext(EgcSnapProperty snapProperty) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* prev;

        prev = gotoNodeWithId(true, &tempIter, *m_node, true, snapProperty);

        if (prev)
                return true;
        return false;
}

bool EgcIdNodeIter::hasNext(void) const
{
        return hasNext(EgcSnapProperty::SnapVisibleCursor);
}

bool EgcIdNodeIter::hasPrevious(EgcSnapProperty snapProperty) const
{
        EgcNodeIterator tempIter = *m_nodeIter;
        EgcNode* next;

        next = gotoNodeWithId(false, &tempIter, *m_node, true, snapProperty);

        if (next)
                return true;
        return false;
}

bool EgcIdNodeIter::hasPrevious(void) const
{
        return hasPrevious(EgcSnapProperty::SnapVisibleCursor);
}

EgcNode &  EgcIdNodeIter::next(EgcSnapProperty snapProperty)
{
        EgcNode* old;
        EgcNode* next;
        EgcNodeIterator tempIter = *m_nodeIter;

        old = m_node;
        if (m_node == &m_nodeIter->peekPrevious()) {
                m_node = &m_nodeIter->peekNext();
                next = gotoNodeWithId(true, &tempIter, *m_node, false, snapProperty);
        } else {
                if (m_nodeIter->hasNext()) {
                        m_nodeIter->next();
                        tempIter = *m_nodeIter;
                        next = gotoNodeWithId(true, &tempIter, *m_node, false, snapProperty);
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

EgcNode &  EgcIdNodeIter::next(void)
{
        return next(EgcSnapProperty::SnapVisibleCursor);
}

EgcNode & EgcIdNodeIter::previous(EgcSnapProperty snapProperty)
{
        EgcNode* old;
        EgcNode* previous;
        EgcNodeIterator tempIter = *m_nodeIter;

        old = m_node;
        if (m_node == &m_nodeIter->peekNext()) {
                m_node = &m_nodeIter->peekPrevious();
                previous = gotoNodeWithId(false, &tempIter, *m_node, false, snapProperty);
        } else {
                if (m_nodeIter->hasPrevious()) {
                        m_nodeIter->previous();
                        tempIter = *m_nodeIter;
                        previous = gotoNodeWithId(false, &tempIter, *m_node, false, snapProperty);
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

EgcNode & EgcIdNodeIter::previous(void)
{
        return previous(EgcSnapProperty::SnapVisibleCursor);
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
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousNode, childIndex)) {
                                        childIndex = 0;
                                } else {
                                        if (childIndex)
                                                childIndex--;
                                }
                        } else if (nextNode) {
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*nextNode, childIndex))
                                        childIndex = 0;
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

EgcNode* EgcIdNodeIter::nextNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, EgcSnapProperty snapProperty) const
{
        EgcNode* retval = &currNode;
        EgcNodeIterator iter = *tempIter;
        bool visible;        
        EgcNode* nextNode = &iter.peekNext();
        EgcNode* prevNode = &iter.peekPrevious();

        if (    &currNode != nextNode
             && &currNode != prevNode)
                return nullptr;

        while (!(visible = nodeStateVisible(iter, *retval, snapProperty)) && iter.hasNext()) {
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

EgcNode* EgcIdNodeIter::prevNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, EgcSnapProperty snapProperty) const
{
        EgcNode* retval = &currNode;
        EgcNodeIterator iter = *tempIter;
        bool visible;
        EgcNode* nextNode = &iter.peekNext();
        EgcNode* prevNode = &iter.peekPrevious();

        if (    &currNode != nextNode
             && &currNode != prevNode)
                return nullptr;

        while (!(visible = nodeStateVisible(iter, *retval, snapProperty)) && iter.hasPrevious()) {
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

EgcNode* EgcIdNodeIter::gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, const EgcNode& node, bool checkFollowing,
                                       EgcSnapProperty snapProperty) const
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

                retval = nextNodeWithId(*retval, tempIter, snapProperty);
        } else {
                if (checkFollowing) {
                        if (curr == &tempIter->peekPrevious() && tempIter->hasPrevious())
                                retval = &tempIter->previous();
                        else
                                retval = &tempIter->peekPrevious();
                }
                retval = prevNodeWithId(*retval, tempIter, snapProperty);
        }

        return retval;
}

bool EgcIdNodeIter::nodeStateVisible(const EgcNodeIterator &iter, EgcNode& nodeToTest, EgcSnapProperty snapProperty) const
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

        if (snapProperty == EgcSnapProperty::SnapVisibleCursor) {
                // only visible cursor positions shall be taken into account,
                // but unvisible operators for example shall be taken into account when a operator
                // shall be modified. Therefore only check for a visible cursor.
                if (!mathmlIdExisting(node, state, otherNode, checkNext))
                        return false;
        }
        if (omitFollowingNode(node, state, rightSide(const_cast<EgcNodeIterator&>(iter), *node), snapProperty))
                return false;

        return true;
}

bool EgcIdNodeIter::omitFollowingNode(EgcNode* node, EgcIteratorState stateToTest, bool atRightSide, EgcSnapProperty snapProperty) const
{
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
        if (snapProperty == EgcSnapProperty::SnapAll)
                return false;

        if (snapProperty == EgcSnapProperty::SnapVisibleCursor)
                return !cursorVisible(*node, stateToTest);
        else if (snapProperty == EgcSnapProperty::SnapVisibleSigns)
                return !visibleSign(*node, stateToTest);
        else if (snapProperty == EgcSnapProperty::SnapModifyable)
                return !modifyableElement(*node, stateToTest);

        return true;
}

bool EgcIdNodeIter::cursorVisible(EgcNode& node, EgcIteratorState state) const
{
        bool retval = false;
        EgcNodeSide side;

        switch (state) {
        case EgcIteratorState::LeftIteration:
                side = EgcNodeSide::left;
                break;
        case EgcIteratorState::MiddleIteration:
                side = EgcNodeSide::middle;
                break;
        default:
                side = EgcNodeSide::right;
                break;
        }
        return node.cursorSnaps(side);

        return retval;
}

bool EgcIdNodeIter::visibleSign(EgcNode& node, EgcIteratorState state) const
{
        bool retval = false;
        EgcNodeSide side;

        switch (state) {
        case EgcIteratorState::LeftIteration:
                side = EgcNodeSide::left;
                break;
        case EgcIteratorState::MiddleIteration:
                side = EgcNodeSide::middle;
                break;
        default:
                side = EgcNodeSide::right;
                break;
        }

        return node.visibleSigns(side);

        return retval;
}

bool EgcIdNodeIter::modifyableElement(EgcNode& node, EgcIteratorState state) const
{
        bool retval = false;
        EgcNodeSide side;
        EgcNodeType type = node.getNodeType();

        switch (state) {
        case EgcIteratorState::LeftIteration:
                side = EgcNodeSide::left;
                break;
        case EgcIteratorState::MiddleIteration:
                side = EgcNodeSide::middle;
                break;
        default:
                side = EgcNodeSide::right;
                break;
        }

        return node.modifyableElement(side);

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

EgcNode* EgcIdNodeIter::insert(EgcNodeType type)
{
        EgcNode* node;
        EgcNode* retval = nullptr;
        bool right = rightSide();

        if (!m_node)
                return nullptr;
        if (m_node->isAtomicallyBoundChild()) {
                if (!moveToAtomicNode(right))
                        return nullptr;
        }

        node = m_node;

        if (besideBinEmptyNode(right)) {
                retval = replaceBinEmptyNodeBy(type, right);
                if (!retval)
                        return nullptr;

                m_iterPosAfterUpdate = node;
                m_atRightSideAfterUpdate = right;
                m_isInsert = true;

        } else {
                bool insertBeforeChild = false;
                if (&m_nodeIter->peekNext() == m_node)
                        insertBeforeChild = true;
                retval = m_nodeIter->insert(type, insertBeforeChild);
                if(!retval)
                        return nullptr;

                m_iterPosAfterUpdate = node;
                m_atRightSideAfterUpdate = right;
                m_isInsert = true;

        }
        
        return retval;
}

bool EgcIdNodeIter::remove(bool before)
{
        bool retval = false;
        EgcIteratorState state;
        EgcNode* node;

        m_isInsert = false;

        node = getNodeToModify(before, state);
        if (!node) return false;

        node = findAtomicNode(*node);
        if (!node)
                return false;

        if (!node)
                return false;
        if (!node->getParent())
                return false;
        if (node->getNodeType() == EgcNodeType::BaseNode)
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

        if (!bin.getChild(0) || !bin.getChild(1))
                deleteAll = true;
        else if (state == EgcIteratorState::LeftIteration || state == EgcIteratorState::RightIteration)
                deleteAll = true;
        else if (before && bin.getChild(1)->getNodeType() == EgcNodeType::EmptyNode)
                deleteChild = true;
        else if (!before && bin.getChild(0)->getNodeType() == EgcNodeType::EmptyNode)
                deleteChild = true;
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
                setAtNodeDelayed(*m_node, !before);
        } else if(deleteChild) {
                if (!m_node)
                        return false;
                if (!bin.getChild(0))
                        return false;
                if (!bin.getChild(1))
                        return false;
                if (before)
                        index = 0;
                else
                        index = 1;

                EgcNode* tmpChild = bin.getChild(index);
                QScopedPointer<EgcNode> child(m_formula.cut(*tmpChild));
                if (child.isNull())
                        return false;
                m_formula.paste(*child.take(), node);

                setAtNodeDelayed(*tmpChild, before);

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
        else if (state == EgcIteratorState::MiddleIteration)
                deleteChild = true;
        else
                return false;

        //search for the index of the child tree of the nodeToDelete we are currently in
        if (deleteAll) {
                EgcContainerNode* parent = node.getParent();
                quint32 index;
                if (!parent->getIndexOfChild(node, index))
                        return false;
                QScopedPointer<EgcNode> container(m_formula.cut(node));
                if (parent)
                        setAtNodeDelayed(*(parent->getChild(index)), false);
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
                if (m_node->isAtomicallyBoundChild())
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

        EgcIdNodeIter iter(m_formula);
        iter.setAtNode(node, before);
        if (before) {
                if (iter.hasPrevious())
                        iter.previous();
        } else {
                if (iter.hasNext())
                        iter.next();
        }
        EgcNode& nd = iter.getNode();
        QScopedPointer<EgcNode> child(m_formula.cut(*unary.getChild(0)));
        if (child.isNull())
                return false;
        m_formula.paste(*child.take(), node);
        setAtNodeDelayed(nd, before);

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

        if (    node.getNodeType() == EgcNodeType::EmptyNode
             && parent->isFlexNode()) {
                EgcFlexNode* flex = static_cast<EgcFlexNode*>(parent);
                flex->remove(index);
                setAtNodeDelayed(*parent, before);
        } else {
                QScopedPointer<EgcNode> leaf(m_formula.cut(node));
                EgcNode* child = parent->getChild(index);
                if (child)
                        setAtNodeDelayed(*child, before);
        }

        return true;
}

bool EgcIdNodeIter::replaceByEmtpy(bool cursorRight)
{
        EgcNode* node = m_node;

        if (!node) return false;

        node = findAtomicNode(*node);
        if (!node)
                return false;

        if (!node)
                return false;
        if (!node->getParent())
                return false;

        QScopedPointer<EgcNode> empty(EgcNodeCreator::create(EgcNodeType::EmptyNode));
        if (empty.isNull())
                return false;
        EgcNode* tmp = empty.data();
        m_formula.paste(*empty.take(), *node);
        setAtNodeDelayed(*tmp, cursorRight);

        return true;
}

EgcNode* EgcIdNodeIter::getNodeToModify(bool before, EgcIteratorState &state, bool goOn)
{
        EgcNode* nodeToModify = nullptr;
        static EgcNodeIterator iter = *m_nodeIter;
        static EgcNode* node;

        if (!goOn) {
                iter = *m_nodeIter;
                node = m_node;
        }

        if (before) {
                if (rightSide())
                        nodeToModify = node;
                else
                        nodeToModify = gotoNodeWithId(false, &iter, *node, true,
                                                      EgcSnapProperty::SnapModifyable);
        } else {
                if (!rightSide())
                        nodeToModify = node;
                else
                        nodeToModify = gotoNodeWithId(true, &iter, *node, true,
                                                      EgcSnapProperty::SnapModifyable);
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
                
                setAtNode(*nodeToDelete, true, EgcSnapProperty::SnapAll);
                m_nodeIter->remove(false);
                
                setAtNodeDelayed(*cParent->getChild(index), atRightSide);
        }

        return true;
}


bool EgcIdNodeIter::finishModOperation(void)
{
        bool retval = false;

        if (m_iterPosAfterUpdate) {
                setAtNode(*m_iterPosAfterUpdate, m_atRightSideAfterUpdate);
                //correct cursor for remove operations
                if (m_isInsert) {
                        if (m_atRightSideAfterUpdate)
                                next();
                        else
                                previous();
                }
                m_iterPosAfterUpdate = nullptr;
        }

        if (!m_atRightSideAfterUpdate && m_isInsert)
                retval = true;
        else
                retval = false;
        m_isInsert = false;

        return retval;
}

void EgcIdNodeIter::setAtNodeDelayed(EgcNode& node, bool atRightSide)
{
        if (m_lockDelayedUpdate == false && m_formula.isNodeInFormula(node)) {
                m_iterPosAfterUpdate = &node;
                m_atRightSideAfterUpdate = atRightSide;
                m_isInsert = false;
        }
}

void EgcIdNodeIter::lockDelayedCursorUpdate(void)
{
        m_lockDelayedUpdate = true;
}

void EgcIdNodeIter::unlockDelayedCursorUpdate(void)
{
        m_lockDelayedUpdate = false;
}

EgcNode* EgcIdNodeIter::findAtomicNode(EgcNode& node) const
{
        EgcNode* node_ptr = &node;

        while(node_ptr->isAtomicallyBoundChild()) {
                node_ptr = node_ptr->getParent();
                if (!node_ptr)
                        return nullptr;
        }

        return node_ptr;
}

bool EgcIdNodeIter::moveToAtomicNode(bool forward)
{
        EgcNode* node = m_node;
        if (!m_node)
                return false;

        if (    (m_node != &m_nodeIter->peekNext())
             && (m_node != &m_nodeIter->peekPrevious()))
                return false;

        node = findAtomicNode(*m_node);
        if (!node)
                return false;

        if (forward) {
                while(hasNext(EgcSnapProperty::SnapAll) && &m_nodeIter->peekPrevious() != node) {
                        next(EgcSnapProperty::SnapAll);
                }
                if (&m_nodeIter->peekPrevious() != node)
                        return false;
        } else {
                while(hasPrevious(EgcSnapProperty::SnapAll) && &m_nodeIter->peekNext() != node) {
                        previous(EgcSnapProperty::SnapAll);
                }
                if (&m_nodeIter->peekNext() != node)
                        return false;
        }

        return true;
}

bool EgcIdNodeIter::besideBinEmptyNode(bool right)
{
        EgcNode* node;
        EgcIteratorState state;

        node = getNodeToModify(!right, state);

        if (!node)
                return false;

        if (node->getNodeType() == EgcNodeType::BinEmptyNode)
                return true;

        return false;
}

EgcNode* EgcIdNodeIter::replaceBinEmptyNodeBy(EgcNodeType type, bool right)
{
        const EgcNode *node = m_node;

        if (!m_node)
                return nullptr;

        (void) gotoNodeWithId(right, m_nodeIter.data(), *node, true, EgcSnapProperty::SnapModifyable);

        return m_nodeIter->replaceBinEmptyNodeBy(type);
}

NodeIterReStructData EgcIdNodeIter::getRestructureData(void) const
{
        NodeIterReStructData data;
        data.m_node = m_node;
        data.m_iterPosAfterUpdate = m_iterPosAfterUpdate;

        return data;
}

bool EgcIdNodeIter::updateRestructureData(NodeIterReStructData& data)
{
        m_nodeIter->invalidate();
        m_node = data.m_node;
        m_iterPosAfterUpdate = data.m_iterPosAfterUpdate;

        return true;
}
