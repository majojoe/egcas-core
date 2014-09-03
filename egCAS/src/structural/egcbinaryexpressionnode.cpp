#include "egcbinaryexpressionnode.h"

EgcBinaryExpressionNode::EgcBinaryExpressionNode() : m_rightChild(nullptr), m_leftChild(nullptr)
{

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
