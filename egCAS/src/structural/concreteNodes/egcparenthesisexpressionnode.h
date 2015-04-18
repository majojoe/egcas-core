#ifndef EGCPARENTHESISEXPRESSIONNODE_H
#define EGCPARENTHESISEXPRESSIONNODE_H

#include "../specialNodes/egcunaryexpressionnode.h"

/**
 * @brief The EgcParenthesisExpressionNode class is a class that reflects parenthesis' in an equation. This manages e.g. something like 3x*(4+8y).
 */
class EgcParenthesisExpressionNode : public EgcUnaryExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcParenthesisExpressionNode, EgcExpressionNodeType::ParenthesisNode);
public:
        EgcParenthesisExpressionNode();
};

#endif // EGCPARENTHESISEXPRESSIONNODE_H
