#ifndef EGCFLEXNODE_H
#define EGCFLEXNODE_H

#include "egccontainernode.h"

/**
 * @brief The EgcFlexNode Node class is a base class for an expression that can have an arbitrary number of child nodes.
 */
class EgcFlexNode : public EgcContainerNode
{
public:
        ///std constructor
        EgcFlexNode();
        ///copy constructor
        EgcFlexNode(const EgcFlexNode& orig);
        /// destructor
        virtual ~EgcFlexNode() = 0;
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcFlexNode& operator=(const EgcFlexNode &rhs);
        /**
         * @brief valid checks if the subnode is valid. This can be the case if e.g. the child is not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcNode* child);
        /**
         * @brief isLeaf checks if the current node is a leaf (there are no childs)
         * @return true if it is a leaf, false otherwise
         */
        virtual bool isLeaf(void) const;
        /**
         * @brief transferChilds transfers all properties (childs and parent) from the node "from" to the node "to".
         * After this operation has succeeded the node "from" has no childs or parent anymore. The operation is only
         * possible if the node "to" has no childs at all and the nodes "from" and "to" are of the same type (all are
         * flex or binary types). The node "to" takes ownership of the childs of the node "from".
         * @param to the node to transfer the childs to.
         * @return true if the operation succeeded, false if the nodes are of different types or the node "to" is not
         * empty.
         */
        virtual bool transferPropertiesTo(EgcNode &to);
        /**
         * @brief adjustChildPointers adjust the child pointers of the current object to point to the new child given.
         * ATTENTION: use this with care since the operation doesn't take care about the old childs. The caller must
         * assure that the old child will be properly deleted -> leak otherwise. It is very unlikely that you will need
         * this function outside the container classes.
         * @param old_child the old child that shall be adjusted to the new one
         * @param new_child child pointers of the current object will be adjusted to this child object.
         */
        virtual void adjustChildPointers(EgcNode &old_child, EgcNode &new_child) override;
        /**
         * @brief takeOwnership takes ownership of the child given. The user is responsible for deleting the child.
         * If the user doesn't handle the child properly a leak will occur.
         * @param child the child to take ownership over.
         * @return a pointer to the child the ownership taken.
         */
        virtual EgcNode* takeOwnership(EgcNode &child) override;
        /**
         * @brief accept takes a visitor and calls the given visitor back to be able to extract data from this node
         * @param visitor the visitor to call back
         */
        virtual void accept(EgcNodeVisitor *visitor) override;
        /**
         * @brief getChild get a pointer to the child at index. If index is > getNumberChildNodes() - 1, the returned
         * pointer is NULL.
         * @param index the index where the child is. E.g. a binary node has a left child 0 and a right child with index
         * 1 (the index starts at 0)
         * @return a pointer to the child at the index position
         */
        virtual EgcNode* getChild(quint32 index) const;
        /**
         * @brief setChild set the given expression as a child at position index. If
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param expression the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, const EgcNode& expression);
        /**
         * @brief getNumberChildNodes returns the number of child nodes a container can have. Even the childs that are
         * null are counted in this context. Only the possibility to hold a number of childs counts. E.g. a binary
         * expression will always return 2 even if one or both of the child pointers are null.
         * @return the number of childs a container can hold.
         */
        virtual quint32 getNumberChildNodes(void) const;
        /**
         * @brief isFirstChild checks if the given node is the first (not the second or third) child of the current node.
         * @param child the child to check wether it is the first child the current parent holds.
         * @return true if given node is the first child, false otherwise.
         */
        virtual bool isFirstChild(EgcNode &child) const override;
        /**
         * @brief isLastChild checks if the given node is the last (not the first or second) child of the current node.
         * @param child the child to check wether it is the last child the current parent holds.
         * @return  true if the given node is the last child, false otherwise.
         */
        virtual bool isLastChild(EgcNode &child) const override;
        /**
         * @brief incrementToNextChild increment to the next non NULL child. Nevertheless the Returned pointer can be
         * a nullptr if there is no child at all.
         * @param previousChild a reference to the previous child, to have a starting point to find the next child.
         * @return the next non NULL child. Nevertheless the Returned pointer can be a nullptr if there is no child at
         * all.
         */
        virtual EgcNode* incrementToNextChild(EgcNode &previousChild) const override;
        /**
         * @brief incrementToNextChild increment to the next non NULL child. Nevertheless the Returned pointer can be
         * a nullptr if there is no child at all.
         * @param previousChild a reference to the previous child (next child in ascending order), to have a starting
         * point to find the next child.
         * @return the next (previous in descending order) non NULL child. Nevertheless the Returned pointer can be a
         * nullptr if there is no child at all.
         */
        virtual EgcNode* decrementToPrevChild(EgcNode &previousChild) const override;
        /**
         * @brief getIndexChild returns the index of the child given within this node as parent
         * @param child the child to calculate the index position for.
         * @param index reverence where to write the index result to.
         * @return true if child is a child of this current node (and index could be calculated), false otherwise.
         */
        virtual bool getIndexChild(EgcNode& child, quint32& index) const override;

protected:
        EgcNode* m_child;
};

#endif // EGCFLEXNODE_H
