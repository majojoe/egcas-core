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
        bool isReorder = true;  //expression is reordering parenthesis
        QScopedPointer<EgcNode> *child;

        if (index > 1)
                return false;

        if (index == 0)
                child = &m_leftChild;
        else if (index == 1)
                child = &m_rightChild;

        if (!child->isNull()) {
                if (    isReorderingProtector(*(*child))
                                && !isReorderingProtector(expression)) {
                        isReorder = false;
                        static_cast<EgcParenthesisNode*>(child->data())->setChild(0, expression);
                } else if (    isReorderingProtector(*(*child))
                               && isReorderingProtector(expression)) {
                        child->reset(); //child will be set below
                }
        } else if (isReorderingProtector(expression)) { //make the reordering protector visible
                if (expression.getNodeType() == EgcNodeType::ParenthesisNode)
                        static_cast<EgcParenthesisNode&>(expression).setVisible(true);
        }

        if (isReorder) {
                return EgcBinaryNode::setChild(index, expression);
        }

        return false;
}

void EgcBinaryOperator::allocReorderProtChild(quint32 index)
{
        if (index > 1)
                return;

        QScopedPointer<EgcParenthesisNode> reorder(static_cast<EgcParenthesisNode*>
                                                   (EgcNodeCreator::create(EgcNodeType::ParenthesisNode)));
        if (!reorder.isNull()) {
                reorder->setVisible(false);
                EgcBinaryNode::setChild(index, *reorder.take());
        }
}

void EgcBinaryOperator::allocReorderingProtector(void)
{
        EgcBinaryOperator::ReordProtectSide prot = getReordProtectSide();

        if (static_cast<int>(prot) & static_cast<int>(ReordProtectSide::leftProtector) )  //left side
                allocReorderProtChild(0);
        if (static_cast<int>(prot) & static_cast<int>(ReordProtectSide::rightProtector))  //right side
                allocReorderProtChild(1);
}

bool EgcBinaryOperator::hasReorderingProtector(quint32 index) const
{
        bool retval = false;
        const QScopedPointer<EgcNode> *child;

        if (index > 1)
                return false;

        if (index == 0)
                child = &m_leftChild;
        else if (index == 1)
                child = &m_rightChild;

        if (!child->isNull()) {
                if (isReorderingProtector(*(*child))) {
                        retval = true;
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
