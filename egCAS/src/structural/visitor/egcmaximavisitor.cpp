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
#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include <QDebug>
#endif //#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include "../egcnodes.h"
#include "egcmaximavisitor.h"
#include "../entities/egcformulaentity.h"

EgcMaximaVisitor::EgcMaximaVisitor(EgcFormulaEntity& formula) : EgcNodeVisitor(formula)
{
}

void EgcMaximaVisitor::visit(EgcBinaryNode* binary)
{
        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        bool sqrt = false;
                        EgcNode *rootexp = binary->getChild(0);
                        if (rootexp) {
                                if (rootexp->getNodeType() == EgcNodeType::EmptyNode)
                                        sqrt = true;
                        }
                        if (sqrt)
                                assembleResult("(%2)^(1/(2%1))", binary); //%1 is a hack here and just works since an empty node has no signs (must be fixed)
                        else
                                assembleResult("(%2)^(1/%1)", binary);
                }
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)+(%2)", binary);
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)-(%2)", binary);
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)*(%2)", binary);
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)/(%2)", binary);
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)^(%2)", binary);
                break;
        case EgcNodeType::EqualNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        assembleResult("%1", binary);
                }
                break;
        }
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1:%2", binary);
                break;
        case EgcNodeType::BinEmptyNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)()(%2)", binary);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(binary->getNodeType())) ;
                break;
        }
}

void EgcMaximaVisitor::visit(EgcUnaryNode* unary)
{
        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)", unary);
                break;
        case EgcNodeType::LParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1", unary);
                break;
        case EgcNodeType::RParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1)", unary);
                break;
        case EgcNodeType::LogNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(log(%1)/log(10))", unary);
                break;
        case EgcNodeType::NatLogNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("log(%1)", unary);
                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("-(%1)", unary);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(unary->getNodeType())) ;
                break;
        }
}

void EgcMaximaVisitor::visit(EgcFlexNode* flex)
{
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult(static_cast<EgcFunctionNode*>(flex)->getStuffedName() % "(", ",", ")", flex);
                break;
        case EgcNodeType::IntegralNode:
                if (flex->getNumberChildNodes() == 2) { // indefinite integral
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("integrate(", ",", ")", flex);
                } else if (flex->getNumberChildNodes() == 4) {
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("romberg(%3,%4,%1,%2)", flex);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(flex);

                        QString str = "diff(%" % QString::number(2) % ",%" % QString::number(3)
                                      % "," % QString::number(diff->getNrDerivative()) % ")";
                        assembleResult(str, flex);
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(flex->getNodeType())) ;
                break;
        }
}

void EgcMaximaVisitor::visit(EgcNode* node)
{
        switch (node->getNodeType()) {
        case EgcNodeType::EmptyNode:
                pushToStack(QString::null, node);
                break;
        case EgcNodeType::AlnumNode:  // normally we extract the AlnumNode's via their container classes
                pushToStack(static_cast<EgcAlnumNode*>(node)->getStuffedValue(), node);
                break;
        case EgcNodeType::VariableNode:  // normally we extract the AlnumNode's via their container classes
                pushToStack(static_cast<EgcVariableNode*>(node)->getStuffedValue(), node);
                break;
        case EgcNodeType::NumberNode:
                pushToStack(static_cast<EgcNumberNode*>(node)->getValue(), node);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

QString EgcMaximaVisitor::getResult(void)
{
        m_suppressList.clear();
        QString tmp = EgcNodeVisitor::getResult();

        if (m_formula) {
                quint8 nrDigits = m_formula->getNumberOfSignificantDigits();
                if (nrDigits == 1 || nrDigits > 16)
                        nrDigits = 0;

                // use standard digits if nothing is set for the formula
                if (nrDigits == 0)
                        nrDigits = EgcFormulaEntity::getStdNrSignificantDigis();

                QString tmpOptions = QString("fpprintprec:%1$").arg(nrDigits);

                switch(m_formula->getNumberResultType()) {
                case EgcNumberResultType::IntegerType:
                        tmp = tmpOptions + "round(" + tmp + ")";
                        tmp += QString(";");
                        break;
                case EgcNumberResultType::ScientificType:
                        tmp = tmpOptions + "float(" + tmp + ")";
                        tmp += QString(";");
                        break;
                case EgcNumberResultType::EngineeringType:
                        tmp = tmpOptions + "engineering_format_floats:true$float(" + tmp + ");engineering_format_floats:false$";
                        break;
                default:
                        tmp = tmpOptions + tmp;
                        tmp += QString(";");
                        break;
                }
        }

#ifdef DEBUG_KERNEL_COMMAND_GENERATION
        qDebug() << "kernel command output of visitor: " << tmp;
#endif //#ifdef DEBUG_KERNEL_COMMAND_GENERATION

        return tmp;
}

void EgcMaximaVisitor::suppressCurrentIfChildType(const EgcNode* node, quint32 index, EgcNodeType type)
{
        if (!node)
                return;

        if (!node->isContainer())
                return;

        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(node);
        if (container->getChild(index)) {
                EgcNode* chldNode = container->getChild(index);
                if (chldNode->getNodeType() == type) {
                        m_suppressList.insert(const_cast<EgcNode*>(node));
                }
        }
}
