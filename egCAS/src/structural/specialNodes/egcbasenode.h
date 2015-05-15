#ifndef EGCBASENODE_H
#define EGCBASENODE_H

#include "egcunarynode.h"

/**
 * @brief The EgcBaseNode class is a class that always is the base element of an expression.
 */
class EgcBaseNode : public EgcUnaryNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcBaseNode, EgcNodeType::BaseNode);
public:
        EgcBaseNode();

};

#endif // EGCBASENODE_H
