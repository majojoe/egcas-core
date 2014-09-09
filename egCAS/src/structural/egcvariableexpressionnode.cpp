#include <QString>
#include <QStringBuilder>
#include "egcvariableexpressionnode.h"

EgcVariableExpressionNode::EgcVariableExpressionNode() : m_value(QString::null), m_subscript(QString::null)
{
}

EgcVariableExpressionNode::~EgcVariableExpressionNode()
{

}

void EgcVariableExpressionNode::setValue(QString& varName, QString& subscript)
{
        m_value = varName;
        m_subscript = subscript;
}

QString& EgcVariableExpressionNode::getValue(void)
{
        return m_value;
}

QString& EgcVariableExpressionNode::getSubscript(void)
{
        return m_subscript;
}

QString EgcVariableExpressionNode::getStuffedVar(void)
{
        //replace (stuffing) of "_" with "__" since "_" is used to concatenate variable name and subscript
        QString var = m_value.replace("_", "__");
        QString sub = m_subscript.replace("_", "__");

        return var % "_" % sub;
}
