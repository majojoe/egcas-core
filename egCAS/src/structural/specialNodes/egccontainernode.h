#ifndef EGCCONTAINERNODE_H
#define EGCCONTAINERNODE_H

#include "egcexpressionnode.h"

/**
 * @brief The EgcContainerNode class is a base class for a container expression that (binary or unary expression).
 */
class EgcContainerNode : public EgcExpressionNode
{
public:
        ///std contructor
        EgcContainerNode();
        ///destructor
        virtual ~EgcContainerNode() = 0;
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void) const;
        /**
         * @brief transferChilds transfers all properties (childs and parent) from the node "from" to the node "to".
         * After this operation has succeeded the node "from" has no childs or parent anymore. The operation is only
         * possible if the node "to" has no childs at all and the nodes "from" and "to" are of the same type (all are
         * unary or binary types). The node "to" takes ownership of the childs of the node "from".
         * @param to the node to transfer the childs to.
         * @return true if the operation succeeded, false if the nodes are of different types or the node "to" is not
         * empty.
         */
        virtual bool transferPropertiesTo(EgcExpressionNode &to) = 0;
        /**
         * @brief adjustChildPointers adjust the child pointers of the current object to point to the new child given.
         * ATTENTION: use this with care since the operation doesn't take care about the old childs. The caller must
         * assure that the old child will be properly deleted -> leak otherwise. It is very unlikely that you will need
         * this function outside the container classes.
         * @param old_child the old child that shall be adjusted to the new one
         * @param new_child child pointers of the current object will be adjusted to this child object.
         */
        virtual void adjustChildPointers(EgcExpressionNode &old_child, EgcExpressionNode &new_child) = 0;
        /**
         * @brief takeOwnership takes ownership of the child given. The user is responsible for deleting the child.
         * If the user doesn't handle the child properly a leak will occur.
         * @param child the child to take ownership over.
         * @return a pointer to the child the ownership taken.
         */
        virtual EgcExpressionNode* takeOwnership(EgcExpressionNode &child) = 0;
};

#endif // EGCCONTAINERNODE_H

