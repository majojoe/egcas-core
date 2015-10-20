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

EgcScrPosIterator::EgcScrPosIterator(const EgcFormulaEntity& formula) : m_formula{formula},
                                                                        m_pos{formula.getScreenPos()}
{
        m_i.reset(new QVectorIterator<EgRenderingPosition>(m_pos.m_positions));
        m_i->toBack();
}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
        if (!m_pos.isConsistent())
                return false;

        return m_i->hasNext();
}

bool EgcScrPosIterator::hasPrevious(void) const
{
        if (!m_pos.isConsistent())
                return false;

        return m_i->hasPrevious();
}

EgcNode & EgcScrPosIterator::next(void)
{
#warning history is not written when we give back the base element
        if (!m_pos.isConsistent())
                return m_formula.getBaseElement();

        m_history = &m_i->next();
        //direct dereferencing is allowed here since the m_pos class is consistent!
        return *m_pos.m_lookup.value(m_history->m_nodeId);
}

EgcNode & EgcScrPosIterator::previous(void)
{
        #warning history is not written when we give back the base element
        if (!m_pos.isConsistent())
                return m_formula.getBaseElement();

        m_history = &m_i->previous();
        //direct dereferencing is allowed here since the m_pos class is consistent!
        return *m_pos.m_lookup.value(m_history->m_nodeId);
}

EgcNode & EgcScrPosIterator::peekNext(void) const
{
        if (!m_pos.isConsistent())
                return m_formula.getBaseElement();

        EgRenderingPosition pos = m_i->peekNext();
        //direct dereferencing is allowed here since the m_pos class is consistent!
        return *m_pos.m_lookup.value(pos.m_nodeId);
}

EgcNode & EgcScrPosIterator::peekPrevious(void) const
{
        if (!m_pos.isConsistent())
                return m_formula.getBaseElement();

        EgRenderingPosition pos = m_i->peekPrevious();
        //direct dereferencing is allowed here since the m_pos class is consistent!
        return *m_pos.m_lookup.value(pos.m_nodeId);
}

void EgcScrPosIterator::toBack(void)
{
        m_i->toBack();
}

void EgcScrPosIterator::toFront(void)
{
        m_i->toFront();
}

quint32 EgcScrPosIterator::subindex(void)
{
        m_history->m_index;
}

const EgcNode& EgcScrPosIterator::node()
{
        m_pos.m_lookup.value(m_history->m_nodeId);
}

