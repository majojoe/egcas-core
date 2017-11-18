#include "formulascriter.h"
#include "../visitor/formulascrvisitor.h"
#include "../visitor/formulascrelement.h"
#include <structural/entities/egcformulaentity.h>

FormulaScrIter::FormulaScrIter(EgcFormulaEntity& formula, FormulaScrVector& vector) : m_formula{formula},
                                                            m_vector{vector},
                                                            m_iter{QMutableVectorIterator<FormulaScrElement>(m_vector)},
                                                            m_pos{0}
{
        FormulaScrVisitor visitor = FormulaScrVisitor(formula, *this);
        (void) visitor.getResult();
        toBack();
}

FormulaScrIter::~FormulaScrIter()
{

}

bool FormulaScrIter::hasNext() const
{
        m_iter.hasNext();
}

bool FormulaScrIter::hasPrevious() const
{
        m_iter.hasPrevious();
}

bool FormulaScrIter::findNext(const QString& value)
{
        bool retval = false;

        while(m_iter.hasNext()) {
                if (next().m_value == value) {
                        retval = true;
                        break;
                }
        }

        return retval;
}

bool FormulaScrIter::findPrevious(const QString& value)
{
        bool retval = false;

        while(m_iter.hasPrevious()) {
                if (previous().m_value == value) {
                        retval = true;
                        break;
                }
        }

        return retval;
}

FormulaScrElement&FormulaScrIter::next()
{
        if (m_pos < std::numeric_limits<quint32>::max() && m_iter.hasNext()) {
                m_pos++;
                m_iter.next();
        }
}

FormulaScrElement&FormulaScrIter::previous()
{
        if (m_pos != 0 && m_iter.hasPrevious()) {
                m_pos--;
                m_iter.previous();
        }
}

FormulaScrElement&FormulaScrIter::peekNext() const
{
        m_iter.peekNext();
}

FormulaScrElement&FormulaScrIter::peekPrevious() const
{
        m_iter.peekPrevious();
}

void FormulaScrIter::toBack()
{
        m_iter.toBack();
        m_pos = m_vector.size();
}

void FormulaScrIter::toFront()
{
        m_iter.toFront();
        m_pos = 0;
}

void FormulaScrIter::insert(FormulaScrElement element)
{
        m_iter.insert(element);
        m_pos = getIterPos();
}

void FormulaScrIter::remove()
{
        m_iter.remove();
        m_pos = getIterPos();
}

void FormulaScrIter::update()
{
        FormulaScrVisitor visitor = FormulaScrVisitor(m_formula, *this);
        (void) visitor.getResult();
        m_iter = QMutableVectorIterator<FormulaScrElement>(m_vector);
        setIterPos(m_pos);
}

void FormulaScrIter::clear()
{
        m_vector.clear();
        m_iter = QMutableVectorIterator<FormulaScrElement>(m_vector);
        setIterPos(m_pos);
}

void FormulaScrIter::setIterPos(quint32 pos)
{
        if (pos <= m_vector.size())
                m_pos = pos;
        m_iter.toFront();
        int i;
        for(i = 0; i < pos; i++) {
                if (m_iter.hasNext()) {
                        (void) m_iter.next();
                } else {
                        break;
                }
        }
}

quint32 FormulaScrIter::getIterPos()
{
        int i = 0;
        QMutableVectorIterator<FormulaScrElement> iter = m_iter;
        while(iter.hasPrevious()) {
                i++;
                (void) iter.previous();
        }

        return i;
}
