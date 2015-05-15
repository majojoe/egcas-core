#include "egcnumbernode.h"


EgcNumberNode::EgcNumberNode() : m_value("0.0")
{
}

EgcNumberNode::~EgcNumberNode()
{

}

void EgcNumberNode::setValue(const QString& value)
{
        m_value = value;
}

QString& EgcNumberNode::getValue(void)
{
        return m_value;
}
