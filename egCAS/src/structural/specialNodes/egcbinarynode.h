#ifndef EGCBINARYNODE_H
#define EGCBINARYNODE_H

#include "egccontainernode.h"

/**
 * @brief The EgcBinaryExpressionNode class is a base class for an expression that takes two arguments (binary) as subexpressions.
 * This can be e.g. a multiplication or substraction.
 */
class EgcBinaryNode : public EgcContainerNode
{
public:
        ///std contructor
        EgcBinaryNode();
        ///copy constructor
        EgcBinaryNode(const EgcBinaryNode& orig);
        ///destructor
        virtual ~EgcBinaryNode() = 0;
        /**
         * @brief setLeftChild set the left child of this binary expression (two childs can be set since this is a binary).
         * @param expression a reference to the child expression given.
         */
        void setLeftChild(const EgcNode& expression);
        /**
         * @brief setRightChild set the right child of this binary expression (two childs can be set since this is a binary).
         * @param expression a reference to the child expression given.
         */
        void setRightChild(const EgcNode& expression);
        /**
         * @brief getLeftChild get a pointer to the left child
         * @return NULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcNode* getLeftChild(void) const;
        /**
         * @brief getRightChild get a pointer to the right child
         * @returnNULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcNode* getRightChild(void) const;
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBinaryNode& operator=(const EgcBinaryNode &rhs);
        /**
         * @brief valid checks if the subnodes are valid. This can be the case if e.g. the childs are not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @returntrue if it is a binary expression, false otherwise
         */
        virtual bool isBinaryExpression(void);
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
         * unary or binary types). The node "to" takes ownership of the childs of the node "from".
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
        virtual EgcNode* getChild(quint32 index) const override;
        /**
         * @brief setChild set the given expression as a child at position index. If
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param expression the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, const EgcNode& expression) override;
        /**
         * @brief getNumberChildNodes returns the number of child nodes a container can have. Even the childs that are
         * null are counted in this context. Only the possibility to hold a number of childs counts. E.g. a binary
         * expression will always return 2 even if one or both of the child pointers are null.
         * @return the number of childs a container can hold.
         */
        virtual quint32 getNumberChildNodes(void) const override;
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

protected:
        EgcNode* m_rightChild;
        EgcNode* m_leftChild;
};

#endif // EGCBINARYNODE_H
