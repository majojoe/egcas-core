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
}

bool EgcBinaryOperator::setChild(quint32 index, EgcNode& expression)
{
        bool retval = false;

        if (index == 0) {
                if (!m_leftChild.isNull()) {
                        if (m_leftChild->getNodeType() == EgcNodeType::ParenthesisNode) {
                                if (!static_cast<EgcParenthesisNode*>(m_leftChild.data())->isVisible()) {
                                        retval = true;
                                        static_cast<EgcParenthesisNode*>(m_leftChild.data())->setChild(0, expression);
                                }
                        }
                }
        } else if (index == 1) {
                if (!m_rightChild.isNull()) {
                        if (m_rightChild->getNodeType() == EgcNodeType::ParenthesisNode) {
                                if (!static_cast<EgcParenthesisNode*>(m_rightChild.data())->isVisible()) {
                                        retval = true;
                                        static_cast<EgcParenthesisNode*>(m_rightChild.data())->setChild(0, expression);
                                }
                        }
                }
        }

        if (!retval) {
                return EgcBinaryNode::setChild(index, expression);
        }

        return retval;
}

bool EgcBinaryOperator::allocReorderingProtector(bool left, bool right)
{
        if (left) {
                QScopedPointer<EgcNode> parenthesis(EgcNodeCreator::create(EgcNodeType::ParenthesisNode));
                if (!parenthesis.isNull()) {
                        static_cast<EgcParenthesisNode*>(parenthesis.data())->setVisible(false);
                        EgcBinaryNode::setChild(0, static_cast<EgcParenthesisNode&>(*parenthesis.take()));
                }
        }
        if (right) {
                QScopedPointer<EgcNode> parenthesis(EgcNodeCreator::create(EgcNodeType::ParenthesisNode));
                if (!parenthesis.isNull()) {
                        static_cast<EgcParenthesisNode*>(parenthesis.data())->setVisible(false);
                        EgcBinaryNode::setChild(1, static_cast<EgcParenthesisNode&>(*parenthesis.take()));
                }
        }
}

bool EgcBinaryOperator::hasReorderingProtector(quint32 index) const
{
        bool retval = false;

        if (index == 0) {
                if (!m_leftChild.isNull()) {
                        if (m_leftChild->getNodeType() == EgcNodeType::ParenthesisNode) {
                                if (!static_cast<EgcParenthesisNode*>(m_leftChild.data())->isVisible()) {
                                        retval = true;
                                }
                        }
                }
        } else if (index == 1) {
                if (!m_rightChild.isNull()) {
                        if (m_rightChild->getNodeType() == EgcNodeType::ParenthesisNode) {
                                if (!static_cast<EgcParenthesisNode*>(m_rightChild.data())->isVisible()) {
                                        retval = true;
                                }
                        }
                }
        }

        return retval;
}
