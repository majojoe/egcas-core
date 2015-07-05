/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <new>
#include <QScopedPointer>
#include "egcformulaexpression.h"
#include "specialNodes/egcnode.h"
#include "egcnodecreator.h"
#include "specialNodes/egcbasenode.h"
#include "egcnodes.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"


quint8 EgcFormulaExpression::s_stdNrSignificantDigits = 15;

EgcFormulaExpression::EgcFormulaExpression(EgcNodeType type) : m_numberSignificantDigits(0),
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

EgcFormulaExpression::EgcFormulaExpression(EgcNode& rootElement) : m_numberSignificantDigits(0),
                                                                   m_numberResultType(EgcNumberResultType::StandardType)
{
        QScopedPointer<EgcNode> tmp(&rootElement);
        if (tmp.data()) {
                m_data.setChild(0, *(tmp.take()));
        }
}

EgcFormulaExpression::EgcFormulaExpression(void) : EgcFormulaExpression(EgcNodeType::EmptyNode)
{
}

EgcFormulaExpression::EgcFormulaExpression(const EgcFormulaExpression& orig)
{
        QScopedPointer<EgcNode> tmp;
        EgcNode* originalRoot = orig.getRootElement();
        tmp.reset(originalRoot->copy());
        if (tmp.data()) {
                m_data.setChild(0, *(tmp.take()));
        }
        m_numberSignificantDigits = orig.m_numberSignificantDigits;
        m_numberResultType = orig.m_numberResultType;

}

EgcFormulaExpression::EgcFormulaExpression(EgcFormulaExpression&& orig)
{
        EgcNode* originalRoot = orig.getRootElement();
        if (originalRoot) {
                m_data.setChild(0, *static_cast<EgcBaseNode&>(orig.getBaseElement()).takeOwnership(*originalRoot));
                originalRoot->provideParent(&static_cast<EgcContainerNode&>(this->getBaseElement()));
                m_numberSignificantDigits = orig.m_numberSignificantDigits;
                m_numberResultType = orig.m_numberResultType;
        } else {
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
                m_data.setChild(0, *(tmp.take()));
        }

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
                m_numberSignificantDigits = rhs.m_numberSignificantDigits;
                m_numberResultType = rhs.m_numberResultType;
        } else {
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

void EgcFormulaExpression::setRootElement(EgcNode* rootElement)
{
        QScopedPointer<EgcNode> tmp(rootElement);
        if (tmp.data()) {
                m_data.setChild(0, *(tmp.take()));
        }
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
        bool retval = false;

        EgcNode* root = getRootElement();
        if (root) {
                if (root->getNodeType() == EgcNodeType::EqualNode)
                        retval = true;
        }

        return retval;
}

bool EgcFormulaExpression::isNumberResult(void)
{
        bool retval = false;

        EgcNode* root = getRootElement();
        if (root) {
                if (root->getNodeType() == EgcNodeType::EqualNode) {
                        EgcNode* rightChild = nullptr;
                        rightChild = static_cast<EgcBinaryNode*>(root)->getChild(1);
                        if (rightChild) {
                                if (rightChild->getNodeType() == EgcNodeType::NumberNode)
                                        retval = true;
                        }
                }
        }

        return retval;
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
        if (isNumberResult())
                return m_numberSignificantDigits;
        else
                return 0;
}

EgcNumberResultType EgcFormulaExpression::getNumberResultType()
{
        if (isNumberResult())
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

bool EgcFormulaExpression::setResult(EgcNode* result)
{
        bool repaint = false;
        bool equal = true;
        QScopedPointer<EgcNode> res(result);
        if (isResult()) {
                EgcEqualNode* root = static_cast<EgcEqualNode*>(getRootElement());
                //check if result is equal with result in formula
                EgcNode* rightChild = root->getChild(1);
                if (rightChild) {
#warning implement isEqual and uncomment the following
                        //if (rightChild->isEqual(res.data()))
                                equal = true;
                }

                //set the result
                root->setChild(1, *(res.take()));
                if (!equal)
                        repaint = true;
        }
}

bool EgcFormulaExpression::resetResult(void)
{
        bool retval = false;

        if (isResult()) {
                QScopedPointer<EgcNode> emptyNode(EgcNodeCreator::create(EgcNodeType::EmptyNode));
                if (!emptyNode.isNull()) {
                        EgcEqualNode *root = static_cast<EgcEqualNode*>(getRootElement());
                        root->setChild(1, *(emptyNode.take()));
                }
        }
}
