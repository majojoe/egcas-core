#include <new>
#include "egcformulaexpression.h"
#include "egcexpressionnode.h"
#include "egcexpressionnodecreator.h"
#include "egcbaseexpressionnode.h"
#include "egcnodes.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"


#warning implement a copy constructor and assignment operator

quint8 EgcFormulaExpression::s_stdNrSignificantDigits = 0;

EgcFormulaExpression::EgcFormulaExpression(EgcExpressionNodeType type) : m_isResult(false), m_isNumberResult(false),
                                                                  m_numberSignificantDigits(0),
                                                                  m_numberResultType(EgcNumberResultType::StandardType)
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
        EgcMathMlVisitor mathMlVisitor(*this);
        return mathMlVisitor.getResult();
}

QString EgcFormulaExpression::getCASKernelCommand(void)
{
        EgcMaximaVisitor maximaVisitor(*this);
        return maximaVisitor.getResult();
}

bool EgcFormulaExpression::isResult(void)
{
        return m_isResult;
}

bool EgcFormulaExpression::isNumberResult(void)
{
        return m_isNumberResult;
}

void EgcFormulaExpression::setNumberOfSignificantDigits(quint8 digits)
{
        m_numberSignificantDigits = digits;
}

void EgcFormulaExpression::setNumberResultType(EgcNumberResultType resultType)
{
        m_numberResultType = resultType;
}

quint8 EgcFormulaExpression::getNumberOfSignificantDigits(void)
{
        if (m_isNumberResult)
                return m_numberSignificantDigits;
        else
                return 0;
}

EgcNumberResultType EgcFormulaExpression::getNumberResultType()
{
        if (m_isNumberResult)
                return m_numberResultType;
        else
                return EgcNumberResultType::NotApplicable;
}

quint8 EgcFormulaExpression::getStdNrSignificantDigis(void)
{
        return s_stdNrSignificantDigits;
}

void EgcFormulaExpression::setStdNrSignificantDigis(quint8 digits)
{
        s_stdNrSignificantDigits = digits;
}
