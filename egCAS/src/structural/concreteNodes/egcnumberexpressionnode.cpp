#include "egcnumberexpressionnode.h"


EgcNumberExpressionNode::EgcNumberExpressionNode() : m_value(0.0)
{
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
