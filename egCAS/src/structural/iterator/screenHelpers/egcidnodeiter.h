/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef EGCIDNODEITER_H
#define EGCIDNODEITER_H

#include <QtGlobal>
#include <QScopedPointer>
#include <QVector>
#include <QRectF>

class EgcNode;
class EgcFormulaEntity;
class EgcMathmlLookup;
class EgcNodeIterator;
enum class EgcIteratorState;
enum class EgcNodeType;


/**
 * @brief The EgcIdNodeIter class is a iterator for navigating between the formula glyphs
 */
class EgcIdNodeIter
{
public:        
        /// constructor for initialization with formula. After the initialization the iterator points to the end of the
        /// formula.
        EgcIdNodeIter(const EgcFormulaEntity& formula);
        /// std destructor
        virtual ~EgcIdNodeIter();
        /**
         * @brief setAtNode set the iterator to a position on the right or left side of the given Node.
         * ATTENTION: the given node must be an element of the formula.
         * @param node the node where to position the iterator (at the left or at the right side of the node)
         * @param atRightSide true if the iterator shall be positioned at the right side (default), false otherwise
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         */
        void setAtNode(EgcNode& node, bool atRightSide = true, bool snapAtOmittedPositions = false);
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
         * @return a reference to the next mathml id.
         */
        EgcNode &  next(void);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @return a refererence to the previous mathml id.
         */
        EgcNode &  previous(void);
        /**
         * @brief toBack Moves the iterator to the back of the tree (after the last item).
         */
        void toBack(void);
        /**
         * @brief toFront Moves the iterator to the front of the tree (before the first item).
         */
        void toFront(void);
        /**
         * @brief getNode returns the current node
         * @return the current node that is currently marked
         */
        EgcNode& getNode(void);
        /**
         * @brief lastId returns the last id we jumped over.
         * @return the id we jumped over lastly.
         */
        quint32 id(void) const;
        /**
         * @brief getState retruns the state of the last traversion
         * @return state the state of the node we jumped over
         */
        EgcIteratorState getState(void) const;
        /**
         * @brief getOriginNodeToMark finds from the given node the next node that can be marked and used as origin node
         * to mark. This can also be the given node.
         * @param node the node where to start the search
         * @return the node that can serve as a origin node where to start marking.
         */
        EgcNode& getOriginNodeToMark(const EgcNode& node) const;
        /**
         * @brief rightSide checks if current cursor is at the right side of the node or not
         * @return true if cursor is at the right side, false otherwise
         */
        bool rightSide(void) const;
        /**
         * @brief insert insert a node at the current position of the next or previous pointer
         * @param type the type of the node to insert
         * @return true if everything went well, false otherwise
         */
        bool insert(EgcNodeType type);
        /**
         * @brief finishModOperation finish any started operation that modifies the tree. Some operations (like inserts
         * and deletes) can only be finished if mathml lookup table had been updated. So correction of cursor positions
         * can only be done then.
         */
        void finishModOperation(void);

private:
        /**
         * @brief getMathmlId returns the mathml id of a node with a specified state and a predecessor or successor
         * @param node the node to check for existence of a mathml id
         * @param state the state of the node given
         * @param previousNode the node that is the predecessor of the given node in the formula tree
         * @param nextNode the node that is the successor of the given node in the formula tree
         * @return the mathml id for the given element, or 0 if no id was found
         */
        quint32 getMathmlId(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const;
        /**
         * @brief mathmlIdExisting checks if a mathml id for the current node is existing.
         * @param node the node to check for existence of a mathml id
         * @param state the state of the node given
         * @param otherNode the node that is at the other side of the iterator as the node given
         * @param nextNode if true the otherNode is the next node, if false it is the previous node
         * @return true if a id is existing, false otherwise
         */
        bool mathmlIdExisting(EgcNode* node, EgcIteratorState state, EgcNode* otherNode, bool nextNode) const;
        /**
         * @brief prevNodeWithId iterates the given iterator to the previous node that has a valid mathml id and sets the
         * given iterator accordingly
         * @param currNode the node that is currently active
         * @param tempIter the iterator given to operate with
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* prevNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, bool snapAtOmittedPositions = false) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next node that has a valid mathml id and sets the
         * given iterator accordingly
         * @param currNode the node that is currently active
         * @param tempIter the iterator given to operate with (must point at the currNode given)
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* nextNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter, bool snapAtOmittedPositions = false) const;
        /**
         * @brief omitNode check if a node must be omitted
         * @param node the node that follows the current node, eigther in forward or backward direction
         * @param stateToTest the state of the node to check
         * @param atRightSide if the cursor is at the right side of an operator
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         * @return true if the current node shall be omitted, false otherwise
         */
        bool omitFollowingNode(EgcNode* node, EgcIteratorState stateToTest, bool atRightSide, bool snapAtOmittedPositions = false) const;
        /**
         * @brief nodeStateVisible checks if the following node is visible when iterating over it
         * @param iter the current iterator in the tree
         * @param nodeToTest node to test for visibility
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         * @return true if the node state in direction is visible, false otherwise
         */
        bool nodeStateVisible(const EgcNodeIterator& iter, EgcNode& nodeToTest, bool snapAtOmittedPositions = false) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next or previous node that has a valid mathml id
         * and sets the given iterator accordingly
         * @param forward if true we test in forward direction, if false in backward
         * @param tempIter the iterator given to operate with
         * @param node the node where to start the testing
         * @param checkFollowing iterate one over to the following node and start testing there.
         * @param snapAtOmittedPositions if true iterator will stop at positions normally omitted while stepping
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, const EgcNode& node, bool checkFollowing = false, bool snapAtOmittedPositions = false) const;
        /**
         * @brief rightSide checks if current cursor is at the right side of the node or not
         * @param iter the iterator that shall be used to check on which side we are
         * @param node the current node to check on which side of the operator iterating over we are (must be the prev
         * or next node of the given iterator)
         * @return true if cursor is at the right side, false otherwise
         */
        bool rightSide(EgcNodeIterator& iter, EgcNode& node) const;

        QScopedPointer<EgcNodeIterator> m_nodeIter;         ///< the node iterator that points to the current node with a mathml id
        const EgcMathmlLookup& m_lookup;                    ///< a reference to the lookup data
        EgcNode* m_node;                                    ///< currently active node (where the cursor is currently)
        EgcNode* m_iterPosAfterUpdate;                      ///< iterator should be at this position after mathml lookup table update
        bool m_atRightSideAfterUpdate;                      ///< if cursor should be at the right side of m_iterPosAfterUpdate after update
};

#endif // EGCIDNODEITER_H
