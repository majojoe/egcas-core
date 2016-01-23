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

#include <QChar>
#include "egcsubidnodeiter.h"
#include "specialNodes/egcnode.h"
#include "concreteNodes/egcnumbernode.h"

EgcSubindNodeIter::EgcSubindNodeIter(EgcNode& node) : m_node(&node)
{
        setNode(node);
}

EgcSubindNodeIter::~EgcSubindNodeIter()
{

}

void EgcSubindNodeIter::setNode(EgcNode& node)
{
        m_node = &node;
        m_nextId = -1;
        m_prevId = -1;
        m_histId = -1;

        if (m_node->nrSubindexes())
                m_nextId = 0;
}

bool EgcSubindNodeIter::hasNext(void) const
{
        if (m_nextId != -1)
                return true;
        else
                return false;
}

bool EgcSubindNodeIter::hasPrevious(void) const
{
        if (m_prevId != -1)
                return true;
        else
                return false;
}

int EgcSubindNodeIter::next(void)
{
        int retval = m_nextId;
        m_histId = retval;

        m_prevId = m_nextId;
        m_nextId++;

        if (m_nextId >= m_node->nrSubindexes())
                m_nextId = -1;

        return retval;
}

int EgcSubindNodeIter::previous(void)
{
        int retval = m_prevId;
        m_histId = retval;

        m_nextId = m_prevId;
        m_prevId--;

        if (m_prevId < 0)
                m_prevId = -1;

        return retval;
}

int EgcSubindNodeIter::peekNext(void) const
{
        return m_nextId;
}

int EgcSubindNodeIter::peekPrevious(void) const
{
        return m_prevId;
}

void EgcSubindNodeIter::toBack(void)
{
        m_histId = -1;
        int indexes = m_node->nrSubindexes();

        m_prevId = indexes - 1;
        m_nextId = -1;

        if (m_prevId < 0)
                m_prevId = -1;
}

void EgcSubindNodeIter::toFront(void)
{
        m_histId = -1;
        int indexes = m_node->nrSubindexes();

        m_prevId = -1;
        m_prevId = 0;

        if (indexes <= 0)
                m_prevId = -1;
}

int EgcSubindNodeIter::lastSubind(void)
{
        return m_histId;
}

bool EgcSubindNodeIter::remove(bool before)
{
        bool retval = false;

        if (m_node->getNodeType() == EgcNodeType::NumberNode) {
                int pos = -1;
                if (before)
                        pos = m_prevId;
                else
                        pos = m_nextId;
                if (retval != -1)
                        retval = static_cast<EgcNumberNode*>(m_node)->remove(pos);
        }

        if (retval) {
                if (before) {
                        if (hasPrevious())
                                previous();
                }
        }

        return retval;
}

bool EgcSubindNodeIter::insert(QChar character)
{
        bool retval = false;

        if (m_node->getNodeType() == EgcNodeType::NumberNode) {
                int pos = -1;
                if (m_nextId != -1)
                        pos = m_nextId;
                else if (m_prevId != -1)
                        pos = m_prevId + 1;
                if (pos != -1)
                        retval = static_cast<EgcNumberNode*>(m_node)->insert(character, pos);
        }

        if (retval) {
                if (hasNext())
                        next();
                else
                        toBack();
        }

        return retval;
}
