#ifndef EGCPARENTHESISNODE_H
#define EGCPARENTHESISNODE_H

#include "../specialNodes/egcunarynode.h"

/**
 * @brief The EgcParenthesisNode class is a class that reflects parenthesis' in an equation. This manages e.g. something like 3x*(4+8y).
 */
class EgcParenthesisNode : public EgcUnaryNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcParenthesisNode, EgcNodeType::ParenthesisNode);
public:
        EgcParenthesisNode();
};

#endif // EGCPARENTHESISNODE_H
