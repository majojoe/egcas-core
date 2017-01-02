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
 * @brief The EgcSnapProperty enum snapping property of e.g. several cursor moving functions. These properties can be
 * combined via OR operator ( EgcSnapProperty::SnapOnlyVisible | EgcSnapProperty::SnapAtLeafContainer )
 */
enum class EgcSnapProperty
{
        SnapAll = 0x7FFFFFFF,           ///< snap at all leafes, containers and structures that are anywhere in the hierarchy
        SnapVisibleCursor = 0x1,        ///< snap at visible cursor positions only e.g. not at any positons
        SnapVisibleSigns = 0x2,         ///< snap at nodes (and states) that have visible signs
        SnapModifyable = 0x4,           ///< snap at nodes (and states) that has modifyable elements
};

/**
 * @brief The EgcIdNodeIter class is a iterator for navigating between the formula glyphs
 */
class EgcIdNodeIter
{
public:        
        /// constructor for initialization with formula. After the initialization the iterator points to the end of the
        /// formula.
        EgcIdNodeIter(EgcFormulaEntity& formula);
        /// std destructor
        virtual ~EgcIdNodeIter();
        /**
         * @brief EgcIdNodeIter copy constructor
         * @param orig reference to original
         */
        EgcIdNodeIter(const EgcIdNodeIter& orig);

        /**
         * @brief setAtNode set the iterator to a position on the right or left side of the given Node.
         * ATTENTION: the given node must be an element of the formula.
         * @param node the node where to position the iterator (at the left or at the right side of the node)
         * @param atRightSide true if the iterator shall be positioned at the right side (default), false otherwise
         * @param snapProperty the snap property to use
         */
        void setAtNode(EgcNode& node, bool atRightSide = true, EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor);
        /**
         * @brief setAtNode set the iterator to a position on the right or left side of the given Node. Same as above,
         * but at a later time when the mathml lookup has been updated.
         * ATTENTION: the given node must be an element of the formula.
         * @param node the node where to position the iterator (at the left or at the right side of the node)
         * @param atRightSide true if the iterator shall be positioned at the right side (default), false otherwise
         */
        void setAtNodeDelayed(EgcNode& node, bool atRightSide = true);
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
         * @return true if the screen iterator needs to step forward, false if not
         */
        bool finishModOperation(void);
        /**
         * @brief remove the node in the direction given
         * @param before if true remove the item in backward direction, if false the item in forward direction
         * @return if true the operation was successful, if false not
         */
        bool remove(bool before);
        /**
         * @brief deleteTree delete the node tree before or after the current cursor position
         * @param before if true, the tree before the current cusrsor position will be deleted, otherwise the tree after
         * the current position.
         * @return true if deleting worked, false otherwise
         */
        bool deleteTree(bool before);
        /**
         * @brief replaceByEmtpy replaces current node by empty node
         * @param cursorRight true if the cursor shall be at the right side of the empty element, false if left
         * @return true if everything went well, false otherwise
         */
        bool replaceByEmtpy(bool cursorRight);
        /**
         * @brief lockDelayedCursorUpdate if the cursor update is locked, setAtNodeDelayed has no effect. This can be
         * useful, if there are some operations in between that shall have no effect. It is very unlikely that a user
         * will need this function.
         */
        void lockDelayedCursorUpdate(void);
        /**
         * @brief unlockDelayedCursorUpdate after unlocking the cursor update will work as expected before. It is very
         * unlikely that a user will need to use this.
         */
        void unlockDelayedCursorUpdate(void);
private:
        /**
         * @brief hasNext Checks if there is at most one more item after the current item.
         * @param snapProperty the snap property to use
         * @return True if an item was found, false otherwise.
         */
        bool hasNext(EgcSnapProperty snapProperty) const;
        /**
         * @brief hasPrevious Checks if there is at most one more item before the current item.
         * @param snapProperty the snap property to use
         * @return True if an item was found, false otherwise.
         */
        bool hasPrevious(EgcSnapProperty snapProperty) const;
        /**
         * @brief next Returns the next node and increments the iterator by one.
         * @param snapProperty the snap property to use
         * @return a reference to the next mathml id.
         */
        EgcNode &  next(EgcSnapProperty snapProperty);
        /**
         * @brief previous Returns the previous node and decrements the iterator by one.
         * @param snapProperty the snap property to use
         * @return a refererence to the previous mathml id.
         */
        EgcNode &  previous(EgcSnapProperty snapProperty);
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
         * @param snapProperty the snap property to use
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* prevNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter,
                                EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next node that has a valid mathml id and sets the
         * given iterator accordingly
         * @param currNode the node that is currently active
         * @param tempIter the iterator given to operate with (must point at the currNode given)
         * @param snapProperty the snap property to use
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* nextNodeWithId(EgcNode& currNode, EgcNodeIterator* tempIter,
                                EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor) const;
        /**
         * @brief omitNode check if a node must be omitted
         * @param node the node that follows the current node, eigther in forward or backward direction
         * @param stateToTest the state of the node to check
         * @param atRightSide if the cursor is at the right side of an operator
         * @param snapProperty the snap property to use
         * @return true if the current node shall be omitted, false otherwise
         */
        bool omitFollowingNode(EgcNode* node, EgcIteratorState stateToTest, bool atRightSide,
                               EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor) const;
        /**
         * @brief nodeStateVisible checks if the following node is visible when iterating over it
         * @param iter the current iterator in the tree
         * @param nodeToTest node to test for visibility
         * @param snapProperty the snap property to use
         * @return true if the node state in direction is visible, false otherwise
         */
        bool nodeStateVisible(const EgcNodeIterator& iter, EgcNode& nodeToTest,
                              EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor) const;
        /**
         * @brief cursorVisible returns the visibility of a node type
         * @param node the node to test for visibility
         * @param state the state in which the node currently is
         * @return if the current state of the node is a visible cursor position then the returnvalue is true, otherwise
         * false
         */
        bool cursorVisible(EgcNode& node, EgcIteratorState state) const;
        /**
         * @brief visibleSign returns the visibility of a node type
         * @param node the node to test for visibility
         * @param state the state in which the node currently is
         * @return if the current state of the node has visible signs then the returnvalue is true, otherwise false
         */
        bool visibleSign(EgcNode& node, EgcIteratorState state) const;
        /**
         * @brief modifyableElement returns the visibility of a node type
         * @param node the node to test for visibility
         * @param state the state in which the node currently is
         * @return if the current state of the node has modifyable elements then the returnvalue is true, otherwise
         * false
         */
        bool modifyableElement(EgcNode& node, EgcIteratorState state) const;
        /**
         * @brief nextNodeWithId iterates the given iterator to the next or previous node that has a valid mathml id
         * and sets the given iterator accordingly
         * @param forward if true we test in forward direction, if false in backward
         * @param tempIter the iterator given to operate with
         * @param node the node where to start the testing
         * @param checkFollowing iterate one over to the following node and start testing there.
         * @param snapProperty the snap property to use
         * @return a pointer to the next valid active node found, nullptr if there is no valid node anymore
         */
        EgcNode* gotoNodeWithId(bool forward, EgcNodeIterator* tempIter, const EgcNode& node, bool checkFollowing = false,
                                EgcSnapProperty snapProperty = EgcSnapProperty::SnapVisibleCursor) const;
        /**
         * @brief rightSide checks if current cursor is at the right side of the node or not
         * @param iter the iterator that shall be used to check on which side we are
         * @param node the current node to check on which side of the operator iterating over we are (must be the prev
         * or next node of the given iterator)
         * @return true if cursor is at the right side, false otherwise
         */
        bool rightSide(EgcNodeIterator& iter, EgcNode& node) const;
        /**
         * @brief removeBinary remove a binary node
         * @param before true if the node before the current cursor shall be removed, false otherwise
         * @param the node to delete/modify
         * @param state the state of the node to delete/modify
         * @return true if removing the binary node succeeded, false otherwise.
         */
        bool removeBinary(bool before, EgcNode& node, EgcIteratorState state);
        /**
         * @brief removeBinary remove a flex node
         * @param before true if the node before the current cursor shall be removed, false otherwise
         * @param the node to delete/modify
         * @param state the state of the node to delete/modify
         * @return true if removing the binary node succeeded, false otherwise.
         */
        bool removeFlex(bool before, EgcNode& node, EgcIteratorState state);
        /**
         * @brief removeBinary remove a unary node
         * @param before true if the node before the current cursor shall be removed, false otherwise
         * @param the node to delete/modify
         * @param state the state of the node to delete/modify
         * @return true if removing the binary node succeeded, false otherwise.
         */
        bool removeUnary(bool before, EgcNode& node, EgcIteratorState state);
        /**
         * @brief removeBinary remove a leaf node
         * @param before true if the node before the current cursor shall be removed, false otherwise
         * @param the node to delete/modify
         * @param state the state of the node to delete/modify
         * @return true if removing the binary node succeeded, false otherwise.
         */
        bool removeLeaf(bool before, EgcNode& node, EgcIteratorState state);
        /**
         * @brief getNodeToModify return the node that will be modified if the cursor is at current position and the
         * node before or after shall be modified.
         * @param before true if the node before the current cursor shall be modified, otherwise the node after
         * @param state the state of the node returned
         * @param goOn if true, the values from the previous run will be used. So if the container from the 1st run was
         * not appropriate, a second run can be started.
         * @return the node that shall be modified
         */
        EgcNode* getNodeToModify(bool before, EgcIteratorState &state, bool goOn = false);
        /**
         * @brief findAtomicNode returns the node that is not bound atomically to its parent. Therefore insert
         * operations (or deletions) can take place now (Is atomic). Note that atomic nodes can also have non atomic
         * childs (beside atomic childs).
         * @node a reference to the node where to start the search
         * @return the next atomic node of the given node (the node given if it is atomic or one of it's parents if
         * given node is a atomic child). This can also return nullptr, if no atomic node has been found.
         */
        EgcNode* findAtomicNode(EgcNode& node) const;
        /**
         * @brief moveToAtomicNode move m_nodeIter to the next atomic node. Therefore insert operations (or deletions)
         * can take place now (Is atomic). Note that atomic nodes can also have non atomic childs (beside atomic
         * childs). Note that this object must be in a consistent state in order to use this function successful (
         * m_node and m_nodeIter must be convergent).
         * @param forward in which direction to move the cursor. if true cursor is moved forward, if false backwards
         * @return true if everything worked well, false otherwise.
         */
        bool moveToAtomicNode(bool forward);
        /**
         * @brief besideBinEmptyNode check if there is a empty binary node near the current cursor
         * @param right check if there is an empty node on the right side of m_node, if false check is done for left
         * side.
         * @return true if empty binary node found, false otherwise
         */
        bool besideBinEmptyNode(bool right);
        /**
         * @brief replaceBinEmptyNodeBy replace the empty binary node beside m_node
         * @param type the type of the node to replace the empty node
         * @param right if true, search on the right side of m_node, if false search on the left
         * @return true if everything worked well, false otherwise
         */
        bool replaceBinEmptyNodeBy(EgcNodeType type, bool right);

        QScopedPointer<EgcNodeIterator> m_nodeIter;         ///< the node iterator that points to the current node with a mathml id
        EgcNode* m_node;                                    ///< currently active node (where the cursor is currently)
        EgcNode* m_iterPosAfterUpdate;                      ///< iterator should be at this position after mathml lookup table update
        bool m_atRightSideAfterUpdate;                      ///< if cursor should be at the right side of m_iterPosAfterUpdate after update
        bool m_isInsert;                                    ///< true if after an insert operation the iterator shall be incremented (that cursor is after inserted element)
        EgcFormulaEntity &m_formula;                        ///< reference to formula associated with
        bool m_lockDelayedUpdate;                           ///< lock the delayed update via setAtNodeDelayed
};

#endif // EGCIDNODEITER_H
