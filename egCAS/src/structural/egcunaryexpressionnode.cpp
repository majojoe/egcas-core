#include "egcunaryexpressionnode.h"

EgcUnaryExpressionNode::EgcUnaryExpressionNode() : m_child(nullptr)
{

}

EgcUnaryExpressionNode::~EgcUnaryExpressionNode()
{
        if (m_child)
                delete m_child;
}

void EgcUnaryExpressionNode::setChild(const EgcExpressionNode& expression)
{
        if (m_child)
                delete m_child;
        m_child = const_cast<EgcExpressionNode*>(&expression);
}

EgcExpressionNode* EgcUnaryExpressionNode::getChild(void)
{
        return m_child;
}
