#include "egcnumberexpressionnode.h"


EgcNumberExpressionNode::EgcNumberExpressionNode() : m_value("0.0")
{
}

EgcNumberExpressionNode::~EgcNumberExpressionNode()
{

}

void EgcNumberExpressionNode::setValue(const QString& value)
{
        m_value = value;
}

QString& EgcNumberExpressionNode::getValue(void)
{
        return m_value;
}
