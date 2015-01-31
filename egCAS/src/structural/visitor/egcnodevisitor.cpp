#include "../egcformulaexpression.h"
#include "egcnodevisitor.h"
#include "../egcnodes.h"

EgcNodeVisitor::EgcNodeVisitor(EgcFormulaExpression& formula) : m_formula(&formula)
{
}

QString EgcNodeVisitor::getResult(void)
{
        EgcExpressionNodeIterator iter(*m_formula);
        EgcNodeIteratorState state;
        EgcExpressionNode *node;
        QString result;

        while(iter.hasNext()) {
                node = &iter.next(state);
                m_state = state;
                node->accept(this);
                result += m_result;
                m_result = QString::null;
        };

        return result;
}
