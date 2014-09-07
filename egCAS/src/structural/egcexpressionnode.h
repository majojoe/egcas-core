#ifndef EGCEXPRESSIONNODE_H
#define EGCEXPRESSIONNODE_H

/**
 * @brief The EgcExpressionNodeType enum is a enum to differentiate the different node types
 */
enum class EgcExpressionNodeType
{
        NumberNode = 0, VariableNode, RootNode, NodeUndefined
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
        /**
         * @brief getNodeType returns the node type of the derived class
         * @return the node type of this class.
         */
        EgcExpressionNodeType getNodeType(void);
private:
        static const EgcExpressionNodeType s_nodeType = EgcExpressionNodeType::NodeUndefined;
};

#endif // EGCEXPRESSIONNODE_H
