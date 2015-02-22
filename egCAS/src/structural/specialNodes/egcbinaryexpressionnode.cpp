#include "egcbinaryexpressionnode.h"
#include "../egcexpressionnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcBinaryExpressionNode::EgcBinaryExpressionNode() : m_rightChild(nullptr), m_leftChild(nullptr)
{
}

EgcBinaryExpressionNode::EgcBinaryExpressionNode(const EgcBinaryExpressionNode& orig)
{
        m_leftChild = nullptr;
        m_rightChild = nullptr;
        EgcExpressionNode *originalChildLeft = const_cast<EgcBinaryExpressionNode&>(orig).getLeftChild();
        EgcExpressionNode *originalChildRight = const_cast<EgcBinaryExpressionNode&>(orig).getRightChild();
        if (originalChildLeft)
                m_leftChild = EgcExpressionNodeCreator::copy(*originalChildLeft);        
        if (originalChildRight)
                m_rightChild = EgcExpressionNodeCreator::copy(*originalChildRight);

        //set the parents also
        if(originalChildLeft)
                originalChildLeft->provideParent(this);
        if(originalChildRight)
                originalChildRight->provideParent(this);
}

EgcBinaryExpressionNode::~EgcBinaryExpressionNode()
{
        if (m_leftChild) {
                delete m_leftChild;
                m_leftChild = nullptr;
        }

        if (m_rightChild) {
                delete m_rightChild;
                m_rightChild = nullptr;
        }
}

void EgcBinaryExpressionNode::setLeftChild(const EgcExpressionNode& expression)
{
        if (m_leftChild)
                delete m_leftChild;
        m_leftChild = const_cast<EgcExpressionNode*>(&expression);

        if (m_leftChild)
                m_leftChild->provideParent(this);
}

void EgcBinaryExpressionNode::setRightChild(const EgcExpressionNode& expression)
{
        if (m_rightChild)
                delete m_rightChild;
        m_rightChild = const_cast<EgcExpressionNode*>(&expression);

        if (m_rightChild)
                m_rightChild->provideParent(this);
}

EgcExpressionNode* EgcBinaryExpressionNode::getLeftChild(void) const
{
        return m_leftChild;
}

EgcExpressionNode* EgcBinaryExpressionNode::getRightChild(void) const
{
        return m_rightChild;
}

EgcBinaryExpressionNode& EgcBinaryExpressionNode::operator=(const EgcBinaryExpressionNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_leftChild) {
                delete m_leftChild;
                m_leftChild = nullptr;
        }
        if (m_rightChild) {
                delete m_rightChild;
                m_rightChild = nullptr;
        }
        //and create a new one
        EgcExpressionNode *originalChildLeft = rhs.getLeftChild();
        EgcExpressionNode *originalChildRight = rhs.getRightChild();
        if (originalChildLeft)
                m_leftChild = EgcExpressionNodeCreator::copy(*originalChildLeft);
        if (originalChildRight)
                m_rightChild = EgcExpressionNodeCreator::copy(*originalChildRight);

        return *this;
}

bool EgcBinaryExpressionNode::valid(void)
{
        if (m_leftChild && m_rightChild)
                if (m_leftChild->valid() && m_rightChild->valid())
                        return true;

        return false;
}

bool EgcBinaryExpressionNode::isBinaryExpression(void)
{
        return true;
}

void EgcBinaryExpressionNode::notifyContainerOnChildDeletion(EgcExpressionNode* child)
{
        if (m_leftChild == child)
                m_leftChild = nullptr;
        if (m_rightChild == child)
                m_rightChild = nullptr;
}

bool EgcBinaryExpressionNode::isLeaf(void) const
{
        if (m_leftChild == nullptr && m_rightChild == nullptr)
                return true;
        else
                return false;
}

bool EgcBinaryExpressionNode::transferPropertiesTo(EgcExpressionNode &to)
{
        bool retval = false;
        EgcBinaryExpressionNode &to_bin = static_cast<EgcBinaryExpressionNode&>(to);

        if (to_bin.m_rightChild == nullptr && to_bin.m_leftChild == nullptr) {
                if (to.isBinaryExpression()) {
                        EgcContainerNode *parent_container;
                        retval = true;
                        to_bin.m_rightChild = m_rightChild;
                        to_bin.m_leftChild = m_leftChild;
                        to_bin.m_parent = m_parent;
                        parent_container = static_cast<EgcContainerNode*>(m_parent);
                        parent_container->adjustChildPointers(*this, to);
                        m_leftChild = nullptr;
                        m_rightChild = nullptr;
                        m_parent = nullptr;
                }
        }

        return retval;
}

void EgcBinaryExpressionNode::adjustChildPointers(EgcExpressionNode &old_child, EgcExpressionNode &new_child)
{
        if (m_leftChild == &old_child)
                m_leftChild = &new_child;
        else if (m_rightChild == &old_child)
                m_rightChild = &new_child;
}

EgcExpressionNode* EgcBinaryExpressionNode::takeOwnership(EgcExpressionNode &child)
{
        EgcExpressionNode* retval = nullptr;

        if (m_leftChild == &child) {
                m_leftChild = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        } else if (m_rightChild == &child) {
                m_rightChild = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcBinaryExpressionNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}
