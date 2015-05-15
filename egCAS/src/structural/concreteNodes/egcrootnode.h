#ifndef EGCROOTNODE_H
#define EGCROOTNODE_H

#include "../specialNodes/egcbinarynode.h"

/**
 * @brief The EgcRootNode class is a class for root operations. This manages e.g. something like sqrt(10).
 */
class EgcRootNode : public EgcBinaryNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcRootNode, EgcNodeType::RootNode);
public:
        EgcRootNode();
};

#endif // EGCROOTNODE_H
