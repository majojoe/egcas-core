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

#include "egcdivisionnode.h"
#include "structural/egcnodecreator.h"
#include "structural/concreteNodes/egcparenthesisnode.h"

EgcDivisionNode::EgcDivisionNode()
{
        QScopedPointer<EgcNode> parenthesis(EgcNodeCreator::create(EgcNodeType::ParenthesisNode));
        if (!parenthesis.isNull()) {
                static_cast<EgcParenthesisNode*>(parenthesis.data())->setVisible(false);
                EgcBinaryNode::setChild(0, static_cast<EgcParenthesisNode&>(*parenthesis.take()));
        }
        parenthesis.reset(EgcNodeCreator::create(EgcNodeType::ParenthesisNode));
        if (!parenthesis.isNull()) {
                static_cast<EgcParenthesisNode*>(parenthesis.data())->setVisible(false);
                EgcBinaryNode::setChild(1, static_cast<EgcParenthesisNode&>(*parenthesis.take()));
        }
}

bool EgcDivisionNode::cursorSnaps(EgcNodeSide side) const
{
        if (   side == EgcNodeSide::left
            || side == EgcNodeSide::right)
                return true;

        return false;
}

bool EgcDivisionNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = false;

        QScopedPointer<const EgcNode> expr(&expression);

        if (!m_leftChild.isNull() && !m_rightChild.isNull()) {
                if (    m_leftChild->getNodeType() == EgcNodeType::ParenthesisNode
                     && m_rightChild->getNodeType() == EgcNodeType::ParenthesisNode) {
                        if (    !static_cast<EgcParenthesisNode*>(m_leftChild.data())->isVisible()
                             && !static_cast<EgcParenthesisNode*>(m_rightChild.data())->isVisible())
                                retval = true;
                }
        }

        if (!retval)
                return EgcBinaryNode::setChild(index, expression);


        if (index == 0) {
                static_cast<EgcParenthesisNode*>(m_leftChild.data())->setChild(0, *expr.take());
        } else if (index == 1) {
                static_cast<EgcParenthesisNode*>(m_rightChild.data())->setChild(0, *expr.take());
        } else {
                retval = false;
        }

        return retval;
}
