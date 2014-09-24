#include "egcexpressionnodeiterator.h"
#include "../egcformulaexpression.h"
#include "../egcexpressionnode.h"


EgcExpressionNodeIterator::EgcExpressionNodeIterator(EgcFormulaExpression& formula)
{
        m_cursor = &(formula.getRootElement());
        m_rootElement = m_cursor;
}

EgcExpressionNodeIterator::~EgcExpressionNodeIterator()
{
}

bool EgcExpressionNodeIterator::findNext(EgcExpressionNodeType type)
{
}

bool EgcExpressionNodeIterator::findPrevious(EgcExpressionNodeType type)
{

}

bool EgcExpressionNodeIterator::hasNext(void) const
{

}

bool EgcExpressionNodeIterator::hasPrevious(void) const
{

}

EgcExpressionNode& EgcExpressionNodeIterator::next(void)
{

}

EgcExpressionNode& EgcExpressionNodeIterator::peekNext(void) const
{

}

EgcExpressionNode& EgcExpressionNodeIterator::peekPrevious(void) const
{

}

EgcExpressionNode& EgcExpressionNodeIterator::previous(void)
{

}

void EgcExpressionNodeIterator::toBack(void)
{

}

void EgcExpressionNodeIterator::toFront(void)
{

}

EgcExpressionNodeIterator& EgcExpressionNodeIterator::operator=(const EgcFormulaExpression & tree)
{

}
