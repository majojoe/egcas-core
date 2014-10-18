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
};

#endif // EGCCONTAINERNODE_H

