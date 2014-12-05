#ifndef EGCEMPTYEXPRESSIONNODE_H
#define EGCEMPTYEXPRESSIONNODE_H

#include "egcvariableexpressionnode.h"

/**
 * @brief The EgcEmptyExpressionNode class is a class that holds the leafes that are empty until now
 */
class EgcEmptyExpressionNode : public EgcVariableExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::EmptyNode);

public:
        ///std constructor
        EgcEmptyExpressionNode();
        ///destructor
        virtual ~EgcEmptyExpressionNode();
        /**
                         * @brief getValue returns the variable name (without subscript)
                         * @return the variable name
                         */
        QString& getValue(void);
        /**
                         * @brief valid returns true if the expression is valid and false otherwise.
                         * A variable expression is valid if the value is not empty.
                         * @return true if the expression is valid, false otherwise.
                         */
        virtual bool valid(void);
private:
        /**
                         * @brief setValue set the variable name (value)
                         * @param varName the variable name as a string
                         * @param subscript the subscript of the variable if any
                         */
        virtual void setValue(QString& varName, QString& subscript) override {(void) varName; (void) subscript;}
        /**
                         * @brief getSubscript returns the subscript of a variable name
                         * @return the subscript of this variable object
                         */
        virtual QString& getSubscript(void) override {return EgcVariableExpressionNode::getSubscript();}
        /**
                         * @brief getStuffedVar returns the stuffed variable name (with subscript)
                         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
                         * and variable name and subscript is seperated via "_")
                         */
        virtual QString getStuffedVar(void) override {return QString::null;}
};


#endif // EGCEMPTYEXPRESSIONNODE_H
