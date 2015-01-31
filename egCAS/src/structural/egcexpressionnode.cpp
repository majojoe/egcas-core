#include "egccontainernode.h"
#include "visitor/egcnodevisitor.h"

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

bool EgcExpressionNode::isContainer(void) const
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

void EgcExpressionNode::provideParent(EgcContainerNode* parent)
{
        m_parent = parent;
}

bool EgcExpressionNode::isLeaf(void) const
{
        if (isContainer())
                return false;
        else
                return true;
}

void EgcExpressionNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}
