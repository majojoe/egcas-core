#ifndef EGCROOTEXPRESSIONNODE_H
#define EGCROOTEXPRESSIONNODE_H

#include "../egcbinaryexpressionnode.h"

/**
 * @brief The EgcRootExpressionNode class is a class for root operations. This manages e.g. something like sqrt(10).
 */
class EgcRootExpressionNode : public EgcBinaryExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::RootNode);
public:
        EgcRootExpressionNode();
};

#endif // EGCROOTEXPRESSIONNODE_H
