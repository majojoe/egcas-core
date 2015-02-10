#include <QStringBuilder>
#include "../egcnodes.h"
#include "egcmaximavisitor.h"

EgcMaximaVisitor::EgcMaximaVisitor(EgcFormulaExpression& formula) : EgcNodeVisitor(formula)
{
}

void EgcMaximaVisitor::visit(EgcBinaryExpressionNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcExpressionNodeType::RootNode:
                if (m_state == EgcNodeIteratorState::LeftIteration)
                        str = "(";
                else if (m_state == EgcNodeIteratorState::RightIteration)
                        str = ")";
                else
                        str = ")^(1/";
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", binary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMaximaVisitor::visit(EgcUnaryExpressionNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcExpressionNodeType::ParenthesisNode:
                if (m_state == EgcNodeIteratorState::LeftIteration)
                        str = "(";
                else
                        str = ")";
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", unary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMaximaVisitor::visit(EgcExpressionNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcExpressionNodeType::NumberNode:
                str = QString::number(static_cast<EgcNumberExpressionNode*>(node)->getValue(), 'g');
                break;
        case EgcExpressionNodeType::VariableNode:
                str = static_cast<EgcVariableExpressionNode*>(node)->getValue();
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", node->getNodeType()) ;
                break;
        }

        m_result += str;
}

QString EgcMaximaVisitor::getResult(void)
{
        return EgcNodeVisitor::getResult() + ";\n";
}

