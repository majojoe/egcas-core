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

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaEntity& formula) : EgcNodeVisitor{formula}, m_prettyPrint{true},
                                                                m_idCounter{1}
{
        m_lookup.clear();
}

void EgcMathMlVisitor::visit(EgcBinaryNode* binary)
{       
        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:                
                if (m_state == EgcIteratorState::LeftIteration) {
                        // don't show the root exponent if it is "2" (square root)
                        suppressIfChildValue(binary, 1, EgcNodeType::NumberNode, "2");
                } else if (m_state == EgcIteratorState::RightIteration) {
                        assembleResult("<mroot" % getId(binary) % "><mrow>%1</mrow><mrow>%2</mrow></mroot>", binary);
                }
                break;
        case EgcNodeType::EqualNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow>%1<mo" % getId(binary) % ">=</mo>%2</mrow>", binary);
                break;
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow>%1<mo" % getId(binary) % ">:=</mo>%2</mrow>", binary);
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow>%1<mo" % getId(binary) % ">+</mo>%2</mrow>", binary);
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow>%1<mo" % getId(binary) % ">-</mo>%2</mrow>", binary);
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow>%1<mo" % getId(binary) % ">&CenterDot;</mo>%2</mrow>", binary);
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        suppressIfChildType(binary, 0, EgcNodeType::ParenthesisNode);
                        suppressIfChildType(binary, 1, EgcNodeType::ParenthesisNode);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        assembleResult("<mfrac" % getId(binary) % ">%1 %2</mfrac>", binary);
                }
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<msup" % getId(binary) % ">%1 %2</msup>", binary);
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", binary->getNodeType()) ;
                break;
        }
}

void EgcMathMlVisitor::visit(EgcUnaryNode* unary)
{
        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode: {
                /*this is to remove unneccesary parenthesis in case of redundant nested parenthesis*/
                suppressIfChildType(unary, 0, EgcNodeType::ParenthesisNode);

                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mfenced " % getId(unary) % "open=\"(\" close=\")\" separators=\",\"><mrow>%1</mrow></mfenced>", unary);
        }
        break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("<mrow><mo" % getId(unary) % ">-</mo>%1</mrow>", unary);
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", unary->getNodeType()) ;
                break;
        }
}

void EgcMathMlVisitor::visit(EgcFlexNode* flex)
{
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        QString startStr = QString("<mrow" % getId(flex) % "><mi>%1</mi><mo>&ApplyFunction;</mo><mrow><mo>(</mo><mrow>")
                                      .arg(static_cast<EgcFunctionNode*>(flex)->getName());
                        assembleResult(startStr, "<mo>,</mo>", "</mrow><mo>)</mo></mrow></mrow>", flex);
                }
                break;
        case EgcNodeType::IntegralNode:
                if (flex->getNumberChildNodes() == 2) { // indefinite integral
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("<mrow" % getId(flex) % "><mstyle scriptlevel=\"-1\"><mo>&Integral;</mo></mstyle><mrow>%1</mrow><mo>d</mo><mrow>%2</mrow></mrow>", flex);
                } else if (flex->getNumberChildNodes() == 4) { // integral with limits number of childs should be 4!
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("<mrow" % getId(flex) % "><munderover><mstyle scriptlevel=\"-1\"><mo>&Integral;</mo></mstyle><mrow>%3</mrow><mrow>%4</mrow></munderover><mrow>%1</mrow><mo>d</mo><mrow>%2</mrow></mrow>", flex);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(flex);
                        quint8 der = diff->getNrDerivative();
                        quint32 indexD = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::differential);
                        quint32 indexV = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::variable);

                        if (    diff->getChild(indexD)->getNodeType() == EgcNodeType::VariableNode
                             && der < 4) { // use lagrange's notation
                                QString derivative;
                                switch (der) {
                                case 2:
                                        derivative = "&Prime;";
                                        break;
                                case 3:
                                        derivative = "&tprime;";
                                        break;
                                case 4:
                                        derivative = "&qprime;";
                                        break;
                                default:
                                        derivative = "&prime;";
                                        break;
                                }

                                derivative = "<mstyle scriptlevel=\"-1\"><mo>" % derivative % "</mo></mstyle>";
                                derivative = "<mrow" % getId(flex) % "><msup>%" % QString::number(indexD + 1) % derivative
                                             % "</msup><mfenced>%" % QString::number(indexV + 1) % "</mfenced></mrow>";
                                assembleResult(derivative, flex);
                        } else { // use leibniz' notation
                                QString result;
                                if (der == 1)
                                        result = "<mfrac" % getId(flex) % "><mrow><mi>d</mi><mfenced>%" % QString::number(indexD + 1)
                                                 % "</mfenced></mrow><mrow><mi>d</mi>%" % QString::number(indexV + 1)
                                                 % "</mrow></mfrac>";
                                else
                                        result = "<mfrac" % getId(flex) % "><mrow><msup><mi>d</mi><mn>" % QString::number(der)
                                                 % "</mn></msup><mfenced>%" % QString::number(indexD + 1)
                                                 % "</mfenced></mrow><msup><mrow><mi>d</mi>%"
                                                 % QString::number(indexV + 1) % "</mrow><mn>"
                                                 % QString::number(der) % "</mn></msup></mfrac>";

                                assembleResult(result, flex);
                        }
                }
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", flex->getNodeType()) ;
                break;
        }
}

void EgcMathMlVisitor::visit(EgcNode* node)
{
        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                pushToStack("<mn" % getId(node) % ">" % static_cast<EgcNumberNode*>(node)->getValue() % "</mn>", node);
                break;
        case EgcNodeType::VariableNode: {
                EgcVariableNode* varNode = static_cast<EgcVariableNode*>(node);
                if (varNode->getSubscript().isEmpty())
                        pushToStack("<mi" % getId(node) % ">" % varNode->getValue() % "</mi>", node);
                else
                        pushToStack("<msub" % getId(node) % "><mi>" % varNode->getValue() % "</mi><mi>" % varNode->getSubscript()
                                    %"</mi></msub>", node);
                break;
        }
        case EgcNodeType::EmptyNode:
                pushToStack("<mi" % getId(node) % ">&#x2B1A;</mi>", node);
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", node->getNodeType()) ;
                break;
        }
}

QString EgcMathMlVisitor::getResult(void)
{
        //do clean up
        m_idCounter = 1;
        m_lookup.clear();

        QString temp;
        //clear suppress list from last run
        m_suppressList.clear();

        temp = "<math>";
        temp += EgcNodeVisitor::getResult();
                
        if (m_formula) {
                if (!m_formula->getErrorMessage().isEmpty()) {
                        temp += "<mi mathcolor='#cc0000'>";
#ifdef EGC_PROJ_NAME
#warning replace '\n' with mathml linebreaks
#endif //#ifdef EGC_PROJ_NAME
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

QString EgcMathMlVisitor::getId(EgcNode* node)
{
        QString str(" id=\"%1\" ");
        str = str.arg(m_idCounter);
        m_lookup.insert(m_idCounter, node);
        m_idCounter++;

        return str;
}

QHash<quint32, EgcNode*> EgcMathMlVisitor::getMathmlMapping(void)
{
        return m_lookup;
}
