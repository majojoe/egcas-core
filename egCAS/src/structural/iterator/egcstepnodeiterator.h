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
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next item.
         */
        virtual EgcExpressionNode & next(void) override;
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous item.
         */
        virtual EgcExpressionNode & previous(void) override;
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        void toFront(void);

protected:
        enum class internalIteratorState {
                gotoLeft = 0, gotoRight, gotoParent
        };

        /**
         * @brief getNextElement returns the next element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @return a pointer to the next element
         */
        virtual EgcExpressionNode& getNextElement(bool* atBeginning, bool* atEnd) const override;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @return a pointer to the pevious element
         */
        virtual EgcExpressionNode& getPreviousElement(bool* atBeginning, bool* atEnd) const override;
        /**
         * @brief getNextElement returns the next element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the next element
         */
        virtual EgcExpressionNode& _getNextElement(bool* atBeginning, bool* atEnd, internalIteratorState* state) const;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the pevious element
         */
        virtual EgcExpressionNode& _getPreviousElement(bool* atBeginning, bool* atEnd, internalIteratorState* state) const;

private:        
        EgcExpressionNode* m_previousCursor[2];         ///< the previous elements that was pointed to by the cursor
        internalIteratorState m_internalState;          ///< reflects the internal iterator state to know where to go next time
};

#endif // EGCSTEPNODEITERATOR_H
