#include <new>
#include "egcformulaexpression.h"
#include "specialNodes/egcnode.h"
#include "egcnodecreator.h"
#include "specialNodes/egcbasenode.h"
#include "egcnodes.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"


quint8 EgcFormulaExpression::s_stdNrSignificantDigits = 0;

EgcFormulaExpression::EgcFormulaExpression(EgcNodeType type) : m_isResult(false), m_isNumberResult(false),
                                                                  m_numberSignificantDigits(0),
                                                                  m_numberResultType(EgcNumberResultType::StandardType)
{
        m_data = new (std::nothrow) EgcBaseNode();
        if (m_data) {
                EgcNode* tmp = EgcNodeCreator::create(type);
                if (tmp) {
                        m_data->setChild(*tmp);
                        if (tmp->isContainer()) {
                                if (tmp->isBinaryExpression()) {
                                        EgcBinaryNode *node = static_cast<EgcBinaryNode*>(tmp);
                                        EgcNode *child =
                                                EgcNodeCreator::create(EgcNodeType::EmptyNode);
                                        if (child)
                                                node->setLeftChild(*child);
                                        child = EgcNodeCreator::create(EgcNodeType::EmptyNode);
                                        if (child)
                                                node->setRightChild(*child);
                                } else { // unary expression
                                        EgcUnaryNode *node = static_cast<EgcUnaryNode*>(tmp);
                                        EgcNode *child =
                                                EgcNodeCreator::create(EgcNodeType::EmptyNode);
                                        if (child)
                                                node->setChild(*child);
                                }
                        }
                }
        }
}

EgcFormulaExpression::EgcFormulaExpression(const EgcFormulaExpression& orig)
{
        m_data = nullptr;
        EgcBaseNode& originalBase = orig.getBaseElement();
        m_data = new (std::nothrow) EgcBaseNode(originalBase);
        m_isResult = orig.m_isResult;
        m_isNumberResult = orig.m_isNumberResult;
        m_numberSignificantDigits = orig.m_numberSignificantDigits;
        m_numberResultType = orig.m_numberResultType;

}

EgcFormulaExpression& EgcFormulaExpression::operator=(const EgcFormulaExpression &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_data) {
                delete m_data;
                m_data = nullptr;
        }

        //and create a new one
        EgcBaseNode& originalBase = rhs.getBaseElement();
        m_data = new (std::nothrow) EgcBaseNode(originalBase);
        m_isResult = rhs.m_isResult;
        m_isNumberResult = rhs.m_isNumberResult;
        m_numberSignificantDigits = rhs.m_numberSignificantDigits;
        m_numberResultType = rhs.m_numberResultType;

        return *this;
}

EgcFormulaExpression::~EgcFormulaExpression()
{
        // delete the formula with the complete tree
        delete m_data;
}

EgcBaseNode& EgcFormulaExpression::getBaseElement(void) const
{
        return *m_data;
}

EgcNode* EgcFormulaExpression::getRootElement(void) const
{
        EgcNode* retval = nullptr;

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
