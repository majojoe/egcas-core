#ifndef EGCNODEEXPRESSIONITERATOR_H
#define EGCNODEEXPRESSIONITERATOR_H

class EgcFormulaExpression;
class EgcExpressionNode;
enum class EgcExpressionNodeType;

class EgcExpressionNodeIterator
{
public:
        /// std constructor
        EgcExpressionNodeIterator(EgcFormulaExpression&formula);
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
        bool hasNext(void) const;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @return True if an item was found, false otherwise.
         */
        bool hasPrevious(void) const;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @return a reference to the next item.
         */
        EgcExpressionNode & next(void);
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
         * @return a refererence to the previous item.
         */
        EgcExpressionNode & previous(void);
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        void toFront(void);
        /**
         * @brief operator= Sets the iterator to operate on the tree given. The iterator is set before the first item.
         * @param tree the tree given to operate on.
         * @return the iterator set for the list.
         */
        EgcExpressionNodeIterator & operator=(const EgcFormulaExpression & tree);
        /**
         * @brief operator= Sets the iterator to operate on the tree given. The iterator is set before the first item.
         * @param node the node given to point to.
         * @return the iterator set for the list.
         */
        EgcExpressionNodeIterator & operator=(const EgcExpressionNode & node);
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
private:
        /**
         * @brief findNextRightMostLeaf finds the next right most leaf at the bottom of the tree
         * @param start the starting point from where to search
         * @return the found leaf (this can be a container without childs or a real leaf)
         */
        EgcExpressionNode& findNextRightMostLeaf(EgcExpressionNode& start) const;

        EgcExpressionNode* m_cursor;            ///< pointer to data element in the tree structure
        EgcExpressionNode* m_rootElement;       ///< pointer to data element at the root of the tree structure
        bool m_atBegin;                           ///< iterator is at the beginning of the tree
        bool m_atEnd;                             ///< iterator is at the end of the tree
};


#endif // EGCNODEEXPRESSIONITERATOR_H
