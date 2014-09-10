#include "egcbinaryexpressionnode.h"
#include "egcexpressionnodecreator.h"

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
}

EgcBinaryExpressionNode::~EgcBinaryExpressionNode()
{
        if (m_leftChild)
                delete m_leftChild;

        if (m_rightChild)
                delete m_rightChild;
}

void EgcBinaryExpressionNode::setLeftChild(const EgcExpressionNode& expression)
{
        if (m_leftChild)
                delete m_leftChild;
        m_leftChild = const_cast<EgcExpressionNode*>(&expression);
}

void EgcBinaryExpressionNode::setRightChild(const EgcExpressionNode& expression)
{
        if (m_rightChild)
                delete m_rightChild;
        m_rightChild = const_cast<EgcExpressionNode*>(&expression);
}

EgcExpressionNode* EgcBinaryExpressionNode::getLeftChild(void)
{
        return m_leftChild;
}

EgcExpressionNode* EgcBinaryExpressionNode::getRightChild(void)
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
        EgcExpressionNode *originalChildLeft = const_cast<EgcBinaryExpressionNode&>(rhs).getLeftChild();
        EgcExpressionNode *originalChildRight = const_cast<EgcBinaryExpressionNode&>(rhs).getRightChild();
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
