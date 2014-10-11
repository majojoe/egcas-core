#ifndef EGCSTEPNODEITERATOR_H
#define EGCSTEPNODEITERATOR_H

#include "egcexpressionnodeiterator.h"

enum class EgcStepIteratorState {
        LeftIteration = 0, MiddleIteration, RightIteration
};

/**
 * @brief The EgcStepNodeIterator class iterates over the tree stopping at each node regardless if it has already been
 * visited.
 */
class EgcStepNodeIterator : public EgcExpressionNodeIterator
{
public:
        /// std constructor
        EgcStepNodeIterator(EgcFormulaExpression& formula);
        /// std destructor
        virtual ~EgcStepNodeIterator();
        /**
         * @brief getIterationState returns the iteration state of the recently returned node.
         * If the node is a binary node and has been recently traversed via the left link, it is in the LeftIteration
         * state. If the node is a binary node and has been recently traversed from its left child to itsself, it is in
         * the Middle Iteration state. If a binary node is traversed via its right link it is in the RightIteration
         * state. A unary node can only be in Left- or RightIteration state and a leaf can only be in MiddleIteration
         * state.
         * @return returns the state of the current node;
         */
        EgcStepIteratorState getIterationState(void);
protected:
        /**
         * @brief getNextElement returns the next element in the tree
         * @return a pointer to the next element
         */
        virtual EgcExpressionNode& getNextElement(bool* atBeginning, bool* atEnd) const;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @return a pointer to the pevious element
         */
        virtual EgcExpressionNode& getPreviousElement(bool* atBeginning, bool* atEnd) const;

private:
        EgcStepIteratorState m_iteratorState;
};

#endif // EGCSTEPNODEITERATOR_H
