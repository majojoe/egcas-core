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
#include "../egcnodes.h"
#include "egcmaximavisitor.h"
#include "../entities/egcformulaentity.h"

EgcMaximaVisitor::EgcMaximaVisitor(EgcFormulaEntity& formula) : EgcNodeVisitor(formula)
{
}

void EgcMaximaVisitor::visit(EgcBinaryNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")^(1/";
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")+(";
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")-(";
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")*(";
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")/(";
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")^(";
                break;
        case EgcNodeType::EqualNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        str = "=";
                break;
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        str = ":";
                break;

        default:
                qDebug("No visitor code for maxima defined for this type: %d", binary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMaximaVisitor::visit(EgcUnaryNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "(";
                else
                        str = ")";
                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "-(";
                else
                        str = ")";
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", unary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMaximaVisitor::visit(EgcFlexNode* flex)
{
        QString str;

        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = static_cast<EgcFunctionNode*>(flex)->getName() % "(";
                else if (m_state == EgcIteratorState::RightIteration)
                        str = ")";
                else
                        str = ",";
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", flex->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMaximaVisitor::visit(EgcNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                str = static_cast<EgcNumberNode*>(node)->getValue();
                break;
        case EgcNodeType::VariableNode:
                str = static_cast<EgcVariableNode*>(node)->getValue();
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", node->getNodeType()) ;
                break;
        }

        m_result += str;
}

QString EgcMaximaVisitor::getResult(void)
{
        QString tmp = EgcNodeVisitor::getResult();

        if (m_formula) {
                if (m_formula->isNumberResult())
                {
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
                        tmp += QString("ffprintprec:%1$").arg(EgcFormulaEntity::getStdNrSignificantDigis());
                }
        }

        return tmp + ";\n";
}

