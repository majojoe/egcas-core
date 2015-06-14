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

bool EgcNode::isUnaryNode(void)
{
        return false;
}

bool EgcNode::isBinaryNode(void)
{
        return false;
}

EgcContainerNode *EgcNode::getParent(void)
{
        return m_parent;
}

void EgcNode::provideParent(EgcContainerNode* parent)
{
        m_parent = parent;
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

bool EgcNode::isFlexNode(void) const
{
        return false;
}
