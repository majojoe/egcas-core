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
#include "iterator/screenHelpers/egcidnodeiter.h"
#include "iterator/screenHelpers/egcsubidnodeiter.h"

EgcScrPosIterator::EgcScrPosIterator(const EgcFormulaEntity& formula) : m_lookup{formula.getMathmlMappingCRef()},
                                                                        m_nodeIter{new EgcIdNodeIter(formula)},
                                                                        m_subIdIter{new EgcSubidNodeIter(m_nodeIter->previous())}
{

}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
}

bool EgcScrPosIterator::hasPrevious(void) const
{
}

const quint32 EgcScrPosIterator::next(void)
{
}

const quint32 EgcScrPosIterator::previous(void)
{
}

const quint32 EgcScrPosIterator::peekNext(void) const
{
}

const quint32 EgcScrPosIterator::peekPrevious(void) const
{
}

void EgcScrPosIterator::toBack(void)
{
}

void EgcScrPosIterator::toFront(void)
{
}

const EgcNode* EgcScrPosIterator::node(void)
{
}

bool EgcScrPosIterator::rightSide(void)
{
//        return m_rightSide;
}

quint32 EgcScrPosIterator::subIndex(void)
{
}

quint32 EgcScrPosIterator::id(void)
{

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

