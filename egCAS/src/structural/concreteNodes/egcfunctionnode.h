#ifndef EGCFUNCTIONNODE_H
#define EGCFUNCTIONNODE_H

#include "../specialNodes/egcflexnode.h"

/**
 * @brief The EgcFuntionNode class is a class to model function calls. This manages e.g. something like calculation1(10, x + 3, 9.8234).
 */
class EgcFunctionNode : public EgcFlexNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcFunctionNode, EgcNodeType::FunctionNode);
public:
        EgcFunctionNode();
};

#endif // EGCFUNCTIONNODE_H
