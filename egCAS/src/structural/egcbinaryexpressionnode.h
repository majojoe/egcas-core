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
protected:
        EgcExpressionNode* m_rightChild;
        EgcExpressionNode* m_leftChild;
};

#endif // EGCBINARYEXPRESSIONNODE_H
