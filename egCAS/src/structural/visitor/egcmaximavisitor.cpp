#include <QStringBuilder>
#include "../egcnodes.h"
#include "egcmaximavisitor.h"
#include "../egcformulaexpression.h"

EgcMaximaVisitor::EgcMaximaVisitor(EgcFormulaExpression& formula) : EgcNodeVisitor(formula)
{
}

void EgcMaximaVisitor::visit(EgcBinaryNode* binary)
{
        QString str;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode:
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

void EgcMaximaVisitor::visit(EgcUnaryNode* unary)
{
        QString str;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
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
#warning maxima function for engineering type is missing (load lisp function "engineering-format" from share/contrib/engineering-format.lisp)
                                tmp = tmpOptions + tmp;
                                break;
                        default:
                                tmp = tmpOptions + tmp;
                                break;
                        }
                        //reset the precision again to standard type
                        tmp += QString("ffprintprec:%1$").arg(EgcFormulaExpression::getStdNrSignificantDigis());
                }
        }

        return tmp + ";\n";
}

