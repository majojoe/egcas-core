#ifndef EGCEXPRESSIONNODE_H
#define EGCEXPRESSIONNODE_H

/**
 * @brief The EgcExpressionNodeType enum is a enum to differentiate the different node types
 */
enum class EgcExpressionNodeType
{
        NumberNode = 0, VariableNode, RootNode, NodeUndefined
};

/** macro for setting the expression type of a class. Change this if you want to have the type
 * changed in a subclass */
#define EGC_SET_EXPRESSION_TYPE(type)                                                           \
public:                                                                                         \
        virtual EgcExpressionNodeType getNodeType(void) {return s_nodeType;}                    \
protected:                                                                                      \
        static const EgcExpressionNodeType s_nodeType = type


/**
 * @brief The EgcExpressionNode class defines the base class for all expressions
 * (multiplication, substraction, ...) in the expression tree.
 * The class is built up as composite pattern.
 */
class EgcExpressionNode
{
        //set the node type of this expression. Set this in each subclass in which you want to have the type changed.
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::NodeUndefined);

public:
        EgcExpressionNode();
        virtual ~EgcExpressionNode() = 0;
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * An expression is valid if all nodes are valid.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
};

#endif // EGCEXPRESSIONNODE_H
