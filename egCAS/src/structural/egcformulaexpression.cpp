#include "egcformulaexpression.h"
#include "egcexpressionnode.h"
#include "egcexpressionnodecreator.h"

EgcFormulaExpression::EgcFormulaExpression(EgcExpressionNodeType type)
{
        m_data = EgcExpressionNodeCreator::create(type);
}

EgcFormulaExpression::~EgcFormulaExpression()
{
        // delete the formula with the complete tree
        delete m_data;
}

EgcExpressionNode& EgcFormulaExpression::getRootElement(void) const
{
        return *m_data;
}
