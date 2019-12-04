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
#include <structural/iterator/formulascriter.h>
#include <QStringBuilder>




FormulaScrVisitor::FormulaScrVisitor(EgcFormulaEntity& formula, FormulaScrIter& iter) :  EgcNodeVisitor(formula),
        m_iter{iter}, m_currNode{nullptr}
{
}

void FormulaScrVisitor::visit(EgcBinaryNode* binary)
{
        m_currNode = binary;
        EgcNode* lnode = binary->getChild(0);
        if (!lnode)
                lnode = binary;
        EgcNode* rnode = binary->getChild(1);
        if (!rnode)
                rnode = binary;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode: {
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("_root(_{", binary, CursorAdhesion::low, 0, true, lnode, 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        appendSegmented("_},", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                } else {
                        appendSegmented(")", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                }
                break;
        }
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("+", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("-", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("*", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        appendSegmented("_{", binary, CursorAdhesion::low, 0, true, lnode, 0, true);
                else if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("_}/_{", lnode, CursorAdhesion::low, 0, true, rnode, 0, true);
                else
                        appendSegmented("_}", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                break;
        case EgcNodeType::BinEmptyNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("_emptybinop", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("^_{", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                else if (m_state == EgcIteratorState::RightIteration)
                        appendSegmented("_}", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                break;
        case EgcNodeType::EqualNode: {
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("=", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                }
                break;
        }
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append(":", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                        setElementSubposition(1, 2);
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(binary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcUnaryNode* unary)
{
        m_currNode = unary;
        EgcNode* node = unary->getChild(0);
        if (!node)
                node = unary;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("(", unary, CursorAdhesion::low, 0, true, node, 0, true);
                        setElementSubposition(1);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        appendSegmented(")", node, CursorAdhesion::low, 0, false, unary, 0, false);
                        setElementSubposition(2);
                }
                break;
        case EgcNodeType::LogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("_log", unary, CursorAdhesion::low, 0, true, unary, 3, false);
                        appendSegmented("(", unary, CursorAdhesion::low, 4, true, node, 0, true);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        appendSegmented(")", node, CursorAdhesion::low, 0, false, unary, 5, false);
                }
                break;
        case EgcNodeType::NatLogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("_ln", unary, CursorAdhesion::low, 0, true, unary, 2, false);
                        setElementSubposition(1, 2);
                        appendSegmented("(", unary, CursorAdhesion::low, 3, true, node, 0, true);
                        setElementSubposition(3);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        appendSegmented(")", node, CursorAdhesion::low, 0, false, unary, 4, false);
                        setElementSubposition(4);
                }
                break;
        case EgcNodeType::LParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("_red_parenth_l", unary, CursorAdhesion::low, 0, true, node, 0, true);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::RParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        append("_red_parenth_r", node, CursorAdhesion::low, 0, false, unary, 0, false);
                        setElementSubposition(1);
                }
                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("-", unary, CursorAdhesion::normal);
                        setElementSubposition(1);
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(unary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcFlexNode* flex)
{
        m_currNode = flex;
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        QString name = static_cast<EgcFunctionNode*>(flex)->getName();
                        if (name.isEmpty()) {
                                appendSegmented("_empty", flex, CursorAdhesion::strong, 1, true, flex, 1, false);
                                setElementSubposition(1);
                        } else {
                                appendSigns(name, flex, CursorAdhesion::strong);
                                setElementSubposition(1, name.length());
                        }
                        appendSegmented("(", flex, CursorAdhesion::low, name.length() + 1, true, flex->getChild(0), 0, true);
                        if (name.isEmpty())
                                setElementSubposition(2);
                        else
                                setElementSubposition(name.length() + 1);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        append(",", flex->getChild(m_childIndex), CursorAdhesion::low, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {                        
                        appendSegmented(")", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::low, 0, false, flex, 0, false);
                        QString name = static_cast<EgcFunctionNode*>(flex)->getName();
                        quint32 i = 0;
                        if (name.isEmpty())
                                i = 1;
                        else
                                i = name.length();
                        i++; // first parenthesis
                        i += flex->getNumberChildNodes() - 1;
                        i++; // this is the last parenthesis
                        setElementSubposition(i);
                }
                break;
        case EgcNodeType::IntegralNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("_integrate(", flex, CursorAdhesion::low, 0, true, flex->getChild(0), 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        append(",", flex->getChild(m_childIndex), CursorAdhesion::low, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented(")", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::low, 0, false, flex, 0, false);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        EgcDifferentialNode &diff = *static_cast<EgcDifferentialNode*>(flex);
                        int type = static_cast<int>(diff.getDifferentialType());
                        appendSegmented(QString("_diff(%1,").arg(type), flex, CursorAdhesion::low, 0, true, flex->getChild(0), 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        appendSegmented(",", flex->getChild(m_childIndex), CursorAdhesion::ultra, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented(")", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::ultra, 0, false, flex, 0, false);
                }
                break;
        case EgcNodeType::MatrixNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented(QString("matrix(["), flex, CursorAdhesion::low, 0, true, flex->getChild(0), 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        EgcMatrixNode &mat = *static_cast<EgcMatrixNode*>(flex);
                        quint16 cols = mat.columns();
                        if ((m_childIndex + 1) % cols == 0)
                                appendSegmented("],[", flex->getChild(m_childIndex), CursorAdhesion::ultra, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                        else
                                appendSegmented(",", flex->getChild(m_childIndex), CursorAdhesion::ultra, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented("])", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::ultra, 0, false, flex, 0, false);
                }
                break;

        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(flex->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcNode* node)
{
        m_currNode = node;
        switch (node->getNodeType()) {
        case EgcNodeType::EmptyNode:
                append("_empty", node, CursorAdhesion::strong);
                setElementSubposition(1);
                break;
        case EgcNodeType::AlnumNode:
                appendSigns(static_cast<EgcAlnumNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        case EgcNodeType::ConstantNode:
                append(static_cast<EgcConstantNode*>(node)->getStringRepresentation(), node, CursorAdhesion::strong);
                break;
        case EgcNodeType::NumberNode:
                appendSigns(static_cast<EgcNumberNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        case EgcNodeType::VariableNode: {
                EgcVariableNode *var = static_cast<EgcVariableNode*>(node);
                quint32 subpos;
                subpos = appendSigns(var->getValue(), node, CursorAdhesion::strong);
                if (!var->getSubscript().isEmpty()) {
                        if (var->isSubscriptEmptyElement()) {
                                append(var->getStuffedVarSeparator(), node);
                                append("_empty", node, CursorAdhesion::strong);
                                setElementSubposition(2);
                        } else {
                                append(var->getStuffedVarSeparator(), node);
                                appendSigns(var->getSubscript(), node, CursorAdhesion::strong, subpos);
                        }
                }
                break;
        }
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::appendSegmented(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subpos, bool leftSide,
                               EgcNode* rNode, quint32 rSubpos, bool rLeftSide)
{
        append(str, node, cursorAdhesion, subpos, leftSide, rNode, rSubpos, rLeftSide);
        FormulaScrElement& tmp = m_iter.peekPrevious();
        tmp.m_isSegmented = true;
}

void FormulaScrVisitor::append(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subpos, bool leftSide,
                               EgcNode* rNode, quint32 rSubpos, bool rLeftSide)
{
        FormulaScrElement el;
        el.m_value = str;
        el.m_cAdh = cursorAdhesion;
        el.m_node = m_currNode;
        el.lTemp.m_node = node;
        el.lTemp.m_subpos = subpos;
        el.lTemp.m_left_side = leftSide;
        if (rNode) {
                el.rTemp.m_node = rNode;
                el.rTemp.m_subpos = rSubpos;
                el.rTemp.m_left_side = rLeftSide;
        } else {
                el.rTemp.m_left_side = false;
                el.rTemp.m_node = node;
                el.rTemp.m_subpos = subpos;
        }
        if (m_state == EgcIteratorState::LeftIteration)
                el.m_sideNode = FormulaScrElement::nodeLeftSide;
        else if (m_state == EgcIteratorState::MiddleIteration)
                el.m_sideNode = FormulaScrElement::nodeMiddle;
        else // (m_state == EgcIteratorState::RightIteration)
                el.m_sideNode = FormulaScrElement::nodeRightSide;
        m_iter.insert(el);
}

quint32 FormulaScrVisitor::appendSigns(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subposBegin)
{
        QString i;
        quint32 n = subposBegin;
        foreach (i, str) {
                append(EgcAlnumNode::encode(i), node, cursorAdhesion);
                FormulaScrElement& tmp = m_iter.peekPrevious();
                tmp.lTemp.m_subpos = n;
                tmp.rTemp.m_subpos = n;
                tmp.m_subpos_min = n;
                tmp.m_subpos_max = n;
                n++;
        }

        return n;
}

void FormulaScrVisitor::updateVector(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;

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

        // do post processing lookup id's from node's
        doPostprocessing();
}

void FormulaScrVisitor::setElementSubposition(quint32 subposition, quint32 maxSubposition)
{
        FormulaScrElement& tmp = m_iter.peekPrevious();
        tmp.m_subpos_min = subposition;
        if (maxSubposition < subposition)
                tmp.m_subpos_max = subposition;
        else
                tmp.m_subpos_max = maxSubposition;
}

void FormulaScrVisitor::doPostprocessing(void)
{
        const EgcMathmlLookup lookup = m_formula->getMathmlMappingCRef();
        quint32 id;
        FormulaScrIter iter = m_iter;
        iter.toFront();
        while(iter.hasNext()) {
                FormulaScrElement& i = iter.next();
                if (i.lTemp.m_node) {
                        id = lookup.getIdFrame(*i.lTemp.m_node);
                        if (id)
                                i.lTemp.m_id = id;
                }
                if (i.rTemp.m_node) {
                        id = lookup.getIdFrame(*i.rTemp.m_node);
                        if (id)
                                i.rTemp.m_id = id;
                }
        }
}

QString FormulaScrVisitor::getResult(void)
{
        QString result;
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
