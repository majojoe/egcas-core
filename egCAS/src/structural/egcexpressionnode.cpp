#include "egcexpressionnode.h"

EgcExpressionNode::EgcExpressionNode()
{
        m_nodeType = EgcExpressionNodeType::NumberNode;
}

EgcExpressionNode::~EgcExpressionNode()
{

}

EgcExpressionNodeType EgcExpressionNode::getNodeType(void)
{
        return m_nodeType;
}
