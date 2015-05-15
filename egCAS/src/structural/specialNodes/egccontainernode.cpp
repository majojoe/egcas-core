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
