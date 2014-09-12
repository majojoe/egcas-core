#include "egcexpressionnode.h"

EgcExpressionNode::EgcExpressionNode()
{
}

EgcExpressionNode::~EgcExpressionNode()
{

}

bool EgcExpressionNode::valid(void)
{
        return true;
}

bool EgcExpressionNode::isContainer(void)
{
        return false;
}

bool EgcExpressionNode::isUnaryExpression(void)
{
        return false;
}

bool EgcExpressionNode::isBinaryExpression(void)
{
        return false;
}
