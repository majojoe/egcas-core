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
#include "../entities/egcformulaentity.h"
#include "../specialNodes/egcnode.h"
#include "../view/egcscreenpos.h"
#include "../structural/concreteNodes/egcnumbernode.h"
#include "../structural/concreteNodes/egcvariablenode.h"
#include "../specialNodes/egcflexnode.h"
#include "egcnodeiterator.h"


EgcScrPosIterator::EgcScrPosIterator(const EgcFormulaEntity& formula) : m_prevNode{nullptr}, m_nextNode{nullptr},
                                                              m_prevSubind{-1}, m_nextSubind{0}, m_subindHist{-1},
                                                              m_lookup{formula.getMathmlMappingCRef()},
                                                              m_nodeIter{new EgcNodeIterator(formula)}
{
        m_nodeIter->toBack();

}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
        if (m_nodeIter->hasNext() || hasNextSubind())
                return true;
        else
                return false;
}

bool EgcScrPosIterator::hasPrevious(void) const
{
        if (m_nodeIter->hasPrevious() || hasPreviousSubind())
                return true;
        else
                return false;
}

const quint32 EgcScrPosIterator::next(void)
{
//        quint32 retval;

//        if (hasNextSubind()) {
//                nextSubind();
//        } else if (m_nodeIter->hasNext()) {
//                retval = m_lookup.getIdFrame(m_nodeIter->next());
//        }
//        if (retval == 0)
//                return m_pseudoRef; //the content is undefined per definition (content will be wrong)

//        return retval;
}

const quint32 EgcScrPosIterator::previous(void)
{
//        if (hasPreviousSubind()) {
//                previousSubind();
//        } else if (m_rightSide == true) {
//                m_rightSide = false;
//        } else if (m_i->hasPrevious()) {
//                m_history = &m_i->previous();
//                m_rightSide = true;
//        }
        
//        if (!m_history)
//                return m_pseudoRef; //the content is undefined per definition (content will be wrong)

//        return m_history->m_mathmlId;
}

const quint32 EgcScrPosIterator::peekNext(void) const
{
//        return m_i->peekNext().m_mathmlId;
}

const quint32 EgcScrPosIterator::peekPrevious(void) const
{
//        return m_i->peekPrevious().m_mathmlId;
}

void EgcScrPosIterator::toBack(void)
{
//        m_i->toBack();
//        m_prevSubind = -1;
//        m_nextSubind = 0;
//        m_history = nullptr;
//        while (hasNextSubind()) {
//                nextSubind();
//        }
//        if (m_i->hasPrevious()) {
//                m_history = &m_i->peekPrevious();
//        }
}

void EgcScrPosIterator::toFront(void)
{
//        m_i->toFront();
//        m_prevSubind = -1;
//        m_nextSubind = 0;
//        m_history = nullptr;
//        if (m_i->hasPrevious()) {
//                m_history = &m_i->peekNext();
//        }
}

const EgcNode* EgcScrPosIterator::node(void)
{
//        if (!m_history)
//                return nullptr;

//        return m_history->m_node;
}

bool EgcScrPosIterator::rightSide(void)
{
//        return m_rightSide;
}

qint32& EgcScrPosIterator::subIndex(void)
{
//        return m_subindHist;
}

bool EgcScrPosIterator::hasNextSubind(void) const
{
//        if (!m_history) { //we are at the beginning
//                if (!m_i->hasNext())
//                        return false;
//                if (m_i->peekNext().m_node->nrSubindexes())
//                        return true;
//                else
//                        return false;
//        }

//        if (m_nextSubind < m_history->m_node->nrSubindexes())
//                return true;
//        else
//                return false;
}

bool EgcScrPosIterator::hasPreviousSubind(void) const
{
//        if (m_prevSubind > -1)
//                return true;
//        else
//                return false;
}

void EgcScrPosIterator::nextSubind(void)
{
//        if (hasNextSubind()) {
//                m_subindHist = m_nextSubind;
//                m_prevSubind++;
//                m_nextSubind++;
//                m_rightSide = true;
//        }
}

void EgcScrPosIterator::previousSubind(void)
{
//        if (hasPreviousSubind()) {
//                m_subindHist = m_prevSubind;
//                m_prevSubind--;
//                m_nextSubind--;
//                m_rightSide = false;
//        }
}

const quint32& EgcScrPosIterator::id(void)
{
//        if (m_history)
//                return m_history->m_mathmlId;
//        else
//                return m_pseudoRef; // this is not a valid id , but a valid reference
}

bool EgcScrPosIterator::mathmlIdExisting(EgcNode* node) const
{
//        if (!node)
//                return false;

//        EgcIteratorState state = m_nodeIter->getLastState();

//        if (    state == EgcIteratorState::LeftIteration
//             || state == EgcIteratorState::RightIteration) {
//                if (m_lookup.getIdFrame(*node) == 0)
//                        return false;
//                else
//                        return true;
//        } else {  //EgcIteratorState::MiddleIteration
//                QList<quint32> list = m_lookup.getIdsNonFrame(*node);
//                quint32 childIndex = 0;

//                if (    node->isBinaryNode()
//                     || node->isFlexNode()) {
//                        if (node->isContainer()) {
//                                if (state == EgcIteratorState::LeftIteration) {
//                                        childIndex = 0;
//                                } else if ( state == EgcIteratorState::RightIteration) {
//                                        childIndex = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();
//                                } else {
//                                        if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(m_nodeIter->peekPrevious(), childIndex))
//                                              childIndex = 0;
//                                }
//                        } else {
//                                childIndex = 0;
//                        }

//                        if (list.size() > childIndex) {
//                                if (list.at(childIndex) != 0)
//                                        return true;
//                        }
//                }
//        }

//        return false;
}

void EgcScrPosIterator::prevNodeWithId(void)
{

}

void EgcScrPosIterator::nextNodeWithId(void)
{
        EgcNode* nextNode;
        EgcIteratorState lastState;
        EgcIteratorState newState;

        do {
                lastState = m_nodeIter->getLastState();
                nextNode = &(m_nodeIter->next());
                newState = m_nodeIter->getLastState();

        } while (    mathmlIdExisting(nextNode)
                  && !(lastState == newState && nextNode == m_nextNode));

        m_nextNode = nextNode;
}
