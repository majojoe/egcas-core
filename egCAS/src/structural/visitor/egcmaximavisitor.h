#ifndef EGCMAXIMAVISITOR_H
#define EGCMAXIMAVISITOR_H

#include <QString>
#include "egcnodevisitor.h"

/**
 * @brief The EgcKernelVisitor class is a visitor class for parsing the tree and output expressions formatted for the
 * used cas kernel.
 */

class EgcMaximaVisitor : public EgcNodeVisitor
{
public:
        /**
         * @brief EgcNodeVisitor std constructor for the visitor
         * @param formula the formula to be parsed
         */
        EgcMaximaVisitor(EgcFormulaExpression& formula);
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcBinaryExpressionNode* binary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcUnaryExpressionNode* unary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcExpressionNode* node) override;
        /**
         * @brief getResult returns the result of the traversion of the tree
         * @return the result of the traversion as string
         */
        virtual QString getResult(void);
};

#endif // EGCMAXIMAVISITOR_H
