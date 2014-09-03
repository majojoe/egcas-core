#ifndef EGCROOTEXPRESSIONNODE_H
#define EGCROOTEXPRESSIONNODE_H

#include "egcbinaryexpressionnode.h"

/**
 * @brief The EgcRootExpressionNode class is a class for root operations. This manages e.g. something like sqrt(10).
 */
class EgcRootExpressionNode : public EgcBinaryExpressionNode
{
public:
        EgcRootExpressionNode();
};

#endif // EGCROOTEXPRESSIONNODE_H
