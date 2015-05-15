#ifndef EGCNUMBERNODE_H
#define EGCNUMBERNODE_H

#include <QtGlobal>
#include <QString>
#include "../specialNodes/egcnode.h"

/**
 * @brief The EgcNumberNode class is a class that holds leafes with numbers
 */
class EgcNumberNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcNumberNode, EgcNodeType::NumberNode);

public:
        ///std constructor
        EgcNumberNode();
        ///destructor
        virtual ~EgcNumberNode();
        /**
         * @brief setValue set the value of this leaf
         * @param value the number value to be set
         */
        void setValue(const QString& value);
        /**
         * @brief getValue returns the value saved in this class
         * @return the value of this object
         */
        QString& getValue(void);
private:
        QString m_value;
};

#endif // EGCNUMBERNODE_H
