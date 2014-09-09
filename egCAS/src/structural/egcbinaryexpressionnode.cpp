#include "egcbinaryexpressionnode.h"
#include "egcexpressionnodecreator.h"

EgcBinaryExpressionNode::EgcBinaryExpressionNode() : m_rightChild(nullptr), m_leftChild(nullptr)
{
}

EgcBinaryExpressionNode::EgcBinaryExpressionNode(const EgcBinaryExpressionNode& orig)
{
        EgcExpressionNode *originalChildLeft = const_cast<EgcBinaryExpressionNode&>(orig).getLeftChild();
        EgcExpressionNode *originalChildRight = const_cast<EgcBinaryExpressionNode&>(orig).getRightChild();
        m_leftChild = EgcExpressionNodeCreator::copy(*originalChildLeft);
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
        delete m_leftChild;
        delete m_rightChild;
        //and create a new one
        EgcExpressionNode *originalChildLeft = const_cast<EgcBinaryExpressionNode&>(rhs).getLeftChild();
        EgcExpressionNode *originalChildRight = const_cast<EgcBinaryExpressionNode&>(rhs).getRightChild();
        m_leftChild = EgcExpressionNodeCreator::copy(*originalChildLeft);
        m_rightChild = EgcExpressionNodeCreator::copy(*originalChildRight);

        return *this;
}
