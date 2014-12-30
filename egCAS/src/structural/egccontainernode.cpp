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
