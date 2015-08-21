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

#include <QStringBuilder>
#include <entities/egcformulaentity.h>
#include "../egcnodes.h"
#include "egcmathmlvisitor.h"

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaEntity& formula) : EgcNodeVisitor{formula}, m_prettyPrint{true}
{
}

void EgcMathMlVisitor::visit(EgcBinaryNode* binary)
{       
        QString str;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:                
                if (m_state == EgcIteratorState::LeftIteration) {
                        str = "<mroot><mrow>";
                        // don't show the root exponent if it is "2" (square root)
                        suppressIfChildValue(binary, 1, EgcNodeType::NumberNode, "2");
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        str = "</mrow><mrow>";
                } else {
                        str = "</mrow></mroot>";
                }
                break;
        case EgcNodeType::EqualNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "<mo>=</mo>";
                else
                        str = "</mrow>";
                break;
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "<mo>:=</mo>";
                else
                        str = "</mrow>";
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "<mo>+</mo>";
                else
                        str = "</mrow>";
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "<mo>-</mo>";
                else
                        str = "</mrow>";
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
                        str = "<mo>&times;</mo>";
                else
                        str = "</mrow>";
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        str = "<mfrac>";
                        suppressIfChildType(binary, 0, EgcNodeType::ParenthesisNode);
                        suppressIfChildType(binary, 1, EgcNodeType::ParenthesisNode);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        str = "</mfrac>";
                }
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<msup>";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = "</msup>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", binary->getNodeType()) ;
                break;
        }

        if (!m_suppressList.contains(binary))
                m_result += str;
}

void EgcMathMlVisitor::visit(EgcUnaryNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode: {
                /*this is to remove unneccesary parenthesis in case of redundant nested parenthesis*/
                suppressIfChildType(unary, 0, EgcNodeType::ParenthesisNode);

                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mfenced open=\"(\" close=\")\" separators=\",\"><mrow>";
                else
                        str = "</mrow></mfenced>";
        }
        break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mrow><mo>-</mo>";
                else
                        str = "</mrow>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", unary->getNodeType()) ;
                break;
        }

        if (!m_suppressList.contains(unary))
                m_result += str;
}

void EgcMathMlVisitor::visit(EgcFlexNode* flex)
{
        QString str;

        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = QString("<mrow><mi>%1</mi><mo>&ApplyFunction;</mo><mrow><mo>(</mo><mrow>")
                                      .arg(static_cast<EgcFunctionNode*>(flex)->getName());
                else if (m_state == EgcIteratorState::RightIteration)
                        str = "</mrow><mo>)</mo></mrow></mrow>";
                else
                        str = "<mo>,</mo>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", flex->getNodeType()) ;
                break;
        }

        if (!m_suppressList.contains(flex))
                m_result += str;
}

void EgcMathMlVisitor::visit(EgcNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                str = "<mn>" % static_cast<EgcNumberNode*>(node)->getValue() % "</mn>";
                break;
        case EgcNodeType::VariableNode: {
                EgcVariableNode* varNode = static_cast<EgcVariableNode*>(node);
                if (varNode->getSubscript().isEmpty())
                        str = "<mi>" % varNode->getValue() % "</mi>";
                else
                        str = "<msub><mi>" % varNode->getValue() % "</mi><mi>" % varNode->getSubscript() %"</mi></msub>";
                break;
        }
        case EgcNodeType::EmptyNode:
                str = "<mi>&#x2B1A;</mi>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", node->getNodeType()) ;
                break;
        }

        if (!m_suppressList.contains(node))
                m_result += str;
}

QString EgcMathMlVisitor::getResult(void)
{
        QString temp;
        //clear suppress list from last run
        m_suppressList.clear();

        temp = "<math>";
        temp += EgcNodeVisitor::getResult();
                
        if (m_formula) {
                if (!m_formula->getErrorMessage().isEmpty()) {
                        temp += "<mi mathcolor='#cc0000'>";
#warning replace '\n' with mathml linebreaks
                        //temp += "<mspace linebreak='newline'/>";
                        //temp += "<mstyle scriptlevel=\"-1\">";
                        temp += m_formula->getErrorMessage();
                        //temp += "</mstyle>";
                        temp += "</mi>";
                }
        }
        temp += "</math>";

        return temp;
}

void EgcMathMlVisitor::setPrettyPrint(bool prettyPrint)
{
        m_prettyPrint = prettyPrint;
}

bool EgcMathMlVisitor::prettyPrint(void)
{
        return m_prettyPrint;
}

void EgcMathMlVisitor::suppressIfChildType(const EgcNode* node, quint32 index, EgcNodeType type)
{
        if (!node)
                return;

        if (!m_prettyPrint)
                return;

        if (!node->isContainer())
                return;

        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(node);
        if (container->getChild(index)) {
                EgcNode* chldNode = container->getChild(index);
                if (chldNode->getNodeType() == type) {
                        m_suppressList.insert(chldNode);
                }
        }
}

void EgcMathMlVisitor::suppressIfChildValue(const EgcNode* node, quint32 index, EgcNodeType type, QString val)
{
        if (!node)
                return;
                
        if (!m_prettyPrint)
                return;

        if (!node->isContainer())
                return;
        
        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(node);
        if (container->getChild(index)) {                                
                EgcNode* chldNode = container->getChild(index);
                if (chldNode->getNodeType() == type) {                        
                        QString value;
                        switch (type) {
                        case EgcNodeType::NumberNode:
                                value = static_cast<EgcNumberNode*>(chldNode)->getValue();
                                break;
                        case EgcNodeType::VariableNode:
                                value = static_cast<EgcVariableNode*>(chldNode)->getValue();
                                break;
                        }

                        if (value == val)
                                m_suppressList.insert(chldNode);
                }                                
        }        
}
