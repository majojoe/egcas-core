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
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void) override;
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void) override;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next item.
         */
        virtual EgcExpressionNode & next(EgcStepIteratorState &state);
        /**
         * @brief this is not implemented in the step iterator since that makes no sense
         * @return returns always the root element
         */
        virtual EgcExpressionNode & previous(EgcStepIteratorState &state);
        /**
         * @brief insert inserts the given item at the current position.
         * @param type the node type to be inserted
         * @return false if insertiation in the tree is not possible (e.g. not a containter type), true otherwise
         */
        virtual bool insert(EgcExpressionNodeType type);
        /**
         * @brief remove removes the last item it was jumped over.
         */
        virtual void remove();

protected:
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
        virtual EgcExpressionNode& _getNextElement(bool* atBeginning, bool* atEnd, EgcStepIteratorState*state) const;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the pevious element
         */
        virtual EgcExpressionNode& _getPreviousElement(bool* atBeginning, bool* atEnd, EgcStepIteratorState* state) const;

private:        
        /**
         * @brief determineFollowingState determines the following state upon the following node
         * @param current current node
         * @param following the following node
         * @param forwardDirection if the iteration is in forward direction, set true here, false otherwise
         * @return the next state upon the following node
         */
        EgcStepIteratorState determineFollowingState(EgcExpressionNode &current, EgcExpressionNode &following, bool forwardDirection) const;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next item.
         */
        virtual EgcExpressionNode & next(void) override;
        /**
         * @brief this is not implemented in the step iterator since that makes no sense
         * @return returns always the root element
         */
        virtual EgcExpressionNode & previous(void) override;

        EgcStepIteratorState m_State;          ///< reflects the iterator state to know where to go next time
        bool m_forward;                        ///< true if tree is traversed in forward direction
        EgcExpressionNode* m_previousCursor;   ///< a pointer to the previous node
};

#endif // EGCSTEPNODEITERATOR_H
