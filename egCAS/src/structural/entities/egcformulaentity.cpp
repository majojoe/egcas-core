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
#include "visitor/egcmathmlvisitor.h"
#include "egcabstractformulaitem.h"
#include "egcabstractentitylist.h"
#include "actions/egcaction.h"
#include "iterator/egcscrpositerator.h"

quint8 EgcFormulaEntity::s_stdNrSignificantDigits = 15;

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
        if (m_scrIter)
                m_scrIter->finishFormulaChange();

        return tmp;
}

QString EgcFormulaEntity::getCASKernelCommand(void)
{
        //delete the error message in case of a recalculation
        m_errorMsg.clear();
        EgcMaximaVisitor maximaVisitor(*this);
        return maximaVisitor.getResult();
}

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

        if (m_scrIter)
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
                if (m_scrIter)
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
        setGenericFontSize(size);
}

void EgcFormulaEntity::setFontSize(int size)
{
        if (!m_item)
                return;

        m_item->setFontSize(size);
}

int EgcFormulaEntity::getGenericFontSize(void)
{
        return getGenericFontSize();
}

int EgcFormulaEntity::getFontSize(void)
{
        if (!m_item)
                return 0;

        return m_item->getFontSize();
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
        }

        if (changeType == EgcItemChangeType::contentChanged) {
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
                m_scrIter.reset(new EgcScrPosIterator(*this));
                showCurrentCursor();
                break;
        case EgcOperations::formulaDeactivated:
                m_scrIter.reset();
                break;
        case EgcOperations::cursorForward:
                moveCursor(true);
                break;
        case EgcOperations::cursorBackward:
                moveCursor(false);
                break;
        case EgcOperations::spacePressed:
                markParent();
                break;
        case EgcOperations::alnumKeyPressed:
                insertCharacter(action.m_character);
                break;
        case EgcOperations::backspacePressed:
                removeCharacter(true);
                break;
        case EgcOperations::delPressed:
                removeCharacter(false);
                break;
        case EgcOperations::mathOperator:
                insertOperation(action.m_character);
                break;
        case EgcOperations::homePressed:
                if (m_scrIter) {
                        m_scrIter->toFront();
                        m_scrIter->resetUnderline();
                        showCurrentCursor();
                }
                break;
        case EgcOperations::endPressed:
                if (m_scrIter) {
                        m_scrIter->toBack();
                        m_scrIter->resetUnderline();
                        showCurrentCursor();
                }
                break;
        }
}

void EgcFormulaEntity::insertOperation(QChar operation)
{
        if (!m_scrIter)
                return;
        if (!m_item)
                return;

        m_scrIter->insertOp(operation);
        m_item->hideCursors();
        m_item->updateView();
}

void EgcFormulaEntity::insertCharacter(QChar character)
{
        if (!m_scrIter)
                return;
        if (!m_item)
                return;

        m_scrIter->insert(character);
        m_item->hideCursors();
        m_item->updateView();        
}

void EgcFormulaEntity::removeCharacter(bool before)
{
        bool structureChanged;

        if (!m_scrIter)
                return;
        if (!m_item)
                return;

        if (before)
                m_scrIter->backspace(structureChanged);
        else
                m_scrIter->remove(structureChanged);

        if (structureChanged) {
                rearrangePrecedence();
        }
        //update m_scrIter since nothing is correct about the position
        m_item->updateView();
        m_item->hideCursors();
        showCurrentCursor();
}

void EgcFormulaEntity::moveCursor(bool forward)
{
        if (!m_scrIter)
                return;
        if (!m_item)
                return;
        if (forward) {
                if (m_scrIter->hasNext())
                        (void) m_scrIter->next();
                else
                        m_scrIter->resetUnderline();
        } else {
                if (m_scrIter->hasPrevious())
                        (void) m_scrIter->previous();
                else
                        m_scrIter->resetUnderline();
        }

        showCurrentCursor();
        if (m_scrIter->isUnderlineActive())
                m_item->showUnderline(m_scrIter->id());
}

void EgcFormulaEntity::showCurrentCursor(void)
{
        quint32 id;
        qint32 ind;
        bool rightSide;

        if (!m_scrIter)
                return;
        if (!m_item)
                return;

        id = m_scrIter->id();
        ind = m_scrIter->subIndex();
        if (ind < 0) { //this is a container
                ind = 0;
        } else { //this is a glyph
                ind++;
        }
        rightSide = m_scrIter->rightSide();

        m_item->hideCursors();

        if (rightSide)
                m_item->showRightCursor(id, ind);
        else
                m_item->showLeftCursor(id, ind);
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
        quint32 id;
        qint32 ind;
        bool rightSide;

        if (!m_scrIter)
                return;
        if (!m_item)
                return;

        rightSide = m_scrIter->rightSide();

        id = m_scrIter->getNextVisibleParent();
        ind = m_scrIter->subIndex();
        if (ind < 0) { //this is a container
                ind = 0;
        } else { //this is a glyph
                ind++;
        }
        if (rightSide)
                m_item->showRightCursor(id, ind);
        else
                m_item->showLeftCursor(id, ind);

        m_item->showUnderline(id);
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
        bool rightSide;
        EgcNode* newCursorPos;
        if (isScreenIterInSubtree(node, rightSide)) {
                newCursorPos = emtpyNode.data();
        } else if (!m_scrIter.isNull()) {
                newCursorPos = const_cast<EgcNode*>(m_scrIter->node());
                rightSide = m_scrIter->rightSide();
        }

        cutTree.reset(cParent->takeOwnership(node));

        if (!cParent->setChild(index, *emtpyNode.take()))
                return nullptr;
        cParent->getChild(index)->provideParent(cParent);

        if (!m_scrIter.isNull())
                m_scrIter->updatePointer(&node, newCursorPos, rightSide);

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
        bool rightSide;
        EgcNode* newCursorPos;
        if (isScreenIterInSubtree(whereToPaste, rightSide)) {
                newCursorPos = &treeToPaste;
        } else if (!m_scrIter.isNull()) {
                newCursorPos = const_cast<EgcNode*>(m_scrIter->node());
                rightSide = m_scrIter->rightSide();
        }

        QScopedPointer<EgcNode> cutTree;
        cutTree.reset(cParent->takeOwnership(whereToPaste));
        cutTree.reset();

        if (!cParent->setChild(index, treeToPaste))
                        return false;
        treeToPaste.provideParent(cParent);

        if (!m_scrIter.isNull())
                m_scrIter->updatePointer(&whereToPaste, newCursorPos, rightSide);

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
        EgcNode* node = nullptr;
        
        if (!m_scrIter.isNull())
                node = const_cast<EgcNode*>(m_scrIter->node());
        else
                return false;

        if (!node)
                return false;

        rightSide = m_scrIter->rightSide();

        if (node == &tree)
                return true;

        if (tree.isContainer()) {
                quint32 index;
                EgcContainerNode* container = static_cast<EgcContainerNode*>(&tree);
                if (container->hasSubNode(*node, index)) {
                        if ((container->getNumberChildNodes() / 2) >= index)
                                rightSide = true;
                        else
                                rightSide = false;

                        return true;
                }
        }

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

void EgcFormulaEntity::rearrangePrecedence(void)
{
        bool rearranged;


        if (m_scrIter) {
                m_scrIter->lockDelayedCursorUpdate();
                m_scrIter->toFront();
        }

        do {
                rearranged = rearrangeOnePrecedence();
        } while(!rearranged);

        if (m_scrIter)
                m_scrIter->unlockDelayedCursorUpdate();
}

bool EgcFormulaEntity::rearrangeOnePrecedence(void)
{
        EgcNodeIterator iter(*this);
        EgcNode* node;

        while(iter.hasNext()) {
                node = &iter.next();
                if (!node->isContainer())
                        continue;
                EgcContainerNode* container = static_cast<EgcContainerNode*>(node);
                if (container->getNumberChildNodes() != 2)
                        continue;
                EgcContainerNode* lchild = nullptr;
                EgcContainerNode* rchild = nullptr;
                if (container->getChild(0)->isContainer())
                        lchild = static_cast<EgcContainerNode*>(container->getChild(0));
                if (container->getChild(1)->isContainer())
                        rchild = static_cast<EgcContainerNode*>(container->getChild(1));

                if (lchild) {
                        if (    container->bindingPower() > lchild->bindingPower() && lchild->isOperation()
                             || (container->bindingPower() == lchild->bindingPower() && container->isOperation()
                                 && !container->isLeftAssociative())) {
                                if (rotateTreeRight(*container))
                                        return false;
                        }
                }

                if (rchild) {
                        if (    container->bindingPower() > rchild->bindingPower() && rchild->isOperation()
                             || (container->bindingPower() == rchild->bindingPower() && container->isOperation()
                                 && container->isLeftAssociative())) {
                                if (rotateTreeLeft(*container))
                                        return false;
                        }
                }
        }

        return true;
}

bool EgcFormulaEntity::rotateTreeRight(EgcNode& treeNodeToRotate)
{
        if (!treeNodeToRotate.isContainer())
                return false;
        EgcContainerNode& rotContainer = static_cast<EgcContainerNode&>(treeNodeToRotate);
        if (rotContainer.getNumberChildNodes() != 2)
                return false;   //only binary nodes can be rotated for now
        EgcNode* lchild = rotContainer.getChild(0);
        if (!lchild)
                return false;
        if (!lchild->isContainer())
                return false;
        EgcContainerNode& lcontainer = static_cast<EgcContainerNode&>(*lchild);
        if (!lcontainer.isOperation())
                return false;
        if (lcontainer.getNumberChildNodes() != 2)
                return false;
        EgcNode* rchild = lcontainer.getChild(1);
        if(!rchild)
                return false;
        if (!treeNodeToRotate.getParent())
                return false;
        EgcContainerNode& parent = *treeNodeToRotate.getParent();
        quint32 pos;
        if(!parent.getIndexOfChild(treeNodeToRotate, pos))
                return false;

        QScopedPointer<EgcNode> b(cut(*rchild));
        QScopedPointer<EgcContainerNode> p(static_cast<EgcContainerNode*>(cut(*lchild)));
        QScopedPointer<EgcContainerNode> q(static_cast<EgcContainerNode*>(cut(treeNodeToRotate)));

        if (!paste(*b.take(), *q->getChild(0)))
                return false;
        if (!paste(*q.take(), *p->getChild(1)))
                return false;
        if (!paste(*p.take(), *parent.getChild(pos)))
                return false;

        return true;
}

bool EgcFormulaEntity::rotateTreeLeft(EgcNode& treeNodeToRotate)
{
        if (!treeNodeToRotate.isContainer())
                return false;
        EgcContainerNode& rotContainer = static_cast<EgcContainerNode&>(treeNodeToRotate);
        if (rotContainer.getNumberChildNodes() != 2)
                return false;   //only binary nodes can be rotated for now
        EgcNode* rchild = rotContainer.getChild(1);
        if (!rchild)
                return false;
        if (!rchild->isContainer())
                return false;
        EgcContainerNode& rcontainer = static_cast<EgcContainerNode&>(*rchild);
        if (!rcontainer.isOperation())
                return false;
        if (rcontainer.getNumberChildNodes() != 2)
                return false;
        EgcNode* lchild = rcontainer.getChild(0);
        if(!lchild)
                return false;
        if (!treeNodeToRotate.getParent())
                return false;
        EgcContainerNode& parent = *treeNodeToRotate.getParent();
        quint32 pos;
        if(!parent.getIndexOfChild(treeNodeToRotate, pos))
                return false;

        QScopedPointer<EgcNode> b(cut(*lchild));
        QScopedPointer<EgcContainerNode> q(static_cast<EgcContainerNode*>(cut(*rchild)));
        QScopedPointer<EgcContainerNode> p(static_cast<EgcContainerNode*>(cut(treeNodeToRotate)));

        if (!paste(*b.take(), *p->getChild(1)))
                return false;
        if (!paste(*p.take(), *q->getChild(0)))
                return false;
        if (!paste(*q.take(), *parent.getChild(pos)))
                return false;

        return true;
}

bool EgcFormulaEntity::cursorAtBegin(void)
{
        if (!m_scrIter->hasPrevious())
                return true;

        return false;
}

bool EgcFormulaEntity::cursorAtEnd(void)
{
        if (!m_scrIter->hasNext())
                return true;

        return false;
}
