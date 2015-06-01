#include "egccontainernode.h"

EgcContainerNode::EgcContainerNode()
{
}

EgcContainerNode::EgcContainerNode(const EgcContainerNode& orig)
{
        m_parent = nullptr;
}

EgcContainerNode::~EgcContainerNode()
{
}

bool EgcContainerNode::isContainer(void) const
{
        return true;
}

bool EgcContainerNode::transferPropertiesTo(EgcNode &to)
{
        (void) to;

        return false;
}

void EgcContainerNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        (void) old_child;
        (void) new_child;
}

EgcNode* EgcContainerNode::getChild(quint32 index) const
{
        (void) index;

        return nullptr;
}

bool EgcContainerNode::setChild(quint32 index, const EgcNode& expression)
{
        (void) index;
        (void) expression;
}

quint32 EgcContainerNode::getNumberChildNodes(void) const
{
        return 0;
}

bool EgcContainerNode::isFirstChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::isLastChild(EgcNode &child) const
{
        (void) child;

        return false;
}

