#include <new>
#include "egcformulaexpression.h"
#include "egcexpressionnode.h"
#include "egcexpressionnodecreator.h"
#include "egcbaseexpressionnode.h"
#include "egcnodes.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"

EgcFormulaExpression::EgcFormulaExpression(EgcExpressionNodeType type)
{
        m_data = new (std::nothrow) EgcBaseExpressionNode();
        if (m_data) {
                EgcExpressionNode* tmp = EgcExpressionNodeCreator::create(type);
                if (tmp) {
                        m_data->setChild(*tmp);
                        if (tmp->isContainer()) {
                                if (tmp->isBinaryExpression()) {
                                        EgcBinaryExpressionNode *node = static_cast<EgcBinaryExpressionNode*>(tmp);
                                        EgcExpressionNode *child =
                                                EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode);
                                        if (child)
                                                node->setLeftChild(*child);
                                        child = EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode);
                                        if (child)
                                                node->setRightChild(*child);
                                } else { // unary expression
                                        EgcUnaryExpressionNode *node = static_cast<EgcUnaryExpressionNode*>(tmp);
                                        EgcExpressionNode *child =
                                                EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode);
                                        if (child)
                                                node->setChild(*child);
                                }
                        }
                }
        }
}

EgcFormulaExpression::~EgcFormulaExpression()
{
        // delete the formula with the complete tree
        delete m_data;
}

EgcBaseExpressionNode& EgcFormulaExpression::getBaseElement(void) const
{
        return *m_data;
}

EgcExpressionNode* EgcFormulaExpression::getRootElement(void) const
{
        EgcExpressionNode* retval = nullptr;

        if (m_data)
                retval = m_data->getChild();

        return retval;
}

QString EgcFormulaExpression::getMathMlCode(void)
{
        QString temp;

        temp = "<math>";
        EgcMathMlVisitor mathMlVisitor(*this);
        temp += mathMlVisitor.getResult();
        temp += "</math>";

        return temp;
}

QString EgcFormulaExpression::getCASKernelCommand(void)
{
        EgcMaximaVisitor maximaVisitor(*this);
        return maximaVisitor.getResult();
}
