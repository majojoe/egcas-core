#ifndef EGCEXPRESSIONNODE_H
#define EGCEXPRESSIONNODE_H

class EgcContainerNode;

/**
 * @brief The EgcExpressionNodeType enum is a enum to differentiate the different node types
 */
enum class EgcExpressionNodeType
{
        NumberNode = 0, VariableNode, RootNode, ParenthesisNode, NodeUndefined
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
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void);
        /**
         * @brief isUnaryExpression returns if the current element is a unary expression (container) or not
         * @return true if it is a unary expression, false otherwise
         */
        virtual bool isUnaryExpression(void);
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @returntrue if it is a binary expression, false otherwise
         */
        virtual bool isBinaryExpression(void);
        /**
         * @brief getParent returns a pointer to the parent node
         * @return a pointer to the parent node or NULL if this is the root element
         */
        EgcExpressionNode* getParent(void);
        /**
         * @brief provideParent THIS IS NO USER INTERFACE! DO NOT USE THAT FROM USER POINT OF VIEW!
         * This is used to set the parent pointers in the implementation of the tree.
         * @param parent a pointer to the parent of this node. This must be a container pointer.
         */
        void provideParent(EgcContainerNode& parent);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcExpressionNode* child) { (void)child; }
protected:
        EgcExpressionNode *m_parent;    ///< pointer to the parent (is needed for traversing the tree)
};

#endif // EGCEXPRESSIONNODE_H
