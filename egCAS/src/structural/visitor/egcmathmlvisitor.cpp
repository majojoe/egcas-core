#include <QStringBuilder>
#include "../egcnodes.h"
#include "egcmathmlvisitor.h"

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaExpression& formula) : EgcNodeVisitor(formula)
{
}

void EgcMathMlVisitor::visit(EgcBinaryExpressionNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcExpressionNodeType::RootNode:
                if (m_state == EgcNodeIteratorState::LeftIteration)
                        str = "<mroot><mrow>";
                else if (m_state == EgcNodeIteratorState::MiddleIteration)
                        str = "</mrow><mrow>";
                else
                        str = "</mrow></mroot>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", binary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMathMlVisitor::visit(EgcUnaryExpressionNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcExpressionNodeType::ParenthesisNode:
                if (m_state == EgcNodeIteratorState::LeftIteration)
                        str = "<mfenced open=\"(\" close=\")\" separators=\",\"><mrow>";
                else
                        str = "</mrow></mfenced>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", unary->getNodeType()) ;
                break;
        }

        m_result += str;
}

void EgcMathMlVisitor::visit(EgcExpressionNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcExpressionNodeType::NumberNode:
                str = "<mn>" + static_cast<EgcNumberExpressionNode*>(node)->getValue() + "</mn>";
                break;
        case EgcExpressionNodeType::VariableNode:
                str = "<mi>" + static_cast<EgcVariableExpressionNode*>(node)->getValue() + "</mi>";
                break;
        default:
                qDebug("No visitor code for mathml defined for this type: %d", node->getNodeType()) ;
                break;
        }

        m_result += str;
}

QString EgcMathMlVisitor::getResult(void)
{
        QString temp;

        temp = "<math>";
        temp += EgcNodeVisitor::getResult();
        temp += "</math>";

        return temp;
}
