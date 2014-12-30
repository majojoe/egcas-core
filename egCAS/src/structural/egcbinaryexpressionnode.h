#ifndef EGCBINARYEXPRESSIONNODE_H
#define EGCBINARYEXPRESSIONNODE_H

#include "egccontainernode.h"

/**
 * @brief The EgcBinaryExpressionNode class is a base class for an expression that takes two arguments (binary) as subexpressions.
 * This can be e.g. a multiplication or substraction.
 */
class EgcBinaryExpressionNode : public EgcContainerNode
{
public:
        ///std contructor
        EgcBinaryExpressionNode();
        ///copy constructor
        EgcBinaryExpressionNode(const EgcBinaryExpressionNode& orig);
        ///destructor
        virtual ~EgcBinaryExpressionNode() = 0;
        /**
         * @brief setLeftChild set the left child of this binary expression (two childs can be set since this is a binary).
         * @param expression a reference to the child expression given.
         */
        void setLeftChild(const EgcExpressionNode& expression);
        /**
         * @brief setRightChild set the right child of this binary expression (two childs can be set since this is a binary).
         * @param expression a reference to the child expression given.
         */
        void setRightChild(const EgcExpressionNode& expression);
        /**
         * @brief getLeftChild get a pointer to the left child
         * @return NULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcExpressionNode* getLeftChild(void) const;
        /**
         * @brief getRightChild get a pointer to the right child
         * @returnNULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcExpressionNode* getRightChild(void) const;
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBinaryExpressionNode& operator=(const EgcBinaryExpressionNode &rhs);
        /**
         * @brief valid checks if the subnodes are valid. This can be the case if e.g. the childs are not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @returntrue if it is a binary expression, false otherwise
         */
        virtual bool isBinaryExpression(void);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcExpressionNode* child);
        /**
         * @brief isLeaf checks if the current node is a leaf (there are no childs)
         * @return true if it is a leaf, false otherwise
         */
        virtual bool isLeaf(void) const;
        /**
         * @brief transferChilds transfers all properties (childs and parent) from the node "from" to the node "to".
         * After this operation has succeeded the node "from" has no childs or parent anymore. The operation is only
         * possible if the node "to" has no childs at all and the nodes "from" and "to" are of the same type (all are
         * unary or binary types). The node "to" takes ownership of the childs of the node "from".
         * @param to the node to transfer the childs to.
         * @return true if the operation succeeded, false if the nodes are of different types or the node "to" is not
         * empty.
         */
        virtual bool transferPropertiesTo(EgcExpressionNode &to);

protected:
        EgcExpressionNode* m_rightChild;
        EgcExpressionNode* m_leftChild;
};

#endif // EGCBINARYEXPRESSIONNODE_H
