#ifndef EGCEXPRESSIONNODE_H
#define EGCEXPRESSIONNODE_H

/**
 * @brief The EgcExpressionNodeType enum is a enum to differentiate the different node types
 */
enum class EgcExpressionNodeType
{
    NodeUndefined = 0, NumberNode, VariableNode, RootNode
};

/**
 * @brief The EgcExpressionNode class defines the base class for all expressions
 * (multiplication, substraction, ...) in the expression tree.
 * The class is built up as composite pattern.
 */
class EgcExpressionNode
{
public:
        EgcExpressionNode();
        virtual ~EgcExpressionNode() = 0;
private:
        static const EgcExpressionNodeType s_nodeType = EgcExpressionNodeType::NodeUndefined;
};

#endif // EGCEXPRESSIONNODE_H
