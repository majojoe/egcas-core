#ifndef EGCUNARYEXPRESSIONNODE_H
#define EGCUNARYEXPRESSIONNODE_H

#include "egccontainernode.h"

/**
 * @brief The EgcUnaryExpressionNode
 * pressionNode class is a base class for an expression that takes one argument (unary) as subexpression.
 * This can be e.g. the not operator.
 */
class EgcUnaryExpressionNode : public EgcContainerNode
{
public:
        ///std constructor
        EgcUnaryExpressionNode();
        ///copy constructor
        EgcUnaryExpressionNode(const EgcUnaryExpressionNode& orig);
        /// destructor
        virtual ~EgcUnaryExpressionNode() = 0;
        /**
         * @brief setChild set the child of this unary expression (only one child can be set).
         * This function takes ownership of the child and will delete the child automatically in its destructor.
         * @param expression a reference to the child expression given.
         */
        void setChild(const EgcExpressionNode&expression);
        /**
         * @brief getChild get a pointer to the child
         * @return NULL if no this expression has no child, a pointer to the child otherwise.
         */
        EgcExpressionNode* getChild(void) const;
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcUnaryExpressionNode& operator=(const EgcUnaryExpressionNode &rhs);
        /**
         * @brief valid checks if the subnode is valid. This can be the case if e.g. the child is not NULL.
         * @return returns true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isUnaryExpression returns if the current element is a unary expression (container) or not
         * @return true if it is a unary expression, false otherwise
         */
        virtual bool isUnaryExpression(void);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcExpressionNode* child);
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
        virtual bool transferPropertiesTo(EgcExpressionNode &to);

        /**
         * @brief adjustChildPointers adjust the child pointers of the current object to point to the new child given.
         * ATTENTION: use this with care since the operation doesn't take care about the old childs. The caller must
         * assure that the old child will be properly deleted -> leak otherwise. It is very unlikely that you will need
         * this function outside the container classes.
         * @param old_child the old child that shall be adjusted to the new one
         * @param new_child child pointers of the current object will be adjusted to this child object.
         */
        virtual void adjustChildPointers(EgcExpressionNode &old_child, EgcExpressionNode &new_child) override;
        /**
         * @brief takeOwnership takes ownership of the child given. The user is responsible for deleting the child.
         * If the user doesn't handle the child properly a leak will occur.
         * @param child the child to take ownership over.
         * @return a pointer to the child the ownership taken.
         */
        virtual EgcExpressionNode* takeOwnership(EgcExpressionNode &child) override;
        /**
         * @brief accept takes a visitor and calls the given visitor back to be able to extract data from this node
         * @param visitor the visitor to call back
         */
        virtual void accept(EgcNodeVisitor *visitor) override;


        EgcExpressionNode* m_child;
};

#endif // EGCUNARYEXPRESSIONNODE_H
