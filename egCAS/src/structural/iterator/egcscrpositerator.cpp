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


#include "egcscrpositerator.h"
#include "entities/egcformulaentity.h"
#include "specialNodes/egcnode.h"
#include "egcscreenpos.h"
#include "iterator/screenHelpers/egcidnodeiter.h"
#include "iterator/screenHelpers/egcsubidnodeiter.h"
#include "iterator/egcnodeiterator.h"
#include "structural/actions/egcoperations.h"
#include "structural/egcnodecreator.h"
#include "structural/specialNodes/egcnode_gen.h"

EgcScrPosIterator::EgcScrPosIterator(EgcFormulaEntity& formula) : m_lookup(formula.getMathmlMappingCRef()), //gcc bug
                                                                        m_nodeIter{new EgcIdNodeIter(formula)},
                                                                        m_lastUnderlinedNode{nullptr},
                                                                        m_originNode{nullptr},
                                                                        m_checkForBinEmpty{false},
                                                                        m_wasBackspace{false}
{

        m_subIdIter.reset(new EgcSubindNodeIter(m_nodeIter->getNode()));
        // the subid iterator is at the beginning, so position it at the end.
        m_subIdIter->toBack();
}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

EgcScrPosIterator::EgcScrPosIterator(const EgcScrPosIterator& orig) : m_lookup(orig.m_lookup), //gcc bug
                                                                      m_nodeIter{new EgcIdNodeIter(*orig.m_nodeIter)},
                                                                      m_subIdIter{new EgcSubindNodeIter(*orig.m_subIdIter)},
                                                                      m_lastUnderlinedNode{orig.m_lastUnderlinedNode},
                                                                      m_originNode{orig.m_originNode},
                                                                      m_checkForBinEmpty{orig.m_checkForBinEmpty},
                                                                      m_wasBackspace{orig.m_wasBackspace}

{

}

void EgcScrPosIterator::setCursorAt(EgcNode* node, quint32 subInd, bool rSide)
{
        if (!node)
                return;

        if (node->isBinaryNode()) {
                if (    node->isOperation()
                     && node->visibleSigns(EgcNodeSide::middle)
                     && !node->visibleSigns(EgcNodeSide::left)
                     && !node->visibleSigns(EgcNodeSide::right)) {
                        EgcContainerNode* container = static_cast<EgcContainerNode*>(node);
                        EgcNode* child;
                        if (!rSide)
                                child = container->getChild(0);
                        else
                                child = container->getChild(1);
                        if (child) {
                                node = child;
                                rSide = !rSide;
                        }
                }
        }

        m_nodeIter->setAtNode(*node, rSide);
        m_subIdIter->setNode(m_nodeIter->getNode());
        m_subIdIter->toFront();
        if (&m_nodeIter->getNode() == node) {
                for (int i = m_subIdIter->peekNext(); m_subIdIter->peekNext() < subInd; i++) {
                        if (!m_subIdIter->hasNext())
                                break;
                        m_subIdIter->next();
                }
                if (!rSide && m_subIdIter->hasPrevious())
                        m_subIdIter->previous();
        }
}

bool EgcScrPosIterator::hasNext(void) const
{
        if (m_nodeIter->hasNext() || m_subIdIter->hasNext())
                return true;
        else
                return false;
}

bool EgcScrPosIterator::hasPrevious(void) const
{
        if (m_nodeIter->hasPrevious() || m_subIdIter->hasPrevious())
                return true;
        else
                return false;
}

const quint32 EgcScrPosIterator::next(void)
{
        if (m_lastUnderlinedNode && !rightSide()) {
                switchSideUnderlinedNode();
                return m_nodeIter->id();
        }

        if (m_subIdIter->hasNext()) {
                (void) m_subIdIter->next();
                //switch from left side to right side, if we are on the half way to the end of the node
                balanceNodeIter();
        } else if (m_nodeIter->hasNext()) {
                m_nodeIter->next();
                m_subIdIter->setNode(m_nodeIter->getNode());
                m_lastUnderlinedNode = nullptr;
        }

        return m_nodeIter->id();
}

const quint32 EgcScrPosIterator::previous(void)
{
        if (m_lastUnderlinedNode && rightSide()) {
                switchSideUnderlinedNode();
                return m_nodeIter->id();
        }

        if (m_subIdIter->hasPrevious()) {
                (void) m_subIdIter->previous();
                //switch from right side to left side, if we are on the half way to the front of the node
                balanceNodeIter();
        } else if (m_nodeIter->hasPrevious()) {
                m_nodeIter->previous();
                m_subIdIter->setNode(m_nodeIter->getNode());
                m_subIdIter->toBack();
                m_lastUnderlinedNode = nullptr;
        }

        return m_nodeIter->id();
}

void EgcScrPosIterator::toBack(void)
{
        m_nodeIter->toBack();
        m_subIdIter->setNode(m_nodeIter->getNode());
        m_subIdIter->toBack();        
}

void EgcScrPosIterator::toFront(void)
{
        m_nodeIter->toFront();
        m_subIdIter->setNode(m_nodeIter->getNode());
        m_subIdIter->toFront();
}

const EgcNode* EgcScrPosIterator::node(void)
{
        return &m_nodeIter->getNode();
}

bool EgcScrPosIterator::rightSide(void)
{
        if (!m_nodeIter->getNode().isContainer()) {
                if (m_subIdIter->peekPrevious() == -1)
                        return false;
                else
                        return true;
        } else {
                return m_nodeIter->rightSide();
        }
}

int EgcScrPosIterator::subIndex(void)
{
        int ind;

        ind = m_subIdIter->peekPrevious();
        if (ind == -1)
                ind = m_subIdIter->peekNext();

        return ind;
}

quint32 EgcScrPosIterator::id(void)
{
        return m_lookup.getIdFrame(m_nodeIter->getNode());
}

EgcNode& EgcScrPosIterator::getNextVisibleParentNode(void)
{
        int loops = 0;
        quint32 id = 0;

        do {
                if (!m_lastUnderlinedNode) {
                        m_lastUnderlinedNode = &m_nodeIter->getOriginNodeToMark(m_nodeIter->getNode());
                        m_originNode = m_lastUnderlinedNode;
                } else {
                        EgcNode* parent = m_lastUnderlinedNode->getParent();
                        if (parent) {
                                m_lastUnderlinedNode = parent;
                                if (m_lastUnderlinedNode == m_originNode)
                                        loops++;
                        } else {
                                m_lastUnderlinedNode = m_originNode;
                        }
                }
                if (m_lastUnderlinedNode)
                        id = m_lookup.getIdFrame(*m_lastUnderlinedNode);
        } while (    (!id)
                  || ( loops > 0));
                
        return *m_lastUnderlinedNode;
}

quint32 EgcScrPosIterator::getNextVisibleParent(void)
{
        bool r_side = rightSide();

        EgcNode* parent = &getNextVisibleParentNode();

        m_nodeIter->setAtNode(*parent, r_side, EgcSnapProperty::SnapAll);

        m_subIdIter->setNode(m_nodeIter->getNode());
        if (r_side) {
                m_subIdIter->toBack();
        } else {
                m_subIdIter->toFront();
        }

        return m_lookup.getIdFrame(*parent);
}

void EgcScrPosIterator::switchSideUnderlinedNode(void)
{
        bool r_side = rightSide();

        if (!m_lastUnderlinedNode)
                return;

        r_side = !r_side;
        m_nodeIter->setAtNode(*m_lastUnderlinedNode, r_side, EgcSnapProperty::SnapAll);

        m_subIdIter->setNode(m_nodeIter->getNode());
        if (r_side) {
                m_subIdIter->toBack();
        } else {
                m_subIdIter->toFront();
        }

        return;
}

bool EgcScrPosIterator::insert(QChar character)
{
        bool retval = m_subIdIter->insert(character);
        balanceNodeIter();
        return retval;
}

EgcNode* EgcScrPosIterator::insert(EgcNodeType type)
{
        return m_nodeIter->insert(type);
}

bool EgcScrPosIterator::remove(void)
{
        bool str;
        return remove(str);
}

bool EgcScrPosIterator::remove(bool &structureChanged)
{
        if (m_nodeIter->getNode().getNodeType() == EgcNodeType::EmptyNode && !m_nodeIter->rightSide() && hasNext()) {
                (void) next();
                return false;
        }
        if (!m_subIdIter->hasNext()) {
                structureChanged = true;
                if (m_lastUnderlinedNode) {
                        return m_nodeIter->deleteTree(false);
                } else {
                        bool retval = m_nodeIter->remove(false);
                        return retval;
                }
        } else {
                structureChanged = false;
                bool retval = m_subIdIter->remove(false);
                balanceNodeIter();
                if (m_nodeIter->getNode().nrSubindexes() == 0) {
                        if (!m_nodeIter->replaceByEmtpy(false))
                                retval = false;
                        else
                                structureChanged = true;
                }

                return retval;
        }
}

bool EgcScrPosIterator::backspace(void)
{
        bool str;
        return backspace(str);
}

bool EgcScrPosIterator::backspace(bool &structureChanged)
{
        if (m_nodeIter->getNode().getNodeType() == EgcNodeType::EmptyNode && m_nodeIter->rightSide() && hasPrevious()) {
                (void) previous();
                return false;
        }
        if (!m_subIdIter->hasPrevious()) {
                bool retval;
                structureChanged = true;
                if (m_lastUnderlinedNode) {
                        retval = m_nodeIter->deleteTree(true);
                } else {
                        retval = m_nodeIter->remove(true);
                        m_checkForBinEmpty = true;
                }
                m_wasBackspace = true;
                return retval;
        } else {
                structureChanged = false;
                bool retval;
                retval =  m_subIdIter->remove(true);
                if (m_nodeIter->getNode().nrSubindexes() == 0) {
                        if (!m_nodeIter->replaceByEmtpy(false))
                                retval = false;
                        else
                                structureChanged = true;
                }

                return retval;
        }
}

void EgcScrPosIterator::resetUnderline(void)
{
        m_lastUnderlinedNode = nullptr;
}

bool EgcScrPosIterator::isUnderlineActive(void)
{
        if (m_lastUnderlinedNode)
                return true;
        else
                return false;
}

void EgcScrPosIterator::finishFormulaChange(void)
{
        bool doStep;

        m_lastUnderlinedNode = nullptr;
        doStep = m_nodeIter->finishModOperation();
        if (m_subIdIter->node() != &m_nodeIter->getNode()) {
                m_subIdIter->setNode(m_nodeIter->getNode());        
                if (!m_nodeIter->getNode().isContainer() && m_nodeIter->rightSide())
                        m_subIdIter->toBack();
        }
        if (hasNext() && doStep)
                next();

        if (m_checkForBinEmpty) {
                if (m_nodeIter->besideBinEmptyNode(false) && hasPrevious())
                        (void) previous();
        }
        if (m_wasBackspace) {
                if (    m_nodeIter->getNode().getNodeType() == EgcNodeType::EmptyNode && m_nodeIter->rightSide()
                     && hasPrevious()) {
                        (void) previous();
                }
        }
        m_checkForBinEmpty = false;
        m_wasBackspace = false;
}

void EgcScrPosIterator::updatePointer(EgcNode* newPointer, bool right)
{
        m_lastUnderlinedNode = nullptr;
        m_originNode = nullptr;

        if (!newPointer)
                return;

        m_nodeIter->setAtNodeDelayed(*newPointer, right);
}

void EgcScrPosIterator::balanceNodeIter(void)
{
        if (    (m_subIdIter->lastSubind() > (m_nodeIter->getNode().nrSubindexes() - 1) / 2)
             || !m_subIdIter->hasNext()) {
                if (!m_nodeIter->rightSide())
                        m_nodeIter->next();
        } else {
                if (m_nodeIter->rightSide())
                        m_nodeIter->previous();
        }        
}

void EgcScrPosIterator::lockDelayedCursorUpdate(void)
{
        m_nodeIter->lockDelayedCursorUpdate();
}

void EgcScrPosIterator::unlockDelayedCursorUpdate(void)
{
        m_nodeIter->unlockDelayedCursorUpdate();
}

void EgcScrPosIterator::invalidateCursor(EgcNode& baseNode)
{
        m_nodeIter->toFront();
        m_subIdIter->setNode(baseNode);
}

void EgcScrPosIterator::setCursorAtDelayed(EgcNode* node, bool rSide)
{
        if (node)
                m_nodeIter->setAtNodeDelayed(*node, rSide);
}

NodeIterReStructData EgcScrPosIterator::getRestructureData() const
{
        return m_nodeIter->getRestructureData();
}

bool EgcScrPosIterator::updateRestructureData(NodeIterReStructData& data)
{
        m_lastUnderlinedNode = nullptr;
        m_originNode = nullptr;

        if (!m_nodeIter->updateRestructureData(data))
                return false;

        return true;
}
