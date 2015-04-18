#include "egcexpressionnodecreator.h"
#include "egcnodes.h"

EgcExpressionNode* EgcExpressionNodeCreator::create(EgcExpressionNodeType type)
{
        EgcExpressionNode *retval;

        switch (type) {
        case EgcExpressionNodeType::NumberNode:
                retval = new (std::nothrow) EgcNumberExpressionNode();
                break;
        case EgcExpressionNodeType::VariableNode:
                retval = new (std::nothrow) EgcVariableExpressionNode();
                break;
        case EgcExpressionNodeType::EmptyNode:
                retval = new (std::nothrow) EgcEmptyExpressionNode();
                break;
        case EgcExpressionNodeType::RootNode:
                retval = new (std::nothrow) EgcRootExpressionNode();
                break;
        case EgcExpressionNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisExpressionNode();
                break;
        case EgcExpressionNodeType::BaseNode:
                retval = new (std::nothrow) EgcBaseExpressionNode();
                break;
        default: //EgcExpressionNodeType::NodeUndefined
                qDebug("The implementation of this function is not complete, Please provide an implementation for the newly added EgcExpressionNodeType.");
                retval = nullptr;
                break;
        }

        return retval;
}
