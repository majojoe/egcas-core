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
                                                                        m_originNode{nullptr}
{

        m_subIdIter.reset(new EgcSubindNodeIter(m_nodeIter->getNode()));
        // the subid iterator is at the beginning, so position it at the end.
        m_subIdIter->toBack();
}

EgcScrPosIterator::~EgcScrPosIterator()
{
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

        m_nodeIter->setAtNode(*parent, r_side, true);

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
        m_nodeIter->setAtNode(*m_lastUnderlinedNode, r_side, true);

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

bool EgcScrPosIterator::remove(void)
{
        if (!m_subIdIter->hasNext()) {
                if (m_lastUnderlinedNode)
                        return m_nodeIter->deleteTree(false);
                else
                        return m_nodeIter->remove(false);
        } else {
                bool retval = m_subIdIter->remove(false);
                balanceNodeIter();
                return retval;
        }
}

bool EgcScrPosIterator::backspace(void)
{
        if (!m_subIdIter->hasPrevious()) {
                if (m_lastUnderlinedNode)
                        return m_nodeIter->deleteTree(true);
                else
                        return m_nodeIter->remove(true);
        } else {
                return m_subIdIter->remove(true);
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

bool EgcScrPosIterator::insertOp(QChar operations)
{
        bool retval = false;

        if (    operations == '('
             || operations == ')') {
                if (    operations == '('
                     && (    !rightSide() ))
                        return insertUnaryOp(EgcNodeType::ParenthesisNode, &m_nodeIter->getNode());
                if (    operations == ')'
                     && (    rightSide()) )
                        return insertUnaryOp(EgcNodeType::ParenthesisNode, &m_nodeIter->getNode());
        }

        return retval;
}

bool EgcScrPosIterator::insertUnaryOp(EgcNodeType type, EgcNode* node)
{
        if (node) {
                if (node->getNodeType() == EgcNodeType::AlnumNode)
                        node = node->getParent();
        }

        if (!node)
                return false;

        if (type == EgcNodeType::BaseNode)
                return false;
        
        QScopedPointer<EgcNode> tmp(EgcNodeCreator::create(type));
        if (!tmp->isUnaryNode())
                return false;

        m_nodeIter->insert(type);
        
        return true;
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
}

void EgcScrPosIterator::updatePointer(EgcNode* oldPointer, EgcNode* newPointer, bool right)
{
        m_lastUnderlinedNode = nullptr;
        m_originNode = nullptr;

        if (!newPointer)
                return;

        m_nodeIter->setAtNodeDelayed(*newPointer, right);
        if (oldPointer != newPointer) {
                m_subIdIter->setNode(*newPointer);
                if (right)
                        m_subIdIter->toBack();
        }
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
