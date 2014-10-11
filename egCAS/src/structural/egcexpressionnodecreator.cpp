#include "egcexpressionnodecreator.h"
#include "egcnodes.h"

EgcExpressionNode* EgcExpressionNodeCreator::copy(EgcExpressionNode& node)
{
        EgcExpressionNode *retval = nullptr;
        EgcExpressionNodeType type = node.getNodeType();

        switch (type) {
        case EgcExpressionNodeType::NumberNode:
                retval = new (std::nothrow) EgcNumberExpressionNode(static_cast<EgcNumberExpressionNode&>(node));
                break;
        case EgcExpressionNodeType::VariableNode:
                retval = new (std::nothrow) EgcVariableExpressionNode(static_cast<EgcVariableExpressionNode&>(node));
                break;
        case EgcExpressionNodeType::RootNode:
                retval = new (std::nothrow) EgcRootExpressionNode(static_cast<EgcRootExpressionNode&>(node));
                break;
        case EgcExpressionNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisExpressionNode(static_cast<EgcParenthesisExpressionNode&>(node));
                break;
        default:  //EgcExpressionNodeType::NodeUndefined
                qDebug("The implementation of this function is not complete, Please provide an implementation for the newly added EgcExpressionNodeType.");
                retval = nullptr;
                break;
        }

        return retval;
}

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
        case EgcExpressionNodeType::RootNode:
                retval = new (std::nothrow) EgcRootExpressionNode();
                break;
        case EgcExpressionNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisExpressionNode();
                break;
        default: //EgcExpressionNodeType::NodeUndefined
                qDebug("The implementation of this function is not complete, Please provide an implementation for the newly added EgcExpressionNodeType.");
                retval = nullptr;
                break;
        }

        return retval;
}
