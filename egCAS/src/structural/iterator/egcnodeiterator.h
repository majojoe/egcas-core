#ifndef EGCNODEITERATOR_H
#define EGCNODEITERATOR_H

class EgcFormulaExpression;
class EgcNode;
class EgcBaseNode;
enum class EgcNodeType;


enum class EgcIteratorState {
        LeftIteration = 0, MiddleIteration, RightIteration
};

class EgcNodeIterator
{
public:
        /// constructor for initialization with formula
        EgcNodeIterator(const EgcFormulaExpression& formula);
        /// constructor for initialization with tree element
        EgcNodeIterator(const EgcNode & node);
        /// std destructor
        virtual ~EgcNodeIterator();
        /**
         * @brief findNext Searches for an EgcNodeType node starting from the current iterator position in forward direction.
         * If type was found after the call, the iterator is positioned just after the matching item.
         * otherwise, the iterator is positioned at the back of the container.
         * @param type the type of the node to be found.
         * @return returns true if value is found, false otherwise
         */
        bool findNext(EgcNodeType type);
        /**
         * @brief findPrevious Searches for an EgcNodeType node starting from the current iterator position in backward direction.
         * If type was found after the call, the iterator is positioned just before the matching item.
         * otherwise, the iterator is positioned at the front of the container.
         * @param type the type of the node to be found.
         * @return returns true if value is found, false otherwise
         */
        bool findPrevious(EgcNodeType type);
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
         * @return a reference to the next item.
         */
        virtual EgcNode & next(void);
        /**
         * @brief getState retruns the state of the last traversion
         * @return state the state of the node we jumped over
         */
        virtual EgcIteratorState getLastState(void) const;
        /**
         * @brief peekNext Returns the next node without incrementing the iterator.
         * @return a reference to the next item.
         */
        EgcNode & peekNext(void) const;
        /**
         * @brief peekPrevious Returns the previous node without decrementing the iterator.
         * @return a reference to the previous item.
         */
        EgcNode & peekPrevious(void) const;
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous item.
         */
        virtual EgcNode & previous(void);
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
        virtual bool insert(EgcNodeType type);
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
        virtual EgcNode* replace(EgcNode& node, EgcNodeType type);
        /**
         * @brief nextParent increments the iterator to the parent of the last node it was jumped over.
         * @return the parent of the last item it was jumped over.
         */
        virtual EgcNode& nextParent(void);

protected:
        /**
         * @brief getNextElement returns the next element in the tree
         * @param currentNext the currently next node in the tree
         * @param currentPrev the currently previous node in the tree
         * @param restart an error while parsing the tree ocurred, so restart at the beginning of the tree
         * @return a pointer to the next (following of currentNext) element
         */
        virtual EgcNode& getNextElement(EgcNode& currentNext, EgcNode &currentPrev, bool &restart) const;
        /**
         * @brief getPreviousElement get the previous element in the tree
         * @param atBeginning true if beginning of the tree has been reached
         * @param atEnd true if the end of the tree has been reached
         * @param state enumeration to be able to decide which node visit next
         * @return a pointer to the pevious element
         */
        virtual EgcNode& getPreviousElement(bool* atBeginning, bool* atEnd, EgcIteratorState* state) const;
        /**
         * @brief isRightChild checks if the given child is the right child of the given parent
         * @param parent reference to the parent to test for
         * @param child reference to the child to test for
         * @return true if child is the right child of the given parent, false otherwise
         */
        bool isRightChild(EgcNode& parent, EgcNode& child) const;
        /**
         * @brief isLeftChild checks if the given child is the left child of the given parent
         * @param parent reference to the parent to test for
         * @param child reference to the child to test for
         * @return true if child is the left child of the given parent, false otherwise
         */
        bool isLeftChild(EgcNode& parent, EgcNode& child) const;
        /**
         * @brief findNextRightMostLeaf finds the next right most leaf at the bottom of the tree
         * @param start the starting point from where to search
         * @return the found leaf (this can be a container without childs or a real leaf)
         */
        EgcNode& findNextRightMostLeaf(EgcNode& start) const;
        /**
         * @brief determineFollowingState determines the following state upon the following node
         * @param previous previous node
         * @param next the following node
         * @param forward true if iteration is in forward direction, false if reverse direction
         * @return the next state upon the following node
         */
        EgcIteratorState determineFollowingState(EgcNode &previous, EgcNode &next, bool forward) const;

        EgcNode* m_next;                        ///< pointer to next data element in the tree structure
        EgcNode* m_previous;                    ///< pointer to previous data element in the tree structure
        EgcBaseNode* m_baseElement;             ///< pointer to data element at the root of the tree structure
        bool m_atBegin;                         ///< iterator is at the beginning of the tree
        bool m_atEnd;                           ///< iterator is at the end of the tree
        EgcIteratorState m_state;               ///< reflects the iterator state to know where to go next time


};


#endif // EGCNODEITERATOR_H
