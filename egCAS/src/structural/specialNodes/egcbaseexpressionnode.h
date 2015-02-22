#ifndef EGCBASEEXPRESSIONNODE_H
#define EGCBASEEXPRESSIONNODE_H

#include "egcunaryexpressionnode.h"

/**
 * @brief The EgcBaseExpressionNode class is a class that always is the base element of an expression.
 */
class EgcBaseExpressionNode : public EgcUnaryExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::BaseNode);
public:
        EgcBaseExpressionNode();

};

#endif // EGCBASEEXPRESSIONNODE_H
