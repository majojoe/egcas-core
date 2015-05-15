#ifndef EGCEMPTYNODE_H
#define EGCEMPTYNODE_H

#include "../concreteNodes/egcvariablenode.h"

/**
 * @brief The EgcEmptyNode class is a class that holds the leafes that are empty until now
 */
class EgcEmptyNode : public EgcVariableNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcEmptyNode, EgcNodeType::EmptyNode);

public:
        ///std constructor
        EgcEmptyNode();
        ///destructor
        virtual ~EgcEmptyNode();
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
        virtual void setValue(const QString& varName, const QString& subscript) override {(void) varName; (void) subscript;}
        /**
         * @brief getSubscript returns the subscript of a variable name
         * @return the subscript of this variable object
         */
        virtual QString& getSubscript(void) override {return EgcVariableNode::getSubscript();}
        /**
         * @brief getStuffedVar returns the stuffed variable name (with subscript)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and variable name and subscript is seperated via "_")
         */
        virtual QString getStuffedVar(void) override {return QString::null;}
};


#endif // EGCEMPTYNODE_H
