#include <new>
#include <QScopedPointer>
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
        QScopedPointer<EgcNode> tmp(EgcNodeCreator::create(type));
        if (tmp.data()) {
                EgcNode* tmp_ptr = tmp.data();
                if (m_data.setChild(0, *tmp)) //if everything is fine
                        (void) tmp.take();
                if (tmp_ptr->isContainer()) {
                        EgcContainerNode* cont = static_cast<EgcContainerNode*>(tmp_ptr);
                        quint32 nrChildNodes = cont->getNumberChildNodes();

                        for (int i = 0; i < nrChildNodes; i++) {
                                QScopedPointer<EgcNode> tempNode(EgcNodeCreator::
                                                                 create(EgcNodeType::EmptyNode));
                                if (tempNode.data() != nullptr)
                                        cont->setChild(i, *(tempNode.take()));
                        }
                }
        }
}

EgcFormulaExpression::EgcFormulaExpression(const EgcFormulaExpression& orig)
{
        QScopedPointer<EgcNode> tmp;
        EgcNode* originalRoot = orig.getRootElement();
        tmp.reset(originalRoot->copy());
        if (tmp.data()) {
                if (m_data.setChild(0, *tmp.data()))
                        (void) tmp.take();
        }
        m_isResult = orig.m_isResult;
        m_isNumberResult = orig.m_isNumberResult;
        m_numberSignificantDigits = orig.m_numberSignificantDigits;
        m_numberResultType = orig.m_numberResultType;

}

EgcFormulaExpression::EgcFormulaExpression(EgcFormulaExpression&& orig)
{
        EgcNode* originalRoot = orig.getRootElement();
        if (originalRoot) {
                m_data.setChild(0, *static_cast<EgcBaseNode&>(orig.getBaseElement()).takeOwnership(*originalRoot));
                originalRoot->provideParent(&static_cast<EgcContainerNode&>(this->getBaseElement()));
                m_isResult = orig.m_isResult;
                m_isNumberResult = orig.m_isNumberResult;
                m_numberSignificantDigits = orig.m_numberSignificantDigits;
                m_numberResultType = orig.m_numberResultType;
        } else {
                m_isResult = false;
                m_isNumberResult = false;
                m_numberSignificantDigits = 0;
                m_numberResultType = EgcNumberResultType::StandardType;
        }
}

EgcFormulaExpression& EgcFormulaExpression::operator=(const EgcFormulaExpression &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        QScopedPointer<EgcNode> tmp;
        EgcNode* rhsRoot = rhs.getRootElement();
        tmp.reset(rhsRoot->copy());
        if (tmp.data()) {
                if (m_data.setChild(0, *tmp.data()))
                        (void) tmp.take();
        }

        m_isResult = rhs.m_isResult;
        m_isNumberResult = rhs.m_isNumberResult;
        m_numberSignificantDigits = rhs.m_numberSignificantDigits;
        m_numberResultType = rhs.m_numberResultType;

        return *this;
}

EgcFormulaExpression& EgcFormulaExpression::operator=(EgcFormulaExpression&& rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        EgcNode* rhsRoot = rhs.getRootElement();
        if (rhsRoot) {
                m_data.setChild(0, *static_cast<EgcBaseNode&>(rhs.getBaseElement()).takeOwnership(*rhsRoot));
                rhsRoot->provideParent(&static_cast<EgcContainerNode&>(this->getBaseElement()));
                m_isResult = rhs.m_isResult;
                m_isNumberResult = rhs.m_isNumberResult;
                m_numberSignificantDigits = rhs.m_numberSignificantDigits;
                m_numberResultType = rhs.m_numberResultType;
        } else {
                m_isResult = false;
                m_isNumberResult = false;
                m_numberSignificantDigits = 0;
                m_numberResultType = EgcNumberResultType::StandardType;
        }

        return *this;
}

EgcFormulaExpression::~EgcFormulaExpression()
{
}

EgcBaseNode& EgcFormulaExpression::getBaseElement(void) const
{
        return const_cast<EgcBaseNode&>(m_data);
}

EgcNode* EgcFormulaExpression::getRootElement(void) const
{
        return m_data.getChild(0);;
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
