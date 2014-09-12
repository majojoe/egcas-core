#ifndef EGCBINARYEXPRESSIONNODE_H
#define EGCBINARYEXPRESSIONNODE_H

#include "egcexpressionnode.h"

/**
 * @brief The EgcBinaryExpressionNode class is a base class for an expression that takes two arguments (binary) as subexpressions.
 * This can be e.g. a multiplication or substraction.
 */
class EgcBinaryExpressionNode : public EgcExpressionNode
{
public:
        ///std contructor
        EgcBinaryExpressionNode();
        ///copy constructor
        EgcBinaryExpressionNode(const EgcBinaryExpressionNode& orig);
        ///destructor
        ~EgcBinaryExpressionNode() = 0;
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
        EgcExpressionNode* getLeftChild(void);
        /**
         * @brief getRightChild get a pointer to the right child
         * @returnNULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcExpressionNode* getRightChild(void);
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
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void);
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @returntrue if it is a binary expression, false otherwise
         */
        virtual bool isBinaryExpression(void);
protected:
        EgcExpressionNode* m_rightChild;
        EgcExpressionNode* m_leftChild;
};

#endif // EGCBINARYEXPRESSIONNODE_H
