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

EgcScrPosIterator::EgcScrPosIterator(EgcMathmlLookup& data) : m_index{0}, m_rightSide{false}, m_lookup{&data},
                                                              m_history{nullptr}
{
        m_i.reset(new QVectorIterator<EgcMathmlIdMapping>(m_lookup->m_lookup));
        m_i->toBack();
        while (hasNextSubind()) {
                nextSubind();
        }
        if (m_i->hasPrevious()) {
                m_history = &m_i->peekPrevious();
        }
}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
        return m_i->hasNext();
}

bool EgcScrPosIterator::hasPrevious(void) const
{
        return m_i->hasPrevious();
}

const quint32 & EgcScrPosIterator::next(void)
{
        if (hasNextSubind())
                nextSubind();
        else if (m_i->hasNext())
                m_history = &m_i->next();
        return m_history->m_mathmlId;
}

const quint32 &EgcScrPosIterator::previous(void)
{
        if (hasPreviousSubind())
                previousSubind();
        else if (m_i->hasPrevious())
                m_history = &m_i->previous();
        return m_history->m_mathmlId;
}

const quint32 &EgcScrPosIterator::peekNext(void) const
{
        return m_i->peekNext().m_mathmlId;
}

const quint32& EgcScrPosIterator::peekPrevious(void) const
{
        return m_i->peekPrevious().m_mathmlId;
}

void EgcScrPosIterator::toBack(void)
{
        m_i->toBack();
        m_index = 0;
        while (hasNextSubind()) {
                nextSubind();
        }
        m_rightSide = true;
        if (m_i->hasPrevious()) {
                m_history = &m_i->peekPrevious();
        }
}

void EgcScrPosIterator::toFront(void)
{
        m_i->toFront();
        m_rightSide = false;
        m_index = 0;
        if (m_i->hasPrevious()) {
                m_history = &m_i->peekPrevious();
        }
}

const EgcNode* EgcScrPosIterator::node(void)
{
        return m_history->m_node;
}

bool EgcScrPosIterator::rightSide(void)
{
        return m_rightSide;
}

quint32& EgcScrPosIterator::subIndex(void)
{
        return m_index;
}

bool EgcScrPosIterator::hasNextSubind(void) const
{
        if (m_history->m_node) {
                int size = m_history->m_node->nrSubindexes();
                if (m_index < size)
                        return true;
        }

        return false;
}

bool EgcScrPosIterator::hasPreviousSubind(void) const
{
        if (m_index > 0)
                return true;

        return false;
}

void EgcScrPosIterator::nextSubind(void)
{
        if (hasNextSubind()) {
                m_index++;
                m_rightSide = true;
        }
}

void EgcScrPosIterator::previousSubind(void)
{
        if (hasPreviousSubind())
                m_index--;

        if (m_index == 0)
                m_rightSide = false;
}
