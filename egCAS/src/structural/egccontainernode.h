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
};

#endif // EGCCONTAINERNODE_H

