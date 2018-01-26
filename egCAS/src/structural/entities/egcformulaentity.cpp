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

#include <new>
#include <QScopedPointer>
#include <QPointF>
#include "egcformulaentity.h"
#include "specialNodes/egcnode.h"
#include "egcnodecreator.h"
#include "specialNodes/egcbasenode.h"
#include "egcnodes.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/restructurevisitor.h"
#include "visitor/egcmathmlvisitor.h"
#include "egcabstractformulaitem.h"
#include "egcabstractentitylist.h"
#include "actions/egcaction.h"
#include "iterator/egcscrpositerator.h"
#include "document/egcabstractdocument.h"
#include "casKernel/parser/abstractkernelparser.h"
#include "casKernel/parser/restructparserprovider.h"

quint8 EgcFormulaEntity::s_stdNrSignificantDigits = 0;
int EgcFormulaEntity::s_fontSize = 20;

EgcFormulaEntity::EgcFormulaEntity(EgcNodeType type) : m_numberSignificantDigits(0),
                                                       m_numberResultType(EgcNumberResultType::StandardType),
                                                       m_item(nullptr),
                                                       m_errorMsg(QString::null)
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

EgcFormulaEntity::EgcFormulaEntity(EgcNode& rootElement) : m_numberSignificantDigits(0),
                                                           m_numberResultType(EgcNumberResultType::StandardType),
                                                           m_item(nullptr),
                                                           m_errorMsg(QString::null)
{
        QScopedPointer<EgcNode> tmp(&rootElement);
        if (tmp.data()) {
                m_data.setChild(0, *(tmp.take()));
        }
}

EgcFormulaEntity::EgcFormulaEntity(void) : EgcFormulaEntity{EgcNodeType::EmptyNode}
{
}

EgcFormulaEntity::EgcFormulaEntity(const EgcFormulaEntity& orig) : m_numberSignificantDigits(0),
                                                                   m_numberResultType(EgcNumberResultType::StandardType),
                                                                   m_item(nullptr),
                                                                   m_errorMsg(QString::null)
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

EgcFormulaEntity::EgcFormulaEntity(EgcFormulaEntity&& orig) : m_numberSignificantDigits(0),
                                                              m_numberResultType(EgcNumberResultType::StandardType),
                                                              m_item(nullptr),
                                                              m_errorMsg(QString::null)
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

EgcFormulaEntity& EgcFormulaEntity::operator=(const EgcFormulaEntity &rhs)
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
        m_item = nullptr;
        m_errorMsg = QString::null;

        return *this;
}

EgcFormulaEntity& EgcFormulaEntity::operator=(EgcFormulaEntity&& rhs)
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

        m_item = nullptr;
        m_errorMsg = QString::null;

        return *this;
}

bool EgcFormulaEntity::operator==(const EgcFormulaEntity& formula) const
{
        EgcNode* node1 = this->getRootElement();
        EgcNode* node2 = formula.getRootElement();

        if (    !node1
             || !node2)
                return false;

        if (*node1 == *node2)
                return true;

        return false;
}


EgcFormulaEntity::~EgcFormulaEntity()
{
}

EgcBaseNode& EgcFormulaEntity::getBaseElement(void) const
{
        return const_cast<EgcBaseNode&>(m_data);
}

EgcNode* EgcFormulaEntity::getRootElement(void) const
{
        return m_data.getChild(0);;
}

void EgcFormulaEntity::setRootElement(EgcNode* rootElement)
{
        QScopedPointer<EgcNode> tmp(rootElement);
        if (tmp.data()) {
                m_data.setChild(0, *(tmp.take()));
        }
}

QString EgcFormulaEntity::getMathMlCode(void)
{
        EgcMathMlVisitor mathMlVisitor(*this);        
        QString tmp = mathMlVisitor.getResult();

        // any started formula change must be finalized when the mathml lookup table has been populated, so do this
//        if (m_scrIter)
//                m_scrIter->finishFormulaChange();

        return tmp;
}

QString EgcFormulaEntity::getCASKernelCommand(void)
{
        //delete the error message in case of a recalculation
        m_errorMsg.clear();
        EgcMaximaVisitor maximaVisitor(*this);
        return maximaVisitor.getResult();
}

//void EgcFormulaEntity::reStructureTree(void)
//{
//        RestructParserProvider pp;
//        ReStructureVisitor restructureVisitor(*this);
//        QString result = restructureVisitor.getResult();
//        NodeIterReStructData iterData;
//        int errCode;
//        EgcNode* tree = pp.getRestructParser()->restructureFormula(result, iterData, &errCode);
//        if (tree) {
//                setRootElement(tree);
//                m_scrIter->invalidateCursor(getBaseElement());
//                m_scrIter->updateRestructureData(iterData);
//        }
//}

bool EgcFormulaEntity::isResult(void)
{
        bool retval = false;

        EgcNode* root = getRootElement();
        if (root) {
                if (root->getNodeType() == EgcNodeType::EqualNode)
                        retval = true;
        }

        return retval;
}

bool EgcFormulaEntity::isNumberResult(void)
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

void EgcFormulaEntity::setNumberOfSignificantDigits(quint8 digits)
{
        m_numberSignificantDigits = digits;
}

void EgcFormulaEntity::setNumberResultType(EgcNumberResultType resultType)
{
        m_numberResultType = resultType;
}

quint8 EgcFormulaEntity::getNumberOfSignificantDigits(void)
{
        return m_numberSignificantDigits;
}

EgcNumberResultType EgcFormulaEntity::getNumberResultType()
{
        return m_numberResultType;
}

quint8 EgcFormulaEntity::getStdNrSignificantDigis(void)
{
        return s_stdNrSignificantDigits;
}

void EgcFormulaEntity::setStdNrSignificantDigis(quint8 digits)
{
        s_stdNrSignificantDigits = digits;
}

bool EgcFormulaEntity::setResult(EgcNode* result)
{
        bool repaint = false;

        if (m_mod)
                return false;
        
        //reset error message of the formula
        m_errorMsg.clear();
        
        QScopedPointer<EgcNode> res(result);
        if (isResult()) {
                bool equal = false;

                EgcEqualNode* root = static_cast<EgcEqualNode*>(getRootElement());
                //check if result is equal with result in formula
                EgcNode* rightChild = root->getChild(1);
                if (rightChild && !res.isNull()) {
                        if (*rightChild == *res)
                                equal = true;
                }

                //set the result
                root->setChild(1, *(res.take()));
                if (!equal)
                        repaint = true;
        }

        return repaint;
}

void EgcFormulaEntity::resetResult(void)
{
        if (isResult()) {
                //don't do anything if the formula is modified at the moment
                if (m_mod)
                        return;

                QScopedPointer<EgcNode> emptyNode(EgcNodeCreator::create(EgcNodeType::EmptyNode));
                if (!emptyNode.isNull()) {
                        EgcEqualNode *root = static_cast<EgcEqualNode*>(getRootElement());
                        root->setChild(1, *(emptyNode.take()));
                }
        }
}

enum EgcEntityType EgcFormulaEntity::getEntityType(void) const
{
        return EgcEntityType::Formula;
}

QPointF EgcFormulaEntity::getPosition(void) const
{
        if (!m_item)
                return QPointF(0,0);
        else
                return m_item->getPosition();
}

void EgcFormulaEntity::setItem(EgcAbstractFormulaItem* item)
{
        m_item = item;
}

void EgcFormulaEntity::setPosition(QPointF pos)
{
        if (!m_item)
                return;

        m_item->setPos(pos);
}

void EgcFormulaEntity::setGenericFontSize(int size)
{
        s_fontSize = size;
}

int EgcFormulaEntity::getGenericFontSize(void)
{
        return s_fontSize;
}

int EgcFormulaEntity::getFontSize(void) const
{
        return s_fontSize;
}

void EgcFormulaEntity::updateView(void)
{
        if (!m_item)
                return;

        m_item->updateView();
}

void EgcFormulaEntity::itemChanged(EgcItemChangeType changeType)
{
        if (changeType == EgcItemChangeType::posChanged) {
                if (m_list)
                        m_list->sort();
                showCurrentCursor();
                if (getDocument()) {
                        getDocument()->restartCalculation();
                }
        }

        if (changeType == EgcItemChangeType::contentChanged) {
                if (m_mod)
                        m_mod->viewHasChanged();
                showCurrentCursor();
        }
}

void EgcFormulaEntity::setErrorMessage(QString msg)
{
        m_errorMsg = msg;
}

QString EgcFormulaEntity::getErrorMessage(void)
{
        return m_errorMsg;
}

void EgcFormulaEntity::handleAction(const EgcAction& action)
{
        switch (action.m_op) {
        case EgcOperations::formulaActivated:
                showCurrentCursor();
                m_mod.reset(new FormulaModificator(*this));
                break;
        case EgcOperations::formulaDeactivated:
                if (m_mod)
                        m_mod.reset();
                break;
        case EgcOperations::cursorForward:
                if (m_mod && m_item)
                        m_mod->moveCursor(true);
                break;
        case EgcOperations::cursorBackward:
                if (m_mod && m_item)
                        m_mod->moveCursor(false);
                break;
        case EgcOperations::spacePressed:
                markParent();
                break;
        case EgcOperations::alnumKeyPressed:
                if (m_mod && m_item)
                        m_mod->insertCharacter(action.m_character);
                break;
        case EgcOperations::backspacePressed:
                if (m_mod && m_item)
                        m_mod->removeElement(true);
                break;
        case EgcOperations::delPressed:
                if (m_mod && m_item)
                        m_mod->removeElement(false);
                break;
        case EgcOperations::mathCharOperator:
        case EgcOperations::mathFunction:
        case EgcOperations::internalFunction:
                if (m_mod && m_item)
                        m_mod->insertOperation(action);
                break;
//        case EgcOperations::homePressed:
//                if (m_scrIter) {
//                        m_scrIter->toFront();
//                        m_scrIter->resetUnderline();
//                        showCurrentCursor();
//                }
//                break;
//        case EgcOperations::endPressed:
//                if (m_scrIter) {
//                        m_scrIter->toBack();
//                        m_scrIter->resetUnderline();
//                        showCurrentCursor();
//                }
//                break;
        case EgcOperations::createSubscript:
                createSubId();
                break;
        }
}

void EgcFormulaEntity::createSubId(void)
{
//        EgcNode* node = const_cast<EgcNode*>(m_scrIter->node());
//        if (node) {
//                if (node->isAtomicallyBoundChild()) {
//                        node = node->getParent();
//                        if (!node)
//                                return;
//                }
//                if (node->getNodeType() == EgcNodeType::VariableNode) {
//                        EgcVariableNode* var = static_cast<EgcVariableNode*>(node);
//                        if (var->getNumberChildNodes() != 1)
//                                return;
//                        var->insertSubscript();
//                        m_item->hideCursors();
//                        m_item->updateView();
//                        m_scrIter->next();
//                        showCurrentCursor();
//                }
//        }
}

EgcNode* EgcFormulaEntity::replaceEmptyNodeWith(EgcNodeType type)
{
//        if (!isEmptyNode())  //check if current node is an empty node
//                return nullptr;

//        QScopedPointer<EgcNode> newNode(EgcNodeCreator::create(type));
//        if (!newNode)
//                return nullptr;

//        EgcNode* oldNode = const_cast<EgcNode*>(m_scrIter->node());
//        EgcNode* newPtr = newNode.data();
//        if (!paste(*newNode.take(), *oldNode))
//                return nullptr;

//        m_scrIter->updatePointer(newPtr, true);

//        return newPtr;
}

bool EgcFormulaEntity::isEmptyNode(void)
{
//        if (m_mod->node()->getNodeType() == EgcNodeType::EmptyNode) {
//                return true;
//        }

        return false;
}

void EgcFormulaEntity::insertOperation(EgcAction operation)
{
//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;

//        insertOp(operation);

//        reStructureTree();

//        m_item->hideCursors();
//        m_item->updateView();
//        showCurrentCursor();
}

bool EgcFormulaEntity::insertOp(EgcAction operations)
{
        bool retval = false;

//        if (operations.m_op == EgcOperations::mathCharOperator) {
//                if (    operations.m_character == '('
//                                || operations.m_character == ')') {
//                        if (    operations.m_character == '(')
//                                return createAndInsertOp(EgcNodeType::LParenthesisNode);
//                        if (    operations.m_character == ')')
//                                return createAndInsertOp(EgcNodeType::RParenthesisNode);
//                }
//                if (operations.m_character == '+')
//                        return createAndInsertOp(EgcNodeType::PlusNode);
//                if (operations.m_character == '-')
//                        return createAndInsertOp(EgcNodeType::MinusNode);
//                if (operations.m_character == '/')
//                        return createAndInsertOp(EgcNodeType::DivisionNode);
//                if (operations.m_character == '*')
//                        return createAndInsertOp(EgcNodeType::MultiplicationNode);
//                if (operations.m_character == ':')
//                        return createAndInsertOp(EgcNodeType::DefinitionNode);
//                if (operations.m_character == '=')
//                        return createAndInsertOp(EgcNodeType::EqualNode);
//                if (operations.m_character == QChar(177))
//                        return createAndInsertOp(EgcNodeType::UnaryMinusNode);
//                if (operations.m_character == QChar(8730))
//                        return createAndInsertOp(EgcNodeType::RootNode);
//                if (operations.m_character == QChar('^'))
//                        return createAndInsertOp(EgcNodeType::ExponentNode);
//                if (operations.m_character == QChar(','))
//                        return insertFunctionContainer();
//        } else if (operations.m_op == EgcOperations::mathFunction) { // functions
//                EgcFunctionNode* fnc = dynamic_cast<EgcFunctionNode*>(createAndInsertOperation(EgcNodeType::FunctionNode));
//                if (!fnc)
//                        return false;
//                if (!operations.m_additionalData.isNull()) {
//                        QString name = operations.m_additionalData.toString();
//                        if (!name.isEmpty()) {
//                                fnc->setName(name);
//                                m_scrIter->setCursorAtDelayed(fnc->getChild(0), true);
//                        } else {
//                                m_scrIter->setCursorAtDelayed(fnc, false);
//                        }
//                }
//        } else if (operations.m_op == EgcOperations::internalFunction) { // internal functions
//                if (operations.m_intType == InternalFunctionType::natLogarithm
//                     || operations.m_intType == InternalFunctionType::logarithm) {
//                        if (operations.m_intType == InternalFunctionType::natLogarithm)
//                                retval = createAndInsertOp(EgcNodeType::NatLogNode);
//                        else if (operations.m_intType == InternalFunctionType::logarithm)
//                                retval = createAndInsertOp(EgcNodeType::LogNode);

//                        if (retval) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        m_scrIter->setCursorAtDelayed(const_cast<EgcNode*>(nd), true);
//                                }
//                        }
//                } else if (operations.m_intType == InternalFunctionType::integral) {
//                        bool ret = createAndInsertOp(EgcNodeType::IntegralNode);
//                        if (ret) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        EgcContainerNode *par = nullptr;
//                                        par = nd->getParent();
//                                        if (par->getNodeType() == EgcNodeType::IntegralNode) {
//                                                static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                if (operations.m_OpModificators == OpModificators::definiteIntegral) {
//                                                        static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                        static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                }
//                                        }
//                                }
//                        }
//                        return ret;
//                } else if (operations.m_intType == InternalFunctionType::differential) {
//                        bool ret = createAndInsertOp(EgcNodeType::DifferentialNode);
//                        if (ret) {
//                                const EgcNode *nd = nullptr;
//                                if (m_scrIter->node()) {
//                                        nd = m_scrIter->node();
//                                        EgcContainerNode *par = nullptr;
//                                        par = nd->getParent();
//                                        if (par->getNodeType() == EgcNodeType::DifferentialNode) {
//                                                EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(par);
//                                                static_cast<EgcIntegralNode*>(par)->insert(0, *new EgcEmptyNode());
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_1)
//                                                        diff->setNrDerivative(1);
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_2)
//                                                        diff->setNrDerivative(2);
//                                                if (operations.m_lookModificatiors == LookModificators::differential_lagrange_notation_3)
//                                                        diff->setNrDerivative(3);
//                                        }
//                                }
//                        }
//                        return ret;
//                }

//        }
        return retval;
}

bool EgcFormulaEntity::createAndInsertOp(EgcNodeType type)
{
    if (createAndInsertOperation(type))
            return true;

    return false;
}

EgcNode* EgcFormulaEntity::createAndInsertOperation(EgcNodeType type)
{
//        const EgcNode* node = m_scrIter->node();
//        if (node) {
//                if (node->getNodeType() == EgcNodeType::AlnumNode)
//                        node = node->getParent();
//        }

//        if (!node)
//                return nullptr;

//        if (type == EgcNodeType::BaseNode)
//                return nullptr;

//        QScopedPointer<EgcNode> tmp(EgcNodeCreator::create(type));
//        if (!tmp)
//                return nullptr;

//        return m_scrIter->insert(type);

        return nullptr;
}

void EgcFormulaEntity::insertCharacter(QChar character)
{
//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;

//        if (isEmptyNode()) {
//                if (character.isDigit()) {
//                        EgcNumberNode * node = static_cast<EgcNumberNode*>(replaceEmptyNodeWith(EgcNodeType::NumberNode));
//                        if (node)
//                                node->setValue("");
//                } else {
//                        replaceEmptyNodeWith(EgcNodeType::VariableNode);
//                }
//                m_item->updateView();
//        }

//        (void) m_scrIter->insert(character);
//        m_item->hideCursors();
//        m_item->updateView();
}

void EgcFormulaEntity::removeCharacter(bool before)
{
//        bool structureChanged;

//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;

//        if (m_scrIter->node()->getNodeType() == EgcNodeType::EmptyNode) {
//                if (isEmpty()) {
//                        if (getDocument()) {
//                                getDocument()->deleteEntity(this);
//                                return;
//                        }
//                }
//        }

//        if (before)
//                m_scrIter->backspace(structureChanged);
//        else
//                m_scrIter->remove(structureChanged);

//        if (structureChanged) {
//                reStructureTree();
//        }
//        //update m_scrIter since nothing is correct about the position
//        m_item->updateView();
//        m_item->hideCursors();
//        showCurrentCursor();
}

bool EgcFormulaEntity::insertFunctionContainer(void)
{

//        return m_scrIter->insertChildSpace();
}

void EgcFormulaEntity::moveCursor(bool forward)
{
//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;
//        if (forward) {
//                if (m_scrIter->hasNext())
//                        (void) m_scrIter->next();
//                else
//                        m_scrIter->resetUnderline();
//        } else {
//                if (m_scrIter->hasPrevious())
//                        (void) m_scrIter->previous();
//                else
//                        m_scrIter->resetUnderline();
//        }

//        showCurrentCursor();
//        if (m_scrIter->isUnderlineActive())
//                m_item->showUnderline(m_scrIter->id());
}

void EgcFormulaEntity::showCurrentCursor(void)
{
//        quint32 id;
//        qint32 ind;
//        bool rightSide;

//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;

//        id = m_scrIter->id();
//        ind = m_scrIter->subIndex();
//        if (ind < 0) { //this is a container
//                ind = 0;
//        } else { //this is a glyph
//                ind++;
//        }
//        rightSide = m_scrIter->rightSide();

//        m_item->hideCursors();

//        if (rightSide)
//                m_item->showRightCursor(id, ind);
//        else
//                m_item->showLeftCursor(id, ind);

        if (m_mod)
                m_mod->showCurrentCursor();
}

EgcMathmlLookup& EgcFormulaEntity::getMathmlMappingRef(void)
{
        return m_mathmlLookup;
}

const EgcMathmlLookup& EgcFormulaEntity::getMathmlMappingCRef(void) const
{
        return m_mathmlLookup;
}

void EgcFormulaEntity::markParent(void)
{
//        quint32 id;
//        qint32 ind;
//        bool rightSide;

//        if (!m_scrIter)
//                return;
//        if (!m_item)
//                return;

//        rightSide = m_scrIter->rightSide();

//        id = m_scrIter->getNextVisibleParent();
//        ind = m_scrIter->subIndex();
//        if (ind < 0) { //this is a container
//                ind = 0;
//        } else { //this is a glyph
//                ind++;
//        }
//        if (rightSide)
//                m_item->showRightCursor(id, ind);
//        else
//                m_item->showLeftCursor(id, ind);

//        m_item->showUnderline(id);
}

EgcNode* EgcFormulaEntity::copy(EgcNode& node)
{
        if (!isNodeInFormula(node))
                return nullptr;

        QScopedPointer<EgcNode> copiedTree(node.copy());

        return copiedTree.take();
}

EgcNode* EgcFormulaEntity::cut(EgcNode& node)
{
        if (!isNodeInFormula(node))
                return nullptr;

        QScopedPointer<EgcNode> cutTree;

        if (node.getNodeType() == EgcNodeType::BaseNode)
                return nullptr;

        EgcNode* parent = node.getParent();
        if (!parent)
                return nullptr;

        EgcContainerNode* cParent = static_cast<EgcContainerNode*>(parent);
        quint32 index;
        bool isChild = cParent->getIndexOfChild(node, index);
        if (!isChild)
                return nullptr;

        QScopedPointer<EgcNode> emtpyNode(new EgcEmptyNode());
        if (emtpyNode.isNull())
                return nullptr;

        //replace all references to node with the references to emptyNode in this class and its subclasses
//        bool rightSide;
//        EgcNode* newCursorPos;
//        if (isScreenIterInSubtree(node, rightSide)) {
//                newCursorPos = emtpyNode.data();
//        } else if (!m_scrIter.isNull()) {
//                newCursorPos = const_cast<EgcNode*>(m_scrIter->node());
//                rightSide = m_scrIter->rightSide();
//        }

        cutTree.reset(cParent->takeOwnership(node));

        if (!cParent->setChild(index, *emtpyNode.take()))
                return nullptr;
        cParent->getChild(index)->provideParent(cParent);

//        if (!m_scrIter.isNull())
//                m_scrIter->updatePointer(newCursorPos, rightSide);

        return cutTree.take();
}

bool EgcFormulaEntity::paste(EgcNode& treeToPaste, EgcNode& whereToPaste)
{
        if (isNodeInFormula(treeToPaste))
                return false;

        if (treeToPaste.getNodeType() == EgcNodeType::BaseNode)
                return false;

        EgcNode* parent = whereToPaste.getParent();
        if (!parent)
                return false;
        if(treeToPaste.getParent())
                return false;
        if(&treeToPaste == &whereToPaste)
                return false;

        EgcContainerNode* cParent = static_cast<EgcContainerNode*>(parent);
        quint32 index;
        bool isChild = cParent->getIndexOfChild(whereToPaste, index);
        if (!isChild)
                return false;

        //replace all references to node with the references to emptyNode in this class and its subclasses
//        bool rightSide;
//        EgcNode* newCursorPos;
//        if (isScreenIterInSubtree(whereToPaste, rightSide)) {
//                newCursorPos = &treeToPaste;
//        } else if (!m_scrIter.isNull()) {
//                newCursorPos = const_cast<EgcNode*>(m_scrIter->node());
//                rightSide = m_scrIter->rightSide();
//        }

        QScopedPointer<EgcNode> cutTree;
        cutTree.reset(cParent->takeOwnership(whereToPaste));
        cutTree.reset();

        if (!cParent->setChild(index, treeToPaste))
                        return false;
        treeToPaste.provideParent(cParent);

//        if (!m_scrIter.isNull())
//                m_scrIter->updatePointer(newCursorPos, rightSide);

        return true;
}

bool EgcFormulaEntity::isNodeInFormula(EgcNode& node) 
{
        quint32 index;

        if (m_data.hasSubNode(node, index))
                return true;

        return false;
}

bool EgcFormulaEntity::isScreenIterInSubtree(EgcNode& tree, bool &rightSide) const
{
//        EgcNode* node = nullptr;
        
//        if (!m_scrIter.isNull())
//                node = const_cast<EgcNode*>(m_scrIter->node());
//        else
//                return false;

//        if (!node)
//                return false;

//        rightSide = m_scrIter->rightSide();

//        if (node == &tree)
//                return true;

//        if (tree.isContainer()) {
//                quint32 index;
//                EgcContainerNode* container = static_cast<EgcContainerNode*>(&tree);
//                if (container->hasSubNode(*node, index)) {
//                        if ((container->getNumberChildNodes() / 2) >= index)
//                                rightSide = true;
//                        else
//                                rightSide = false;

//                        return true;
//                }
//        }

        return false;
}

//check if the given node is a result node (activate this if insert and remove have been defined)
bool EgcFormulaEntity::isResultNode(const EgcNode& node) 
{
        bool retval = false;
        EgcNode* child = const_cast<EgcNode*>(&node);
        EgcNode* parent = node.getParent();

        EgcNodeType type = child->getNodeType();

        if (type == EgcNodeType::EqualNode)
                return true;
                
        while(    type != EgcNodeType::EqualNode
               && type != EgcNodeType::BaseNode
               && parent) {
                type = parent->getNodeType();

                if (type == EgcNodeType::EqualNode) {
                        quint32 ind;
                        static_cast<EgcEqualNode*>(parent)->getIndexOfChild(*child, ind);
                        if (ind == 1) {
                                retval = true;
                                break;
                        }
                }
                child = parent;
                parent = parent->getParent();
        }

        return retval;
}

bool EgcFormulaEntity::cursorAtBegin(void)
{
        if (m_mod) {
                return m_mod->cursorAtBegin();
        }

        return false;
}

bool EgcFormulaEntity::cursorAtEnd(void)
{
        if (m_mod) {
                return m_mod->cursorAtEnd();
        }

        return false;
}

void EgcFormulaEntity::setSelected(bool selected)
{
        m_item->selectFormula(selected);
}

bool EgcFormulaEntity::isEmpty(void) const
{
        EgcNode* node = getRootElement();
        if (!node)
                return true;
        if (node->getNodeType() == EgcNodeType::EmptyNode)
                return true;
        else { // if there are only invisible (unary) containers and the only leaf is an empty node
                EgcContainerNode* container;
                while(node->isContainer() && !node->hasVisibleSigns()) {
                        container = static_cast<EgcContainerNode*>(node);
                        if (container->getNumberChildNodes() == 1)
                                node = static_cast<EgcContainerNode*>(node)->getChild(0);
                        else
                                break;
                        if (!node->isContainer() && node->getNodeType() == EgcNodeType::EmptyNode) {
                                return true;
                        }

                }
        }

        return false;
}

EgcAbstractFormulaItem* EgcFormulaEntity::getItem(void) const
{
        return m_item;
}

void EgcFormulaEntity::setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide)
{
//        EgcNode* node = m_mathmlLookup.findNode(nodeId);
//        if (node == nullptr)
//                return;

//        m_scrIter->setCursorAt(node, subPos, rightSide);
//        showCurrentCursor();
}

EgcScrPosIterator* EgcFormulaEntity::getIterator(void)
{
//        return m_scrIter.data();
}
