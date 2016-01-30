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
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)^(1/%2)", binary);
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
        case EgcNodeType::VariableNode:
                if (m_state == EgcIteratorState::RightIteration) { //there are no subsequent nodes but the Alnum nodes -> so push to stack
                        deleteFromStack(2);
                        pushToStack(static_cast<EgcVariableNode*>(binary)->getStuffedVar(), binary);
                }
                break;


        default:
                qDebug("No visitor code for maxima defined for this type: %d", binary->getNodeType()) ;
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
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("-(%1)", unary);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", unary->getNodeType()) ;
                break;
        }
}

void EgcMaximaVisitor::visit(EgcFlexNode* flex)
{
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult(static_cast<EgcFunctionNode*>(flex)->getName() % "(", ",", ")", flex);
                break;
        case EgcNodeType::IntegralNode:
                if (flex->getNumberChildNodes() == 2) { // indefinite integral
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("integrate(", ",", ")", flex);
                } else if (flex->getNumberChildNodes() == 4) {
                        if (m_state == EgcIteratorState::RightIteration)
                                assembleResult("romberg(", ",", ")", flex);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(flex);
                        quint32 indexD = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::differential);
                        quint32 indexV = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::variable);

                        QString str = "diff(%" % QString::number(indexD + 1) % ",%" % QString::number(indexV + 1)
                                      % "," % QString::number(diff->getNrDerivative()) % ")";
                        assembleResult(str, flex);
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", flex->getNodeType()) ;
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
                pushToStack(static_cast<EgcNumberNode*>(node)->getValue(), node);
                break;
        case EgcNodeType::NumberNode:
                pushToStack(static_cast<EgcNumberNode*>(node)->getValue(), node);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", node->getNodeType()) ;
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

                QString tmpOptions = QString("fpprintprec:%1$").arg(nrDigits);

                switch(m_formula->getNumberResultType()) {
                case EgcNumberResultType::IntegerType:
                        tmp = tmpOptions + "round(" + tmp + ")";
                        break;
                case EgcNumberResultType::ScientificType:
                        tmp = tmpOptions + "float(" + tmp + ")";
                        break;
                case EgcNumberResultType::EngineeringType:
#ifdef EGC_PROJ_NAME
#warning maxima function for engineering type is missing (load lisp function "engineering-format" from share/contrib/engineering-format.lisp)
#endif //#ifdef EGC_PROJ_NAME
                        tmp = tmpOptions + tmp;
                        break;
                default:
                        tmp = tmpOptions + tmp;
                        break;
                }
                //reset the precision again to standard type
                tmp += QString(";fpprintprec:%1$").arg(EgcFormulaEntity::getStdNrSignificantDigis());
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
