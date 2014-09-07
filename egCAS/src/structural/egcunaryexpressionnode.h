#ifndef EGCUNARYEXPRESSIONNODE_H
#define EGCUNARYEXPRESSIONNODE_H

#include "egcexpressionnode.h"

/**
 * @brief The EgcUnaryExpressionNode
 * pressionNode class is a base class for an expression that takes one argument (unary) as subexpression.
 * This can be e.g. the not operator.
 */
class EgcUnaryExpressionNode : public EgcExpressionNode
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
protected:
        EgcExpressionNode* m_child;
};

#endif // EGCUNARYEXPRESSIONNODE_H
