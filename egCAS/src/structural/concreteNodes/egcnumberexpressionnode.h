#ifndef EGCNUMBEREXPRESSIONNODE_H
#define EGCNUMBEREXPRESSIONNODE_H

#include <QtGlobal>
#include "../egcexpressionnode.h"

/**
 * @brief The EgcNumberExpressionNode class is a class that holds leafes with numbers
 */
class EgcNumberExpressionNode : public EgcExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::NumberNode);

public:
        ///std constructor
        EgcNumberExpressionNode();
        ///destructor
        virtual ~EgcNumberExpressionNode();
        /**
         * @brief setValue set the value of this leaf
         * @param value the number value to be set
         */
        void setValue(qreal value);
        /**
         * @brief getValue returns the value saved in this class
         * @return the value of this object
         */
        qreal getValue(void);
private:
        qreal m_value;
};

#endif // EGCNUMBEREXPRESSIONNODE_H
