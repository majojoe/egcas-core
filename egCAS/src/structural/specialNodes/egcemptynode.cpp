#include <QChar>
#include "egcemptynode.h"


EgcEmptyNode::EgcEmptyNode()
{
        m_value = QChar(0x2B1A);
        m_subscript = QString::null;
}

EgcEmptyNode::~EgcEmptyNode()
{

}

QString& EgcEmptyNode::getValue(void)
{
        return m_value;
}

bool EgcEmptyNode::valid(void)
{
        return false;
}
