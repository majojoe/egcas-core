#include <QString>
#include <QStringBuilder>
#include <QRegularExpression>
#include "egcvariableexpressionnode.h"

EgcVariableExpressionNode::EgcVariableExpressionNode() : m_value(QString::null), m_subscript(QString::null)
{
}

EgcVariableExpressionNode::~EgcVariableExpressionNode()
{

}

void EgcVariableExpressionNode::setValue(const QString& varName, const QString& subscript)
{
        m_value = varName;
        m_subscript = subscript;
}

void EgcVariableExpressionNode::setValueRaw(const QString& varName)
{
        QRegularExpression regex = QRegularExpression("(.*[^_]+)_([^_]+.*)");
        QRegularExpressionMatch regexMatch = regex.match(varName);
        if (regexMatch.hasMatch()) {
                QString tmp = regexMatch.captured(1);
                tmp.replace("__", "_");
                m_value = tmp;
                tmp = regexMatch.captured(2);
                tmp.replace("__", "_");
                m_subscript = tmp;
        } else {
                m_value = varName;
                m_value.replace("__", "_");
                m_subscript = QString::null;
        }
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

bool EgcVariableExpressionNode::valid(void)
{
        if (m_value.isEmpty())
                return false;
        else
                return true;
}
