#include <QSharedPointer>
#include "egcexpressionnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"
#include "../egcbinaryexpressionnode.h"
#include "../egcunaryexpressionnode.h"
#include "../egcbaseexpressionnode.h"
#include "../egcexpressionnodecreator.h"

EgcExpressionNodeIterator::EgcExpressionNodeIterator(const EgcFormulaExpression& formula) :
        m_cursor(formula.getBaseElement().getChild()), m_baseElement(&formula.getBaseElement()),
        m_atBegin(true), m_atEnd(false), m_history(m_baseElement)
{
}

EgcExpressionNodeIterator::EgcExpressionNodeIterator(const EgcExpressionNode & node)
{
        EgcExpressionNode* tempNode = const_cast<EgcExpressionNode*>(&node);
        EgcExpressionNode* parent = tempNode->getParent();
        while (parent) {
                tempNode = parent;
                parent = tempNode->getParent();
        };

        m_cursor = const_cast<EgcExpressionNode*>(&node);
        m_baseElement = static_cast<EgcBaseExpressionNode*>(tempNode);
        m_atBegin = true;
        m_atEnd = false;
}

EgcExpressionNodeIterator::~EgcExpressionNodeIterator()
{
}

bool EgcExpressionNodeIterator::findNext(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode* node;
        bool found = false;

        while (iter.hasNext()) {
                node = &(iter.next());
                if (node->getNodeType() == type) {
                        found = true;
                        m_cursor = iter.m_cursor;
                        break;
                }
        }

        if (!found)
                this->toBack();

        return found;
}

bool EgcExpressionNodeIterator::findPrevious(EgcExpressionNodeType type)
{
        EgcExpressionNodeIterator iter = *this;
        EgcExpressionNode* node;
        bool found = false;

        while (iter.hasPrevious()) {
                node = &(iter.previous());
                if (node->getNodeType() == type) {
                        found = true;
                        m_cursor = iter.m_cursor;
                        break;
                }
        }

        if (!found)
                this->toFront();

        return found;
}

bool EgcExpressionNodeIterator::hasNext(void) const
{
        EgcExpressionNode *rootNode = m_baseElement->getChild();
        if (rootNode) {
                if (rootNode->isContainer()) {
                        if (rootNode->isBinaryExpression()) {
                                EgcBinaryExpressionNode* binary = static_cast<EgcBinaryExpressionNode*>(rootNode);
                                if (    (binary->getLeftChild() == nullptr)
                                                && (binary->getRightChild() == nullptr))
                                        return false;
                        } else { //unary container
                                if (static_cast<EgcUnaryExpressionNode*>(rootNode)->getChild() == nullptr)
                                        return false;
                        }
                } else {
                        return false;
                }
        } else {
                return false;
        }

        return !m_atEnd;
}

bool EgcExpressionNodeIterator::hasPrevious(void) const
{
        EgcExpressionNode *rootNode = m_baseElement->getChild();
        if (rootNode) {
                if (rootNode->isContainer()) {
                        if (rootNode->isBinaryExpression()) {
                                EgcBinaryExpressionNode* binary = static_cast<EgcBinaryExpressionNode*>(rootNode);
                                if (    (binary->getLeftChild() == nullptr)
                                                && (binary->getRightChild() == nullptr))
                                        return false;
                        } else { //unary container
                                if (static_cast<EgcUnaryExpressionNode*>(rootNode)->getChild() == nullptr)
                                        return false;
                        }
                } else {
                        return false;
                }
        } else {
                return false;
        }

        return !m_atBegin;
}

EgcExpressionNode& EgcExpressionNodeIterator::next(void)
{
        //if the base element has no child
        if (!m_baseElement->getChild()) {
                m_history = m_baseElement;
                return *m_baseElement;
        }

        EgcExpressionNode& tempCursor = *m_cursor;
        m_cursor = &getNextElement(nullptr, &m_atEnd);

        m_history = &tempCursor;

        return tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekNext(void) const
{
        if (!m_baseElement->getChild())
                return *m_baseElement;

        return *m_cursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::peekPrevious(void) const
{
        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        return getPreviousElement(nullptr, nullptr);
}

EgcExpressionNode& EgcExpressionNodeIterator::previous(void)
{
        //if the base element has no child
        if (!m_baseElement->getChild()) {
                m_history =  m_baseElement;
                return *m_baseElement;
        }

        m_cursor = &getPreviousElement(&m_atBegin, nullptr);        
        m_history = m_cursor;

        return *m_cursor;
}

void EgcExpressionNodeIterator::toBack(void)
{
        m_cursor = m_baseElement->getChild();
        m_atBegin = false;
        m_atEnd = true;
}

void EgcExpressionNodeIterator::toFront(void)
{
        m_cursor = m_baseElement->getChild();
        m_atBegin = true;
        m_atEnd = false;
}

EgcExpressionNode& EgcExpressionNodeIterator::getNextElement(bool* atBeginning, bool* atEnd) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool searchUpwards = true;
        bool beginning = false;
        bool end = false;

        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        if (tempCursor->isContainer()) {
                //search downwards since this is a containter
                if (tempCursor->isBinaryExpression()) { //this is a binary container
                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        } else { //the left child is null, maybe the right child is not null
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                        searchUpwards = false;
                                }
                        }
                 } else {  //this is a unary container
                        tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                        if (tempPointer) {
                                tempCursor = tempPointer;
                                searchUpwards = false;
                        }
                }
        }

        if (searchUpwards) {
                //search for a binary containter whose node has not been visited yet,
                //since only that one can have another unvisited node.
                do {
                        EgcExpressionNode* parent = tempCursor->getParent();
                        if (parent) {
                                if (parent->getNodeType() != EgcExpressionNodeType::BaseNode) {
                                        if (parent->isBinaryExpression()) {
                                                if (static_cast<EgcBinaryExpressionNode*>(parent)->getLeftChild() == tempCursor) {
                                                        if (static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild()) {
                                                                tempCursor = static_cast<EgcBinaryExpressionNode*>(parent)->getRightChild();
                                                                break; //next element found
                                                        }
                                                }
                                        }
                                        tempCursor = parent;
                                } else { //we are at the root element of the tree
                                        tempCursor = m_baseElement->getChild();
                                        break;
                                }
                        } else { // if the parent pointer is null, this is the base element or the node is invalid
                                //return the root element since the rest of the tree does not really exist or it is
                                //already the base element.
                                tempCursor = m_baseElement->getChild();
                                break;
                        }
                } while (tempCursor != m_baseElement->getChild());
        }
        if (tempCursor == m_baseElement->getChild())
                end = true;

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;

        return *tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::getPreviousElement(bool* atBeginning, bool* atEnd) const
{
        EgcExpressionNode* tempCursor = m_cursor;
        EgcExpressionNode* tempPointer = nullptr;
        bool beginning = false;
        bool end = false;

        //if the base element has no child
        if (!m_baseElement->getChild())
                return *m_baseElement;

        tempPointer = m_cursor->getParent();
        if (!tempPointer) //this normally cannot happen
                return *m_baseElement;


        if (tempPointer->getNodeType() != EgcExpressionNodeType::BaseNode) {
                if (tempPointer->isUnaryExpression()) {
                        tempCursor = tempPointer;
                } else { // is a binary expression
                        if (isRightChild(*tempPointer, *tempCursor)) { // is a right child
                                //check left child
                                EgcBinaryExpressionNode* parent = static_cast<EgcBinaryExpressionNode*>(tempPointer);
                                tempPointer = parent->getLeftChild();
                                if (tempPointer) {
                                        tempCursor = &findNextRightMostLeaf(*tempPointer);
                                } else {
                                        tempCursor = parent;
                                }
                        } else { //is a left child
                                tempCursor = tempPointer;
                        }
                }
        } else {
                //this seems to be the root node, so jump to the back
                tempCursor = &findNextRightMostLeaf(*m_cursor);
        }

        EgcExpressionNode *parent = tempCursor->getParent();
        if (parent) {
                if (parent->getNodeType() == EgcExpressionNodeType::BaseNode)
                        beginning = true;
        }

        if (atBeginning)
                *atBeginning = beginning;
        if (atEnd)
                *atEnd = end;

        return *tempCursor;
}

EgcExpressionNode& EgcExpressionNodeIterator::findNextRightMostLeaf(EgcExpressionNode& start) const
{
        EgcExpressionNode* tempCursor = &start;
        EgcExpressionNode* tempPointer = nullptr;

        do {
                if (tempCursor->isContainer()) {
                        if (tempCursor->isBinaryExpression()) {
                                tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getRightChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else { // check the left child also, maybe this is not null
                                        tempPointer = static_cast<EgcBinaryExpressionNode*>(tempCursor)->getLeftChild();
                                        if (tempPointer) {
                                                tempCursor = tempPointer;
                                        } else { // this binary container has no childs, so this is also a leaf.
                                                break;
                                        }
                                }
                        } else {
                                tempPointer = static_cast<EgcUnaryExpressionNode*>(tempCursor)->getChild();
                                if (tempPointer) {
                                        tempCursor = tempPointer;
                                } else { // we found a unary container without child, so this is a leaf
                                        break;
                                }
                        }
                } else { // we found a leaf, so return
                        break;
                }
        } while (tempPointer);

        return *tempCursor;
}

bool EgcExpressionNodeIterator::isRightChild(EgcExpressionNode& parent, EgcExpressionNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryExpressionNode&>(parent).getRightChild() == &child)
                        return true;
        }

        return false;
}

bool EgcExpressionNodeIterator::isLeftChild(EgcExpressionNode& parent, EgcExpressionNode& child) const
{
        if (parent.isBinaryExpression()) {
                if (static_cast<EgcBinaryExpressionNode&>(parent).getLeftChild() == &child)
                        return true;
        }

        return false;
}

EgcExpressionNode& EgcExpressionNodeIterator::incrementToNextNonChildNode(EgcExpressionNode& start)
{
        EgcExpressionNode* tempNode = &findNextRightMostLeaf(start);
        m_cursor = tempNode;
        m_cursor = &getNextElement(nullptr, nullptr);
        return peekNext();
}

bool EgcExpressionNodeIterator::insert(EgcExpressionNodeType type)
{        
        bool retval = false;

        QScopedPointer<EgcExpressionNode> node (EgcExpressionNodeCreator::create(type));

        // only containers can be inserted in the tree
        if (node->isContainer()) {
                retval = true;
                if (node->isBinaryExpression()) {
                        // set the right child to a empty child
                        QScopedPointer<EgcExpressionNode> tempNode(EgcExpressionNodeCreator::
                                                                  create(EgcExpressionNodeType::EmptyNode));
                        static_cast<EgcBinaryExpressionNode*>(node.data())->setRightChild(*(tempNode.take()));
                }
                //insert the container into the tree
                EgcExpressionNode &nextNode = getNextElement(nullptr, nullptr);
                EgcExpressionNode &previousNode = getPreviousElement(nullptr, nullptr);
                QScopedPointer<EgcExpressionNode> childNode;
                EgcExpressionNode *parentNode;
                if (nextNode.getParent() == &previousNode) {
                        childNode.reset(&nextNode);
                        parentNode = &previousNode;
                } else {
                        parentNode = previousNode.getParent();
                        childNode.reset(&previousNode);
                }
                if (parentNode == nullptr || parentNode == childNode.data()) {
                        parentNode = m_baseElement;
                }

                //set the parent
                if (parentNode->isBinaryExpression()) {
                        if (isRightChild(*parentNode, *childNode)) {
                                static_cast<EgcBinaryExpressionNode*>(parentNode)->setRightChild(*(node.take()));
                        } else {
                                static_cast<EgcBinaryExpressionNode*>(parentNode)->setLeftChild(*(node.take()));
                        }

                } else {
                        static_cast<EgcUnaryExpressionNode*>(parentNode)->setChild(*(node.take()));
                }

                //set the child if one
                if (childNode.data()) {
                        if (node->isBinaryExpression())
                                static_cast<EgcBinaryExpressionNode*>(node.data())->setLeftChild(*(childNode.take()));
                        else
                                static_cast<EgcUnaryExpressionNode*>(node.data())->setChild(*(childNode.take()));
                } else { // insert empty nodes if no child node is present
                        if (node->isBinaryExpression())
                                static_cast<EgcBinaryExpressionNode*>(node.data())->
                                        setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                        else
                                static_cast<EgcUnaryExpressionNode*>(node.data())->
                                        setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));

                }

        } else { // if the next node is a empty node, replace it by the non container type node
#warning code for replacing an empty node is going here

        }

        //repair the node pointer organization data
        m_cursor = node.data();
        if (m_cursor == m_baseElement->getChild()) {
                m_atBegin = true;
                m_atEnd = false;
        } else {
                m_atBegin = false;
                m_atEnd = false;
        }

        return retval;
}

void EgcExpressionNodeIterator::remove()
{
        //the last node jumped over is in m_history
        m_cursor = &incrementToNextNonChildNode(*m_history);
        EgcExpressionNode *parent = m_history->getParent();
        if (parent->isBinaryExpression()) {
                if (isRightChild(*parent, *m_history))
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setRightChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
                else
                        static_cast<EgcBinaryExpressionNode*>(parent)->
                                setLeftChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));

        } else {
                static_cast<EgcUnaryExpressionNode*>(parent)->
                                setChild(*EgcExpressionNodeCreator::create(EgcExpressionNodeType::EmptyNode));
        }
        delete(m_history);
        m_history = m_baseElement;
        if (m_cursor == m_baseElement->getChild()) {
                m_atBegin = true;
                m_atEnd = false;
        } else {
                m_atBegin = false;
                m_atEnd = false;
        }
}

bool EgcExpressionNodeIterator::replace(EgcExpressionNode& node, EgcExpressionNodeType type)
{
        bool replaceable = false;

        QScopedPointer<EgcExpressionNode> replacement(EgcExpressionNodeCreator::create(type));

        if (node.isBinaryExpression() && replacement.data()->isBinaryExpression()) {
                EgcBinaryExpressionNode &bin_node = static_cast<EgcBinaryExpressionNode&>(node);
                if (bin_node.transferPropertiesTo(*static_cast<EgcBinaryExpressionNode*>(replacement.data())))
                        replaceable = true;
        }

        if (node.isUnaryExpression() && replacement.data()->isUnaryExpression()) {
                EgcUnaryExpressionNode &una_node = static_cast<EgcUnaryExpressionNode&>(node);
                if (una_node.transferPropertiesTo(*static_cast<EgcUnaryExpressionNode*>(replacement.data())))
                        replaceable = true;
        }

        if (replaceable) {
                if (m_cursor == &node)
                        m_cursor = replacement.data();
                if (m_history == &node)
                        m_history = replacement.data();                
#warning activate this code for handling previous node in step node iterator
//                if (m_previousCursor == &node)
//                        m_previousCursor = replacement.data();
        }

        return replaceable;
}
