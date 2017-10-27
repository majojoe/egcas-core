/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
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
#include "formulascrvector.h"
#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include <QDebug>
#endif //#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include "../egcnodes.h"
#include "formulascrvisitor.h"
#include "../entities/egcformulaentity.h"
#include <iterator/egcscrpositerator.h>
#include <QStringBuilder>



FormulaScrVisitor::FormulaScrVisitor(EgcFormulaEntity& formula, QVector<FormulaScrElement>& vector) :  m_formula{&formula}, m_childIndex{0}, m_vector{&vector}, m_id{0}
{
        if (m_formula->getIterator())
                m_restructureData = m_formula->getIterator()->getRestructureData();
}

void FormulaScrVisitor::visit(EgcBinaryNode* binary)
{
        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("_root(_{%1_},%2)", binary);
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1+%2", binary);
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1-%2", binary);
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1*%2", binary);
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("_{%1_}/_{%2_}", binary);
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1^_{%2_}", binary);
                break;
        case EgcNodeType::EqualNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        assembleResult("%1=%2", binary);
                }
                break;
        }
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1:%2", binary);
                break;
        case EgcNodeType::BinEmptyNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1()%2", binary);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(binary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcUnaryNode* unary)
{
        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1)", unary);
                break;
        case EgcNodeType::LogNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("_log(%1)", unary);
                break;
        case EgcNodeType::NatLogNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("log(%1)", unary);
                break;
        case EgcNodeType::LParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("(%1", unary);
                break;
        case EgcNodeType::RParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("%1)", unary);
                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("-%1", unary);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(unary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcFlexNode* flex)
{
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::RightIteration)
                        assembleResult("", "(", ",", ")", flex);
                break;
        case EgcNodeType::IntegralNode:
                if (m_state == EgcIteratorState::RightIteration) {
                                assembleResult("_integrate(", ",", ")", flex);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(flex);
                        quint32 indexD = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::differential);
                        quint32 indexV = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::variable);

                        QString str = "_diff(%" % QString::number(indexD + 1) % ",%" % QString::number(indexV + 1)
                                      % "," % QString::number(diff->getNrDerivative()) % ")";
                        assembleResult(str, flex);
                }
                break;
        case EgcNodeType::VariableNode:
                if (m_state == EgcIteratorState::RightIteration) { //there are no subsequent nodes but the Alnum nodes -> so push to stack
                        if (flex->getNumberChildNodes() == 2) {
                                EgcVariableNode *var = static_cast<EgcVariableNode*>(flex);
                                QString str = "%1" % var->getStuffedVarSeparator() % "%2";
                                assembleResult(str, flex);
                        } else if (flex->getNumberChildNodes() == 1) {
                               assembleResult("%1", flex);
                        }
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(flex->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcNode* node)
{
        switch (node->getNodeType()) {
        case EgcNodeType::EmptyNode:
                append("_empty", node);
                break;
        case EgcNodeType::AlnumNode:  // normally we extract the AlnumNode's via their container classes
                QString val = static_cast<EgcAlnumNode*>(node)->getValue();
                QString i;
                foreach (i, val) {
                        append(EgcAlnumNode::encode(i), node);
                }
                break;
        case EgcNodeType::NumberNode:
                QString val = static_cast<EgcNumberNode*>(node)->getValue();
                QString i;
                foreach (i, val) {
                        append(i, node);
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::append(QString str, EgcNode* node)
{
        FormulaScrElement el;
        el.m_id = m_id++;
        el.m_node = node;
        el.m_value = str;
        m_vector->append(el);
}

QString FormulaScrVisitor::getResult(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;

        m_id = 0;

        while(iter.hasNext()) {
                previousChildNode = &iter.peekPrevious();
                node = &iter.next();
                m_state = iter.getLastState();
                if (node->isContainer()) {
                        if (m_state == EgcIteratorState::LeftIteration) {
                                m_childIndex = 0;
                        } else if (   m_state == EgcIteratorState::RightIteration) {
                                m_childIndex = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();
                        } else {
                                quint32 ind;
                                if (static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousChildNode, ind))
                                      m_childIndex = ind;
                                else
                                      m_childIndex = 0;
                        }
                } else {
                        m_childIndex = 0;
                }
                node->accept(this);
        };

        QString result;
        FormulaScrElement i;
        foreach (i, m_vector) {
                result %= i.m_value;
        }

        return result;
}

