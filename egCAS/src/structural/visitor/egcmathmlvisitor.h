#ifndef EGCMATHMLVISITOR_H
#define EGCMATHMLVISITOR_H

#include <QString>
#include "egcnodevisitor.h"

/**
 * @brief The EgcMathMlVisitor class is a visitor class for parsing the tree and output expressions formatted for the
 * used cas kernel.
 */

class EgcMathMlVisitor : public EgcNodeVisitor
{
public:
        /**
         * @brief EgcNodeVisitor std constructor for the visitor
         * @param formula the formula to be parsed
         */
        EgcMathMlVisitor(EgcFormulaExpression& formula);
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcBinaryNode* binary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcUnaryNode* unary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcNode* node) override;
        /**
         * @brief getResult returns the result of the traversion of the tree
         * @return the result of the traversion as string
         */
        virtual QString getResult(void) override;

};

#endif // EGCMATHMLVISITOR_H
