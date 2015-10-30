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
#include "../view/egcformulaitem.h"
#include "../specialNodes/egcnode.h"
#include "../view/egcscreenpos.h"

EgcScrPosIterator::EgcScrPosIterator(EgcFormulaItem &formula) : m_pos{formula.getScreenPos()}
{
        m_i.reset(new QVectorIterator<EgRenderingPosition>(m_pos.m_positions));
        m_i->toBack();
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

const EgRenderingPosition & EgcScrPosIterator::next(void)
{
        return m_i->next();
}

const EgRenderingPosition &EgcScrPosIterator::previous(void)
{
        return m_i->previous();
}

const EgRenderingPosition &EgcScrPosIterator::peekNext(void) const
{
        return m_i->peekNext();
}

const EgRenderingPosition &EgcScrPosIterator::peekPrevious(void) const
{
        return m_i->peekPrevious();
}

void EgcScrPosIterator::toBack(void)
{
        m_i->toBack();
}

void EgcScrPosIterator::toFront(void)
{
        m_i->toFront();
}

bool EgcScrPosIterator::findMathMlId(quint32 mathmlId, EgRenderingPosition* var) const
{
        EgRenderingPosition tmp;
        QVectorIterator<EgRenderingPosition> iter(*m_i);
        while(iter.hasNext()) {
                tmp = iter.next();
                if (tmp.m_nodeId == mathmlId && tmp.m_index == 0) {
                        if (var)
                                *var = tmp;
                        return true;
                }
        }

        return false;
}

