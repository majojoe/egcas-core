/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <QStringBuilder>
#include "../egcnodes.h"
#include "egcmathmlvisitor.h"

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaExpression& formula) : EgcNodeVisitor(formula)
{
}

void EgcMathMlVisitor::visit(EgcBinaryNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mroot><mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "</mrow><mrow>";
                else
                        str = "</mrow></mroot>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", binary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMathMlVisitor::visit(EgcUnaryNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mfenced open=\"(\" close=\")\" separators=\",\"><mrow>";
                else
                        str = "</mrow></mfenced>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", unary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMathMlVisitor::visit(EgcNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                str = "<mn>" + static_cast<EgcNumberNode*>(node)->getValue() + "</mn>";
                break;
        case EgcNodeType::VariableNode:
                str = "<mi>" + static_cast<EgcVariableNode*>(node)->getValue() + "</mi>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", node->getNodeType()) ;
                break;
        }

        m_result += str;
}

QString EgcMathMlVisitor::getResult(void)
{
        QString temp;

        temp = "<math>";
        temp += EgcNodeVisitor::getResult();
        temp += "</math>";

        return temp;
}
