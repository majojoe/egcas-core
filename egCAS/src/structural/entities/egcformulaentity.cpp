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

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
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
#include "casKernel/parser/abstractkernelparser.h"
#include "casKernel/parser/restructparserprovider.h"

quint8 EgcFormulaEntity::s_stdNrSignificantDigits = 0;
int EgcFormulaEntity::s_fontSize = 20;

EgcFormulaEntity::EgcFormulaEntity(EgcNodeType type) : m_numberSignificantDigits(0),
                                                       m_numberResultType(EgcNumberResultType::StandardType),
                                                       m_item(nullptr),
                                                       m_isActive(false)
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
                                                           m_item(nullptr)
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
                                                                   m_item(nullptr)
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
                                                              m_item(nullptr)
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

        return tmp;
}

QString EgcFormulaEntity::getCASKernelCommand(void)
{
        //delete the error message in case of a recalculation
        if (m_item)
                m_item->clearErrorMessage();
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

        if (m_mod)
                return false;
        
        //reset error message of the formula
        if (m_item)
                m_item->clearErrorMessage();
        
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

EgcAbstractFormulaItem*EgcFormulaEntity::getItem()
{
        return m_item;
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
        if (m_item)
                m_item->setErrorMessage(msg);
}

void EgcFormulaEntity::handleAction(const EgcAction& action)
{
        switch (action.m_op) {
        case EgcOperations::formulaActivated:
                m_isActive = true;
                updateView();
                m_mod.reset(new FormulaModificator(*this));
                showCurrentCursor();
                break;
        case EgcOperations::formulaDeactivated:
                m_isActive = false;
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
                if (m_mod && m_item)
                        m_mod->markParent();
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
        case EgcOperations::homePressed:
                if (m_mod && m_item)
                        m_mod->toFront();
                break;
        case EgcOperations::endPressed:
                if (m_mod && m_item)
                        m_mod->toBack();
                break;
        case EgcOperations::createSubscript:
                if (m_mod && m_item)
                        m_mod->createSubscript();
                break;
        }
}

void EgcFormulaEntity::showCurrentCursor(void)
{
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

        cutTree.reset(cParent->takeOwnership(node));

        if (!cParent->setChild(index, *emtpyNode.take()))
                return nullptr;
        cParent->getChild(index)->provideParent(cParent);

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

        QScopedPointer<EgcNode> cutTree;
        cutTree.reset(cParent->takeOwnership(whereToPaste));
        cutTree.reset();

        if (!cParent->setChild(index, treeToPaste))
                        return false;
        treeToPaste.provideParent(cParent);

        return true;
}

bool EgcFormulaEntity::isNodeInFormula(EgcNode& node) 
{
        quint32 index;

        if (m_data.hasSubNode(node, index))
                return true;

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

EgcAbstractFormulaItem* EgcFormulaEntity::getItem(void) const
{
        return m_item;
}

void EgcFormulaEntity::setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide)
{
        if (m_mod)
                m_mod->setCursorPos(nodeId, subPos, rightSide);
        showCurrentCursor();
}

bool EgcFormulaEntity::isActive() const
{
        return m_isActive;
}

void EgcFormulaEntity::serialize(QXmlStreamWriter& stream, SerializerProperties &properties)
{
        stream.writeStartElement("formula_entity");
        stream.writeAttribute("pos_x", QString("%1").arg(getPosition().x()));
        stream.writeAttribute("pos_y", QString("%1").arg(getPosition().y()));
        EgcNumberResultType type = getNumberResultType();
        switch (type) {
        case EgcNumberResultType::StandardType:
                stream.writeAttribute("type", QLatin1String("standard"));
                break;
        case EgcNumberResultType::IntegerType:
                stream.writeAttribute("type", QLatin1String("integer"));
                break;
        case EgcNumberResultType::EngineeringType:
                stream.writeAttribute("type", QLatin1String("engineering"));
                break;
        case EgcNumberResultType::ScientificType:
                stream.writeAttribute("type", QLatin1String("scientific"));
                break;
        }
        stream.writeAttribute("digits", QString("%1").arg(getNumberOfSignificantDigits()));

        m_data.serialize(stream, properties);

        stream.writeEndElement(); // formula_entity
}

void EgcFormulaEntity::deserialize(QXmlStreamReader& stream, SerializerProperties& properties)
{
        (void) properties;

        if (stream.name() == QLatin1String("formula_entity")) {
                QXmlStreamAttributes attr = stream.attributes();
                if (attr.hasAttribute("type")) {
                        QString str = attr.value("type").toString();
                        if (str == QLatin1String("standard"))
                                setNumberResultType(EgcNumberResultType::StandardType);
                        if (str == QLatin1String("engineering"))
                                setNumberResultType(EgcNumberResultType::EngineeringType);
                        if (str == QLatin1String("integer"))
                                setNumberResultType(EgcNumberResultType::IntegerType);
                        if (str == QLatin1String("scientific"))
                                setNumberResultType(EgcNumberResultType::ScientificType);
                }
                if (attr.hasAttribute("pos_x") && attr.hasAttribute("pos_y")) {
                        qreal x = attr.value("pos_x").toFloat();
                        qreal y = attr.value("pos_y").toFloat();
                        setPosition(QPointF(x, y));
                }
                if (attr.hasAttribute("digits")) {
                        quint8 d = static_cast<quint8>(attr.value("digits").toUInt());
                        setNumberOfSignificantDigits(d);
                }

                stream.readNextStartElement();
                if (stream.name() != QLatin1String("basenode"))
                        stream.raiseError();

                m_data.deserialize(stream, properties);
                if (stream.name() == QLatin1String("basenode"))
                        stream.skipCurrentElement();
        }

        updateView();
}

bool EgcFormulaEntity::aboutToBeDeleted() const
{
        if (m_mod)
                return m_mod->aboutToBeDeleted();
        return true;
}

