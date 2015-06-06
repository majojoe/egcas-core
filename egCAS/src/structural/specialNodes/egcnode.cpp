#include "egccontainernode.h"
#include "../visitor/egcnodevisitor.h"

EgcNode::EgcNode() : m_parent(nullptr)
{
}

EgcNode::~EgcNode()
{
        notifyContainerOnChildDeletion(this);
}

bool EgcNode::valid(void)
{
        return true;
}

bool EgcNode::isContainer(void) const
{
        return false;
}

bool EgcNode::isUnaryExpression(void)
{
        return false;
}

bool EgcNode::isBinaryExpression(void)
{
        return false;
}

EgcNode* EgcNode::getParent(void)
{
        return m_parent;
}

void EgcNode::provideParent(EgcContainerNode* parent)
{
        m_parent = parent;
}

bool EgcNode::isLeaf(void) const
{
        if (isContainer())
                return false;
        else
                return true;
}

void EgcNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

bool EgcNode::isChild(EgcNode& parent)
{
        if (&parent == m_parent)
                return true;
        else
                return false;
}
