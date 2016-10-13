/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#include "egcbinaryoperator.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"
#include "structural/concreteNodes/egcparenthesisnode.h"
#include "structural/entities/egcformulaentity.h"

EgcBinaryOperator::EgcBinaryOperator()
{
        allocReorderingProtector();
}

bool EgcBinaryOperator::setChild(quint32 index, EgcNode& expression)
{
        bool retval = false;

        if (index == 0) {
                if (!m_leftChild.isNull()) {
                        if (    isReorderingProtector(*m_leftChild)
                             && !isReorderingProtector(expression)) {
                                retval = true;
                                static_cast<EgcParenthesisNode*>(m_leftChild.data())->setChild(0, expression);
                        } else if (    isReorderingProtector(*m_leftChild)
                                    && isReorderingProtector(expression)) {
                                m_leftChild.reset(); //child will be set below
                        }
                }
        } else if (index == 1) {
                if (!m_rightChild.isNull()) {
                        if (    isReorderingProtector(*m_rightChild)
                             && !isReorderingProtector(expression)) {
                                retval = true;
                                static_cast<EgcParenthesisNode*>(m_rightChild.data())->setChild(0, expression);
                        } else if (    isReorderingProtector(*m_rightChild)
                                    && isReorderingProtector(expression)) {
                                m_rightChild.reset(); //child will be set below
                        }
                }
        }

        if (!retval) {
                return EgcBinaryNode::setChild(index, expression);
        }

        return retval;
}

bool EgcBinaryOperator::allocReorderingProtector(void)
{
        EgcBinaryOperator::ReordProtectSide prot = getReordProtectSide();

        if (static_cast<int>(prot) & static_cast<int>(ReordProtectSide::leftProtector) ) { //left side
                QScopedPointer<EgcParenthesisNode> reorder(static_cast<EgcParenthesisNode*>
                                                           (EgcNodeCreator::create(EgcNodeType::ParenthesisNode)));
                if (!reorder.isNull()) {
                        reorder->setVisible(false);
                        EgcBinaryNode::setChild(0, *reorder.take());
                }
        }
        if (static_cast<int>(prot) & static_cast<int>(ReordProtectSide::rightProtector)) { //right side
                QScopedPointer<EgcParenthesisNode> reorder(static_cast<EgcParenthesisNode*>
                                                              (EgcNodeCreator::create(EgcNodeType::ParenthesisNode)));
                if (!reorder.isNull()) {
                        reorder->setVisible(false);
                        EgcBinaryNode::setChild(1, static_cast<EgcParenthesisNode&>(*reorder.take()));
                }
        }
}

bool EgcBinaryOperator::hasReorderingProtector(quint32 index) const
{
        bool retval = false;

        if (index == 0) {
                if (!m_leftChild.isNull()) {
                        if (isReorderingProtector(*m_leftChild)) {
                                retval = true;
                        }
                }
        } else if (index == 1) {
                if (!m_rightChild.isNull()) {
                        if (isReorderingProtector(*m_rightChild)) {
                                retval = true;
                        }
                }
        }

        return retval;
}

bool EgcBinaryOperator::isLeftAssociative(void) const
{
        return true;
}

EgcBinaryOperator::ReordProtectSide EgcBinaryOperator::getReordProtectSide(void) const
{
        return ReordProtectSide::none;
}

bool EgcBinaryOperator::isReorderingProtector(EgcNode& node)
{
        if (node.getNodeType() == EgcNodeType::ParenthesisNode) {
                if (!static_cast<EgcParenthesisNode&>(node).isVisible())
                        return true;
        }

        return false;
}
