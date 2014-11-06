#ifndef EGCNATURALNODEITERATOR_H
#define EGCNATURALNODEITERATOR_H

#include "egcexpressionnodeiterator.h"

/**
 * @brief The EgcNaturalNodeIterator class iterates over the tree in a way a human would read a formula.
 */
class EgcNaturalNodeIterator : public EgcExpressionNodeIterator
{
public:
        /// std constructor
        EgcNaturalNodeIterator(EgcFormulaExpression& formula);
        /// std destructor
        virtual ~EgcNaturalNodeIterator();
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        virtual void toBack(void) override;
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        virtual void toFront(void) override;

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

private:
        /**
         * @brief findNextRightMostLeaf finds the next left most leaf at the bottom of the tree
         * @param start the starting point from where to search
         * @return the found leaf (this can be a container without childs or a real leaf)
         */
        EgcExpressionNode& findNextLeftMostLeaf(EgcExpressionNode& start) const;

        /**
         * @brief updateStartNode is a function that updates the start node. This must be done after changes to the tree.
         * Eigther inserting or deleting elements that move the starting node.
         */
        void updateStartNode(void);
        /**
         * @brief findFirstLeftChildParent finds the first node that is a left child of its parent and gets its
         * parent.
         * @param startNode the node where to start the search
         * @return the parent of the first node which is a left child
         */
        EgcExpressionNode& findFirstLeftChildParent(EgcExpressionNode& startNode) const;

        EgcExpressionNode* m_startNode;         ///< the starting point of the tree when doing natural traversing
        EgcExpressionNode* m_endNode;           ///< the end point of the tree when doing natural traversing

};




#endif // EGCNATURALNODEITERATOR_H
