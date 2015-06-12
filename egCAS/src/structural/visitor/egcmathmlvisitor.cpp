#include <QStringBuilder>
#include "../egcnodes.h"
#include "egcmathmlvisitor.h"

EgcMathMlVisitor::EgcMathMlVisitor(EgcFormulaExpression& formula) : EgcNodeVisitor(formula)
{
}

void EgcMathMlVisitor::visit(EgcBinaryNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        str = "<mroot><mrow>";
                else if (m_state == EgcIteratorState::MiddleIteration)
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

void EgcMathMlVisitor::visit(EgcUnaryNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration)
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

void EgcMathMlVisitor::visit(EgcNode* node)
{
        QString str;

        switch (node->getNodeType()) {
        case EgcNodeType::NumberNode:
                str = "<mn>" + static_cast<EgcNumberNode*>(node)->getValue() + "</mn>";
                break;
        case EgcNodeType::VariableNode:
                str = "<mi>" + static_cast<EgcVariableNode*>(node)->getValue() + "</mi>";
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
