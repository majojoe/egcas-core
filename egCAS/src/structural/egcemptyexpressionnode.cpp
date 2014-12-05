#include <QChar>
#include "egcemptyexpressionnode.h"


EgcEmptyExpressionNode::EgcEmptyExpressionNode()
{
        m_value = QChar(0x2B1A);
        m_subscript = QString::null;
}

EgcEmptyExpressionNode::~EgcEmptyExpressionNode()
{

}

QString& EgcEmptyExpressionNode::getValue(void)
{
        return m_value;
}

bool EgcEmptyExpressionNode::valid(void)
{
        return false;
}
