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
#ifdef DEBUG_MATHML_GENERATION
#include <QDebug>
#endif //#ifdef DEBUG_MATHML_GENERATION
#include "../egcnodes.h"
#include "egcmathmlvisitor.h"

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaEntity& formula) : VisitorHelper{formula},
                                                                m_prettyPrint{true},
                                                                m_idCounter{1},
                                                                m_lookup(formula.getMathmlMappingRef()) //gcc bug
{
}

void EgcMathMlVisitor::visit(EgcBinaryNode* node)
{
        /*the id for the outer bounds of the operation (the id that characterizes the outer bounds must always be the
         * lowest one - in order execution of the id generation)*/
        QString id;
        switch (node->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        // don't show the root exponent if it is empty
                        if (!m_formula->isActive())
                                suppressChildIfChildValue(node, 0, EgcNodeType::EmptyNode, "");
                } else if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mroot" %id%"><mrow>%2</mrow><mrow>%1</mrow></mroot>", node);
                }
                break;
        case EgcNodeType::EqualNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo" % getId(node) % ">=</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo" % getId(node) % ">:=</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo" % getId(node) % ">+</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo" % getId(node) % ">-</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo" % getId(node) % ">&CenterDot;</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::BinEmptyNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%">%1<mo mathcolor=\"#7F7F7F\"" % getId(node) % ">&compfn;</mo>%2</mrow>", node);
                }
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                } else if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mfrac "%id%">%1 %2</mfrac>", node);
                }
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<msup "%id%">%1 %2</msup>", node);
                }
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void EgcMathMlVisitor::visit(EgcUnaryNode* node)
{
        /*the id for the outer bounds of the operation (the id that characterizes the outer bounds must always be the
         * lowest one - in order execution of the id generation)*/
        QString id;

        switch (node->getNodeType()) {
        case EgcNodeType::ParenthesisNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mfenced "%id%" open=\"(\" close=\")\" separators=\",\"><mrow>%1</mrow></mfenced>", node);
                }
        }
        break;
        case EgcNodeType::LogNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%"><mi mathvariant=\"italic\" "%id%">log</mi><mo>&ApplyFunction;</mo><mrow>"
                                       "<mo "%id%">(</mo><mrow>%1</mrow><mo "%id%">)</mo></mrow></mrow>", node);
                }
        }
        break;
        case EgcNodeType::NatLogNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%"><mi mathvariant=\"italic\" "%id%">ln</mi><mo>&ApplyFunction;</mo><mrow>"
                                       "<mo "%id%">(</mo><mrow>%1</mrow><mo "%id%">)</mo></mrow></mrow>", node);
                }
        }
        break;
        case EgcNodeType::LParenthesisNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mfenced "%id%" open=\"(\" close=\"\" separators=\",\"><mrow>%1</mrow></mfenced>", node);
                }
        }
        break;
        case EgcNodeType::RParenthesisNode: {
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mfenced "%id%" open=\"\" close=\")\" separators=\",\"><mrow>%1</mrow></mfenced>", node);
                }
        }
        break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        assembleResult("<mrow "%id%"><mo" % getId(node) % ">-</mo>%1</mrow>", node);
                }
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void EgcMathMlVisitor::visit(EgcFlexNode* node)
{
        /*the id for the outer bounds of the operation (the id that characterizes the outer bounds must always be the
         * lowest one - in order execution of the id generation)*/
        QString id;

        switch (node->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        QString name = static_cast<EgcFunctionNode*>(node)->getName();
                        QString color;
                        if (name.isEmpty()) {
                                name = "&#x2B1A;";
                                color = "mathcolor=\"#7F7F7F\"";
                        }
                        assembleResult("<mrow "%id%"><mi " %color% " mathvariant=\"italic\"" %id%">" % name % "</mi>"
                                       "<mo>&ApplyFunction;</mo><mrow><mo" %id%">(</mo><mrow>",
                                       "<mo" %id%">,</mo>", "</mrow><mo" %id%">)</mo></mrow></mrow>", node);
                }
                break;
        case EgcNodeType::IntegralNode:
                if (node->getNumberChildNodes() == 2) { // indefinite integral
                        if (m_state == EgcIteratorState::RightIteration) {
                                id = getId(node);
                                assembleResult("<mrow "%id%"><mstyle scriptlevel=\"-1\"><mo>&Integral;</mo></mstyle><mrow>%1</mrow><mo>d</mo><mrow>%2</mrow></mrow>", node);
                        }
                } else if (node->getNumberChildNodes() == 4) { // integral with limits number of childs should be 4!
                        if (m_state == EgcIteratorState::RightIteration) {
                                id = getId(node);
                                assembleResult("<mrow "%id%"><munderover><mstyle scriptlevel=\"-1\"><mo>&Integral;</mo></mstyle><mrow>%1</mrow><mrow>%2</mrow></munderover><mrow>%3</mrow><mo>d</mo><mrow>%4</mrow></mrow>", node);
                        }
                }
                break;
        case EgcNodeType::DifferentialNode:               
                if (m_state == EgcIteratorState::LeftIteration) {
                        // don't show the empty exponent if exponent is 1 and type is leibnitz
                        if (    static_cast<EgcDifferentialNode*>(node)->getDifferentialType() == EgcDifferentialNode::DifferentialType::leibnitz
                             && !m_formula->isActive())
                                suppressChildIfChildValue(node, 2, EgcNodeType::EmptyNode, "");

                } else if (m_state == EgcIteratorState::RightIteration) {
                        id = getId(node);
                        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(node);
                        quint8 der = diff->getNrDerivative();
                        EgcDifferentialNode::DifferentialType type = diff->getDifferentialType();
                        QString derivative;

                        switch (type) {
                        case EgcDifferentialNode::DifferentialType::lagrange2:
                                derivative = "&Prime;";
                                break;
                        case EgcDifferentialNode::DifferentialType::lagrange3:
                                derivative = "&tprime;";
                                break;
                        case EgcDifferentialNode::DifferentialType::leibnitz:
                                derivative = "&qprime;";
                                break;
                        case EgcDifferentialNode::DifferentialType::lagrange1:
                                derivative = "&prime;";
                                break;
                        }

                        if (    type == EgcDifferentialNode::DifferentialType::lagrange1
                             || type == EgcDifferentialNode::DifferentialType::lagrange2
                             || type == EgcDifferentialNode::DifferentialType::lagrange3) {
                                derivative = "<mstyle scriptlevel=\"-1\"><mo>" % derivative % "</mo></mstyle>";
                                derivative = "<mrow "%id%"><msup>%" % QString::number(1) % derivative
                                             % "</msup><mfenced>%" % QString::number(2) % "</mfenced></mrow>";
                                assembleResult(derivative, node);
                        } else { // use leibniz' notation
                                QString result;
                                if (der == 1) {
                                        if (m_formula->isActive()) {
                                                result = "<mfrac "%id%"><mrow><mi>d</mi>"
                                                         % "<mfenced>%" % QString::number(1)
                                                         % "</mfenced></mrow><msup><mrow><mi>d</mi>%"
                                                         % QString::number(2) % "</mrow>%"
                                                         % QString::number(3) % "</msup></mfrac>";
                                        } else {
                                                result = "<mfrac "%id%"><mrow><mi>d</mi>"
                                                         % "<mfenced>%" % QString::number(1)
                                                         % "</mfenced></mrow><mrow><mi>d</mi>%"
                                                         % QString::number(2) % "</mrow></mfrac>";
                                        }
                                } else {
                                        result = "<mfrac "%id%"><mrow><msup><mi>d</mi><mn "%id%">" % QString::number(der)
                                                 % "</mn></msup><mfenced>%" % QString::number(1)
                                                 % "</mfenced></mrow><msup><mrow><mi>d</mi>%"
                                                 % QString::number(2) % "</mrow>%"
                                                 % QString::number(3) % "</msup></mfrac>";
                                }

                                assembleResult(result, node);
                        }
                }
                break;
        case EgcNodeType::ArgumentsNode:
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", static_cast<int>(node->getNodeType()));
                break;
        }
}

void EgcMathMlVisitor::visit(EgcNode* node)
{
        /*the id for the outer bounds of the operation (the id that characterizes the outer bounds must always be the
         * lowest one - in order execution of the id generation)*/
        QString id = getId(node);

        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                pushToStack("<mn" %id%">" % static_cast<EgcNumberNode*>(node)->getValue() % "</mn>", node);
                break;
        case EgcNodeType::AlnumNode:
                pushToStack("<mi mathvariant=\"normal\"" %id%">" % static_cast<EgcAlnumNode*>(node)->getValue()
                            % "</mi>", node);
                break;
        case EgcNodeType::VariableNode: {
                EgcVariableNode *var = static_cast<EgcVariableNode*>(node);
                if (var->getSubscript().isEmpty())
                        pushToStack("<mi mathvariant=\"normal\" "%id%">" % var->getValue() % "</mi>", node);
                else
                        pushToStack("<mrow><msub><mi mathvariant=\"normal\" "%id%">" % var->getValue()
                                       % "</mi><mi mathvariant=\"normal\" "%id%">" % var->getSubscript()
                                       % "</mi></msub></mrow>", node);
        }
                break;
        case EgcNodeType::ConstantNode: {
                EgcConstantNode *cnst = static_cast<EgcConstantNode*>(node);
                QString str = cnst->getStringRepresentation();
                str = str.remove("_const_");
                Constants c = cnst->getValue();
                switch (c) {
                case Constants::infinity:
                        pushToStack("<mo "%id%">&#x221E;</mo>", node);
                        break;
                default:
                        pushToStack("<mi mathvariant=\"italic\" "%id%">&InvisibleTimes; " % str % " &InvisibleTimes;</mi>", node);
                        break;
                }
        }
                break;
        case EgcNodeType::EmptyNode:
                pushToStack("<mi mathcolor=\"#7F7F7F\"" %id%">&#x2B1A;</mi>", node);
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", static_cast<int>(node->getNodeType()));
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
        temp += "</math>";

        //remove entries from lookup table that shall not be rendered
        cleanMathmlLookupTable();

#ifdef DEBUG_MATHML_GENERATION
        qDebug() << "mathml output of visitor: " << temp;
#endif //#ifdef DEBUG_MATHML_GENERATION

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

void EgcMathMlVisitor::suppressChildIfChildType(const EgcNode* node, quint32 index, EgcNodeType type)
{
        EgcNode* chldNode = getChildToSuppress(node, index);
        if (chldNode) {
                if (chldNode->getNodeType() == type) {
                        m_suppressList.insert(chldNode);
                }
        }
}

void EgcMathMlVisitor::suppressChildIfChildValue(const EgcNode* node, quint32 index, EgcNodeType type, QString val)
{
        EgcNode* chldNode = getChildToSuppress(node, index);
        if (chldNode) {
                if (chldNode->getNodeType() == type) {
                        QString value;
                        switch (type) {
                        case EgcNodeType::NumberNode:
                                value = static_cast<EgcNumberNode*>(chldNode)->getValue();
                                break;
                        case EgcNodeType::VariableNode:
                                value = static_cast<EgcVariableNode*>(chldNode)->getValue();
                                break;
                        case EgcNodeType::AlnumNode:
                                value = static_cast<EgcAlnumNode*>(chldNode)->getValue();
                                break;
                        case EgcNodeType::EmptyNode:
                                value = "";
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
        if (node)
                m_lookup.addId(*node, m_idCounter);
        m_idCounter++;

        return str;
}

void EgcMathMlVisitor::cleanMathmlLookupTable(void)
{
        QSetIterator<EgcNode*> i(m_suppressList);
        while (i.hasNext())
                m_lookup.removeId(i.next());
}

EgcNode* EgcMathMlVisitor::getChildToSuppress(const EgcNode* node, quint32 index)
{
        if (!m_prettyPrint)
                return nullptr;

        return VisitorHelper::getChildToSuppress(node, index);
}
