#ifndef EGCNODEEXPRESSIONITERATOR_H
#define EGCNODEEXPRESSIONITERATOR_H

class EgcFormulaExpression;
class EgcExpressionNode;
class EgcBaseExpressionNode;
enum class EgcExpressionNodeType;


enum class EgcNodeIteratorState {
        LeftIteration = 0, MiddleIteration, RightIteration
};

class EgcExpressionNodeIterator
{
public:
        /// constructor for initialization with formula
        EgcExpressionNodeIterator(const EgcFormulaExpression& formula);
        /// constructor for initialization with tree element
        EgcExpressionNodeIterator(const EgcExpressionNode & node);
        /// std destructor
        virtual ~EgcExpressionNodeIterator();
        /**
         * @brief findNext Searches for an EgcExpressionNodeType node starting from the current iterator position in forward direction.
         * If type was found after the call, the iterator is positioned just after the matching item.
         * otherwise, the iterator is positioned at the back of the container.
         * @param type the type of the node to be found.
         * @return returns true if value is found, false otherwise
         */
        bool findNext(EgcExpressionNodeType type);
        /**
         * @brief findPrevious Searches for an EgcExpressionNodeType node starting from the current iterator position in backward direction.
         * If type was found after the call, the iterator is positioned just before the matching item.
         * otherwise, the iterator is positioned at the front of the container.
         * @param type the type of the node to be found.
         * @return returns true if value is found, false otherwise
         */
        bool findPrevious(EgcExpressionNodeType type);
        /**
         * @brief hasNext Checks if there is at most one more item after the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasNext(void) const;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @return True if an item was found, false otherwise.
         */
        virtual bool hasPrevious(void) const;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @param state the state of the node we jumped over
         * @return a reference to the next item.
         */
        virtual EgcExpressionNode & next(EgcNodeIteratorState &state);
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next item.
         */
        EgcExpressionNode & peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous item.
         */
        EgcExpressionNode & peekPrevious(void) const;
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @param state the state of the node we jumped over
         * @return a refererence to the previous item.
         */
        virtual EgcExpressionNode & previous(EgcNodeIteratorState &state);
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void);
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
        /**
         * @brief replace replace the given node by the type given
         * @param node the node to be replaced
         * @param type the node type to use instead
         * @return true if replacement was possible, false otherwise
         */
        virtual EgcExpressionNode* replace(EgcExpressionNode& node, EgcExpressionNodeType type);
        /**
         * @brief nextParent increments the iterator to the parent of the last node it was jumped over.
         * @return the parent of the last item it was jumped over.
         */
        virtual EgcExpressionNode& nextParent(void);

protected:
        /**
         * @brief getNextElement returns the next element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the next element
         */
        virtual EgcExpressionNode& getNextElement(bool* atBeginning, bool* atEnd, EgcNodeIteratorState* state) const;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the pevious element
         */
        virtual EgcExpressionNode& getPreviousElement(bool* atBeginning, bool* atEnd, EgcNodeIteratorState* state) const;
        /**
         * @brief isRightChild checks if the given child is the right child of the given parent
         * @param parent reference to the parent to test for
         * @param child reference to the child to test for
         * @return true if child is the right child of the given parent, false otherwise
         */
        bool isRightChild(EgcExpressionNode& parent, EgcExpressionNode& child) const;
        /**
         * @brief isLeftChild checks if the given child is the left child of the given parent
         * @param parent reference to the parent to test for
         * @param child reference to the child to test for
         * @return true if child is the left child of the given parent, false otherwise
         */
        bool isLeftChild(EgcExpressionNode& parent, EgcExpressionNode& child) const;
        /**
         * @brief findNextRightMostLeaf finds the next right most leaf at the bottom of the tree
         * @param start the starting point from where to search
         * @return the found leaf (this can be a container without childs or a real leaf)
         */
        EgcExpressionNode& findNextRightMostLeaf(EgcExpressionNode& start) const;
        /**
         * @brief determineFollowingState determines the following state upon the following node
         * @param current current node
         * @param following the following node
         * @param forwardDirection if the iteration is in forward direction, set true here, false otherwise
         * @return the next state upon the following node
         */
        EgcNodeIteratorState determineFollowingState(EgcExpressionNode &current, EgcExpressionNode &following,
                                                     bool forwardDirection) const;

        EgcExpressionNode* m_cursor;            ///< pointer to data element in the tree structure
        EgcBaseExpressionNode* m_baseElement;   ///< pointer to data element at the root of the tree structure
        bool m_atBegin;                         ///< iterator is at the beginning of the tree
        bool m_atEnd;                           ///< iterator is at the end of the tree
        EgcExpressionNode* m_history;           ///< pointer to node we jumped over last time
        EgcNodeIteratorState m_State;           ///< reflects the iterator state to know where to go next time
        bool m_forward;                         ///< true if tree is traversed in forward direction
        EgcExpressionNode* m_previousCursor;    ///< a pointer to the previous node


};


#endif // EGCNODEEXPRESSIONITERATOR_H
