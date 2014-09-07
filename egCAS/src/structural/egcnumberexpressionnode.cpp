#include "egcnumberexpressionnode.h"


EgcNumberExpressionNode::EgcNumberExpressionNode() : m_value(0.0)
{
        m_nodeType = EgcExpressionNodeType::NumberNode;
}

EgcNumberExpressionNode::~EgcNumberExpressionNode()
{

}

void EgcNumberExpressionNode::setValue(qreal value)
{
        m_value = value;
}

qreal EgcNumberExpressionNode::getValue(void)
{
        return m_value;
}
