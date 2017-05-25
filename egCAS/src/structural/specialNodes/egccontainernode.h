/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#ifndef EGCCONTAINERNODE_H
#define EGCCONTAINERNODE_H

#include <new>
#include <QtGlobal>
#include "egcnode.h"

/**
 * @brief The EgcContainerNode class is a base class for a container expression that (binary or unary expression).
 */
class EgcContainerNode : public EgcNode
{
public:
        ///std contructor
        EgcContainerNode();
        ///copy constructor
        EgcContainerNode(const EgcContainerNode& orig);
        ///destructor
        virtual ~EgcContainerNode() = 0;
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void) const;
        /**
         * @brief takeOwnership takes ownership of the child given. The user is responsible for deleting the child.
         * If the user doesn't handle the child properly a leak will occur.
         * @param child the child to take ownership over.
         * @return a pointer to the child the ownership taken.
         */
        virtual EgcNode* takeOwnership(EgcNode &child) = 0;
        /**
         * @brief getChild get a pointer to the child at index. If index is > getNumberChildNodes() - 1, the returned
         * pointer is NULL.
         * @param index the index where the child is. E.g. a binary node has a left child 0 and a right child with index
         * 1 (the index starts at 0)
         * @return a pointer to the child at the index position
         */
        virtual EgcNode* getChild(quint32 index) const = 0;
        /**
         * @brief setChild set the given expression as a child at position index. Takes ownership of the node given,
         * even if setting the child failed (the given node will be deleted in this case).
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param child the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, EgcNode& child) = 0;
        /**
         * @brief getNumberChildNodes returns the number of child nodes a container can have. Even the childs that are
         * null are counted in this context. Only the possibility to hold a number of childs counts. E.g. a binary
         * expression will always return 2 even if one or both of the child pointers are null.
         * @return the number of childs a container can hold.
         */
        virtual quint32 getNumberChildNodes(void) const = 0;
        /**
         * @brief isFirstChild checks if the given node is the first (not the second or third) child of the current node.
         * @param child the child to check wether it is the first child the current parent holds.
         * @return true if given node is the first child, false otherwise.
         */
        virtual bool isFirstChild(EgcNode &child) const = 0;
        /**
         * @brief isLastChild checks if the given node is the last (not the first or second) child of the current node.
         * @param child the child to check wether it is the last child the current parent holds.
         * @return  true if the given node is the last child, false otherwise.
         */
        virtual bool isLastChild(EgcNode &child) const = 0;
        /**
         * @brief incrementToNextChild increment to the next non NULL child. Nevertheless the Returned pointer can be
         * a nullptr if there is no child at all.
         * @param previousChild a reference to the previous child, to have a starting point to find the next child.
         * @return the next non NULL child. Nevertheless the Returned pointer can be a nullptr if there is no child at
         * all.
         */
        virtual EgcNode* incrementToNextChild(EgcNode &previousChild) const = 0;
        /**
         * @brief incrementToNextChild increment to the next non NULL child. Nevertheless the Returned pointer can be
         * a nullptr if there is no child at all.
         * @param previousChild a reference to the previous child (next child in ascending order), to have a starting
         * point to find the next child.
         * @return the next (previous in descending order) non NULL child. Nevertheless the Returned pointer can be a
         * nullptr if there is no child at all.
         */
        virtual EgcNode* decrementToPrevChild(EgcNode &previousChild) const = 0;
        /**
         * @brief getIndexChild returns the index of the child given within this node as parent
         * @param child the child to calculate the index position for.
         * @param index reverence where to write the index result to.
         * @return true if child is a child of this current node (and index could be calculated), false otherwise.
         */
        virtual bool getIndexOfChild(EgcNode& child, quint32& index) const = 0;
        /**
         * @brief transferProperties transfers all properties (childs and parent) from the node "from" to this node.
         * After this operation has succeeded the node "from" has no childs or parent anymore. The operation is only
         * possible if this node has no childs at all and the number of childs of "from" and this node are equal or this
         * node is a variable node like a FlexNode. This node takes ownership of the childs of the node "from".
         * @param from the node to transfer the childs from.
         * @return true if the operation succeeded, false if the number of childs are different or this node is not
         * empty.
         */
        bool transferProperties(EgcContainerNode &from);
        /**
         * @brief isOperation checks if the node is a operation. There are also nodes that are containers, but no operations
         * @return true if the node is an operation, false otherwise
         */
        virtual bool isOperation(void) const override;
        /**
         * @brief hasSubNode checks if node is a subnode (any child, grandchild, ...) of this container node
         * @param node the node to check if it is a subnode of this container node
         * @param index the index of this container where the given subchild is in
         * @return true if given node is a subchild of this container, false if not. If the given node is the current
         * containter node, the result will also be false.
         */
        bool hasSubNode(const EgcNode& node, quint32 &index) const;
        /**
         * @brief determineIfChildIsAtomicallyBound normally a parent knows better if his child is atomically bound to the
         * parent. By implementing this the child can ask the parent whether it may be deleted (insert can take place)
         * or not. So the method isAtomicallyBoundChild can be easily implemented.
         * ONLY INTENDED TO BE USED BY DIRECT CHILDS!
         * @param node the node to check for.
         * @return true if child is atomically bound to its parent, false if not
         */
        virtual bool determineIfChildIsAtomicallyBound(const EgcNode* node) const;

protected:
        /**
         * @brief adjustChildPointers adjust the child pointers of the current object to point to the new child given.
         * ATTENTION: use this with care since the operation doesn't take care about the old childs. The caller must
         * assure that the old child will be properly deleted -> leak otherwise. It is very unlikely that you will need
         * this function outside the container classes.
         * @param old_child the old child that shall be adjusted to the new one
         * @param new_child child pointers of the current object will be adjusted to this child object.
         */
        virtual void adjustChildPointers(EgcNode &old_child, EgcNode &new_child) = 0;
};

#endif // EGCCONTAINERNODE_H

