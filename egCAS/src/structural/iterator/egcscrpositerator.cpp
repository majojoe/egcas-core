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
#include "egcnodeiterator.h"


EgcScrPosIterator::EgcScrPosIterator(const EgcFormulaEntity& formula) : m_prevSubind{-1}, m_nextSubind{0}, m_subindHist{-1},
                                                              m_lookup{formula.getMathmlMappingCRef()},
                                                              m_nodeIter{new EgcNodeIterator(formula)}
{
}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
//        if (m_i->hasNext() || hasNextSubind() || m_rightSide == false)
//                return true;
//        else
//                return false;
}

bool EgcScrPosIterator::hasPrevious(void) const
{
//        if (m_i->hasPrevious() || hasPreviousSubind() || m_rightSide == true)
//                return true;
//        else
//                return false;
}

const quint32 & EgcScrPosIterator::next(void)
{
//        if (hasNextSubind()) {
//                nextSubind();
//        } else if (m_rightSide == false) {
//                m_rightSide = true;
//        } else if (m_i->hasNext()) {
//                m_history = &m_i->next();
//                m_rightSide = false;
//        }
//        if (!m_history)
//                return m_pseudoRef; //the content is undefined per definition (content will be wrong)

//        return m_history->m_mathmlId;
}

const quint32 &EgcScrPosIterator::previous(void)
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

const quint32 &EgcScrPosIterator::peekNext(void) const
{
//        return m_i->peekNext().m_mathmlId;
}

const quint32& EgcScrPosIterator::peekPrevious(void) const
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
