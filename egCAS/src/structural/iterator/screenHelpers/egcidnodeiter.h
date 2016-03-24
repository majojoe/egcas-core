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
         */
        void setAtNode(EgcNode& node, bool atRightSide);
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
        bool rightSide(void);

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
         * @param testForPrevNode if true starts with the test at the next node the iterator points to, if false start
         * at the previous node
         * @param tempIter the iterator given to operate with
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* prevNodeWithId(bool testForPrevNode, EgcNodeIterator* tempIter) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next node that has a valid mathml id and sets the
         * given iterator accordingly
         * @param testForNextNode if true starts with the test at the next node the iterator points to, if false start
         * at the previous node
         * @param tempIter the iterator given to operate with
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* nextNodeWithId(bool testForNextNode, EgcNodeIterator* tempIter) const;
        /**
         * @brief omitNode check if a node must be omitted
         * @param followingNode the node that follows the current node, eigther in forward or backward direction
         * @param followingState the state of the following node
         * @return true if the current node shall be omitted, false otherwise
         */
        bool omitFollowingNode(EgcNode* followingNode, EgcIteratorState followingState) const;
        /**
         * @brief nodeStateVisible checks if the following node is visible when iterating over it
         * @param iter the current iterator in the tree
         * @param forwardDirection is true if we want to iterate forward in the tree, false otherwise
         * @return true if the node state in direction is visible, false otherwise
         */
        bool nodeStateVisible(const EgcNodeIterator& iter, bool forwardDirection) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next or previous node that has a valid mathml id
         * and sets the given iterator accordingly
         * @param forward if true we test in forward direction, if false in backward
         * @param tempIter the iterator given to operate with
         * @param checkFollowing iterate one over to the following node and start testing there.
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, bool checkFollowing = false) const;

        QScopedPointer<EgcNodeIterator> m_nodeIter;         ///< the node iterator that points to the current node with a mathml id
        const EgcMathmlLookup& m_lookup;                    ///< a reference to the lookup data
        EgcNode* m_node;                                    ///< currently active node (where the cursor is currently)
};

#endif // EGCIDNODEITER_H
