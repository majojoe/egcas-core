#include "egccontainernode.h"

EgcExpressionNode::EgcExpressionNode() : m_parent(nullptr)
{
}

EgcExpressionNode::~EgcExpressionNode()
{
        notifyContainerOnChildDeletion(this);
}

bool EgcExpressionNode::valid(void)
{
        return true;
}

bool EgcExpressionNode::isContainer(void)
{
        return false;
}

bool EgcExpressionNode::isUnaryExpression(void)
{
        return false;
}

bool EgcExpressionNode::isBinaryExpression(void)
{
        return false;
}

EgcExpressionNode* EgcExpressionNode::getParent(void)
{
        return m_parent;
}

void EgcExpressionNode::provideParent(EgcContainerNode& parent)
{
        m_parent = &parent;
}
