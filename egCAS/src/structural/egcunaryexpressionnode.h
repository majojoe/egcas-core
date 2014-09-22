#ifndef EGCUNARYEXPRESSIONNODE_H
#define EGCUNARYEXPRESSIONNODE_H

#include "egccontainernode.h"

/**
 * @brief The EgcUnaryExpressionNode
 * pressionNode class is a base class for an expression that takes one argument (unary) as subexpression.
 * This can be e.g. the not operator.
 */
class EgcUnaryExpressionNode : public EgcContainerNode
{
public:
        ///std constructor
        EgcUnaryExpressionNode();
        ///copy constructor
        EgcUnaryExpressionNode(const EgcUnaryExpressionNode& orig);
        /// destructor
        virtual ~EgcUnaryExpressionNode() = 0;
        /**
         * @brief setChild set the child of this unary expression (only one child can be set).
         * This function takes ownership of the child and will delete the child automatically in its destructor.
         * @param expression a reference to the child expression given.
         */
        void setChild(const EgcExpressionNode&expression);
        /**
         * @brief getChild get a pointer to the child
         * @return NULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcExpressionNode* getChild(void);
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcUnaryExpressionNode& operator=(const EgcUnaryExpressionNode &rhs);
        /**
         * @brief valid checks if the subnode is valid. This can be the case if e.g. the child is not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isUnaryExpression returns if the current element is a unary expression (container) or not
         * @return true if it is a unary expression, false otherwise
         */
        virtual bool isUnaryExpression(void);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcExpressionNode* child);
protected:
        EgcExpressionNode* m_child;
};

#endif // EGCUNARYEXPRESSIONNODE_H
