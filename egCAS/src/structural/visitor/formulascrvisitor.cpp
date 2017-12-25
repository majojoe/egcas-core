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
#include "formulascrelement.h"
#include "../entities/egcformulaentity.h"
#include "../egcnodes.h"
#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include <QDebug>
#endif //#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include "../egcnodes.h"
#include "formulascrvisitor.h"
#include "../entities/egcformulaentity.h"
#include <iterator/egcscrpositerator.h>
#include <structural/iterator/formulascriter.h>
#include <QStringBuilder>




FormulaScrVisitor::FormulaScrVisitor(EgcFormulaEntity& formula, FormulaScrIter& iter) :  EgcNodeVisitor(formula), m_iter{iter}, m_id{0}
{
}

void FormulaScrVisitor::visit(EgcBinaryNode* binary)
{
        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode: {
                EgcNode* child = binary->getChild(1);
                bool isSquaredRoot = false;
                if (child->getNodeType() == EgcNodeType::NumberNode) {
                        if (static_cast<EgcNumberNode*>(child)->getValue() == QString("2"))
                                isSquaredRoot = true;
                }
                if (m_state == EgcIteratorState::LeftIteration) {
                        if (isSquaredRoot) {
                                m_suppressList.insert(binary); //exponent of squared roots are not shown
                                appendSegmented("_root_{", binary, CursorAdhesion::normal);
                        } else {
                                appendSegmented("_root_{_{", binary, CursorAdhesion::normal);
                        }
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        if (!isSquaredRoot)
                                appendSegmented("_},", binary, CursorAdhesion::normal);
                } else {
                        appendSegmented("_}", binary, CursorAdhesion::normal);
                }
                break;
        }
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("+", binary);
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("-", binary);
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("*", binary);
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        appendSegmented("_{", binary);
                else if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("_}/_{", binary);
                else
                        appendSegmented("_}", binary);
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("^_{", binary);
                else if (m_state == EgcIteratorState::RightIteration)
                        appendSegmented("_}", binary);
                break;
        case EgcNodeType::EqualNode: {
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("=", binary);
                }
                break;
        }
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append(":", binary);
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
                if (m_state == EgcIteratorState::LeftIteration)
                        append("(", unary, CursorAdhesion::low, 1);
                else if (m_state == EgcIteratorState::RightIteration)
                        append(")", unary, CursorAdhesion::low, 2);
                break;
        case EgcNodeType::LogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("_log", unary);
                        append("(", unary);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        append(")", unary);
                }
                break;
        case EgcNodeType::NatLogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("log", unary);
                        append("(", unary);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        append(")", unary);
                }
                break;
//        case EgcNodeType::LParenthesisNode:
//                if (m_state == EgcIteratorState::RightIteration)
//                        assembleResult("(%1", unary);
//                break;
//        case EgcNodeType::RParenthesisNode:
//                if (m_state == EgcIteratorState::RightIteration)
//                        assembleResult("%1)", unary);
//                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        append("-", unary, CursorAdhesion::normal);
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
//                if (m_state == EgcIteratorState::LeftIteration)
//                        append("_{", flex);
//                else if (m_state == EgcIteratorState::MiddleIteration)
//                        append("");
//                        assembleResult("", "(", ",", ")", flex);
//                break;
//        case EgcNodeType::IntegralNode:
//                if (m_state == EgcIteratorState::RightIteration) {
//                                assembleResult("_integrate(", ",", ")", flex);
//                }
//                break;
//        case EgcNodeType::DifferentialNode:
//                if (m_state == EgcIteratorState::RightIteration) {
//                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(flex);
//                        quint32 indexD = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::differential);
//                        quint32 indexV = diff->getIndexOf(EgcDifferentialNode::EgcDifferentialIndexes::variable);

//                        QString str = "_diff(%" % QString::number(indexD + 1) % ",%" % QString::number(indexV + 1)
//                                      % "," % QString::number(diff->getNrDerivative()) % ")";
//                        assembleResult(str, flex);
//                }
//                break;
        case EgcNodeType::VariableNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        if (flex->getNumberChildNodes() == 2) {
                                EgcVariableNode *var = static_cast<EgcVariableNode*>(flex);
                                append(var->getStuffedVarSeparator(), flex);
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
                append("_empty", node, CursorAdhesion::strong);
                break;
        case EgcNodeType::AlnumNode:
                appendSigns(static_cast<EgcAlnumNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        case EgcNodeType::NumberNode:
                appendSigns(static_cast<EgcNumberNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::append(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subpos)
{
        ++m_id;
        appendRaw(str, node, m_id, cursorAdhesion, subpos);
}

void FormulaScrVisitor::appendRaw(QString str, EgcNode* node, quint32 id, CursorAdhesion cursorAdhesion, quint32 subpos)
{
        FormulaScrElement el;
        el.m_id = id;
        el.m_node = node;
        el.m_value = str;
        el.m_cAdh = cursorAdhesion;
        el.m_subpos = subpos;
        m_iter.insert(el);
}

void FormulaScrVisitor::appendSigns(QString str, EgcNode* node, CursorAdhesion cursorAdhesion)
{
        QString i;
        quint32 n = 1;
        foreach (i, str) {
                append(EgcAlnumNode::encode(i), node, cursorAdhesion);
                m_iter.peekPrevious().m_subpos = n++;
        }
}

void FormulaScrVisitor::appendSegmented(QString str, EgcNode* node, CursorAdhesion cursorAdhesion)
{
        quint32 id;
        if (m_hash.contains(node)) {
                id = m_hash.value(node);
        } else {
                id = ++m_id;
                m_hash.insert(node, id);
        }
        appendRaw(str, node, id, cursorAdhesion);
}

void FormulaScrVisitor::updateVector(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;

        m_id = 0;

        m_iter.clear();

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
                if (!m_suppressList.contains(node))
                        node->accept(this);
        };
}

QString FormulaScrVisitor::getResult(void)
{
        QString result;
        FormulaScrElement i;
        FormulaScrIter iter = m_iter;
        iter.toFront();
        while(iter.hasNext())
                result = result % iter.next().m_value;

        return result;
}

void FormulaScrVisitor::suppressNode(const EgcNode* node)
{
        m_suppressList.insert(node);
}
