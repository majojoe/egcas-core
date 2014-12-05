#ifndef EGCVARIABLEEXPRESSIONNODE_H
#define EGCVARIABLEEXPRESSIONNODE_H

#include <QString>
#include "egcexpressionnode.h"

/**
 * @brief The EgcVariableExpressionNode class is a class that holds the leafes with variable names
 */
class EgcVariableExpressionNode : public EgcExpressionNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcExpressionNodeType::VariableNode);

public:
        ///std constructor
        EgcVariableExpressionNode();
        ///destructor
        virtual ~EgcVariableExpressionNode();
        /**
         * @brief setValue set the variable name (value)
         * @param varName the variable name as a string
         * @param subscript the subscript of the variable if any
         */
        virtual void setValue(QString& varName, QString& subscript);
        /**
         * @brief getValue returns the variable name (without subscript)
         * @return the variable name
         */
        QString& getValue(void);
        /**
         * @brief getSubscript returns the subscript of a variable name
         * @return the subscript of this variable object
         */
        virtual QString& getSubscript(void);
        /**
         * @brief getStuffedVar returns the stuffed variable name (with subscript)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and variable name and subscript is seperated via "_")
         */
        virtual QString getStuffedVar(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
protected:
        QString m_value;          ///< the variable name used
        QString m_subscript;      ///< the subscript if any        
};

#endif // EGCVARIABLEEXPRESSIONNODE_H
