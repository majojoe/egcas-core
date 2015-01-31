#ifndef EGCNODEVISITOR_H
#define EGCNODEVISITOR_H

#include <QString>
#include "../iterator/egcexpressionnodeiterator.h"


class EgcFormulaExpression;
class EgcBinaryExpressionNode;
class EgcUnaryExpressionNode;
class EgcExpressionNode;

/**
 * @brief The EgcNodeVisitor class is a base class for all visitors to parse information from a tree
 */
class EgcNodeVisitor
{
public:
        /**
         * @brief EgcNodeVisitor std constructor for the visitor
         * @param formula the formula to be parsed
         */
        EgcNodeVisitor(EgcFormulaExpression& formula);
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcBinaryExpressionNode* binary) = 0;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcUnaryExpressionNode* unary) = 0;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcExpressionNode* node) = 0;
        /**
         * @brief getResult returns the result of the traversion of the tree
         * @return the result of the traversion as string
         */
        virtual QString getResult(void);
protected:
        QString m_result;                       ///< saves the result of the information extracted.
        EgcFormulaExpression *m_formula;        ///< the formula to with the nodes to work on
        EgcNodeIteratorState m_state;           ///< the current state (helps to extract the correct information from tree)

};

#endif // EGCNODEVISITOR_H
