#include "egccontainernode.h"

EgcContainerNode::EgcContainerNode()
{
}

EgcContainerNode::~EgcContainerNode()
{
}

bool EgcContainerNode::isContainer(void) const
{
        return true;
}

bool EgcContainerNode::transferPropertiesTo(EgcExpressionNode &to)
{
        (void) to;

        return false;
}

void EgcContainerNode::adjustChildPointers(EgcExpressionNode &old_child, EgcExpressionNode &new_child)
{
        (void) old_child;
        (void) new_child;
}
