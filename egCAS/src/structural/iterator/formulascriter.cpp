#include "formulascriter.h"
#include "../visitor/formulascrvisitor.h"
#include "../visitor/formulascrelement.h"
#include <structural/entities/egcformulaentity.h>


#define NEXT_POS() (m_pos >= 0u && m_pos < static_cast<quint32>(m_vector->size())?static_cast<int>(m_pos):0)
#define PREV_POS() (m_pos > 0u && m_pos <= static_cast<quint32>(m_vector->size())?static_cast<int>(m_pos - 1):0)

FormulaScrIter::FormulaScrIter(EgcFormulaEntity& formula, FormulaScrVector& vector) :
                                                            m_vector{&vector},
                                                            m_formula{&formula},
                                                            m_pos{0},
                                                            m_tmpPos{0}

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
        if (m_pos >= static_cast<quint32>(m_vector->size()))
                return false;

        return true;
}

bool FormulaScrIter::hasPrevious() const
{
        if (m_pos == 0)
                return false;

        return true;
}

bool FormulaScrIter::findNext(const QString& value)
{
        bool retval = false;

        while(hasNext()) {
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

        while(hasPrevious()) {
                if (previous().m_value == value) {
                        retval = true;
                        break;
                }
        }

        return retval;
}

FormulaScrElement&FormulaScrIter::next()
{
        int nextPos = NEXT_POS();

        if (m_pos < std::numeric_limits<quint32>::max() && hasNext()) {
                m_pos++;
        }

        return (*m_vector)[nextPos];
}

FormulaScrElement&FormulaScrIter::previous()
{
        int prevPos = PREV_POS();

        if (m_pos != 0 && hasPrevious()) {
                m_pos--;
        }

        return (*m_vector)[prevPos];
}

FormulaScrElement&FormulaScrIter::peekNext() const
{
        return (*m_vector)[NEXT_POS()];
}

FormulaScrElement&FormulaScrIter::peekPrevious() const
{
        return (*m_vector)[PREV_POS()];
}

void FormulaScrIter::toBack()
{
        m_pos = static_cast<quint32>(m_vector->size());
}

void FormulaScrIter::toFront()
{
        m_pos = 0;
}

void FormulaScrIter::insert(FormulaScrElement element)
{
        if (m_pos > static_cast<quint32>(m_vector->size()))
                m_pos = static_cast<quint32>(m_vector->size());
        m_vector->insert(static_cast<int>(m_pos), element);
        m_pos++;
}

void FormulaScrIter::remove(bool previous)
{
        if (previous && !hasPrevious())
                return;
        if (!previous && !hasNext())
                return;

        if (previous) {
                m_vector->remove(static_cast<int>(m_pos) - 1);
                m_pos--;
        } else {
                m_vector->remove(static_cast<int>(m_pos));
        }
}

void FormulaScrIter::update()
{
        quint32 pos = m_pos;
        FormulaScrVisitor visitor = FormulaScrVisitor(*m_formula, *this);
        visitor.updateVector();
        setIterPos(pos);
}

void FormulaScrIter::clear()
{
        m_vector->clear();
        m_pos = 0;
}

void FormulaScrIter::revert()
{
        *m_vector = m_tmpVector;
        m_pos = m_tmpPos;
}

void FormulaScrIter::save()
{
        m_tmpVector = *m_vector;
        m_tmpPos = m_pos;
}

bool FormulaScrIter::contains(EgcNode& node)
{
        QVectorIterator<FormulaScrElement> iter(*m_vector);
        iter.toFront();
        while(iter.hasNext()) {
                if (iter.next().m_node == &node) {
                        return true;
                }
        }

        return false;
}

FormulaScrVector FormulaScrIter::split(bool left) const
{
        FormulaScrVector tmp = *m_vector;
        if (left)
                tmp.remove(static_cast<int>(m_pos), tmp.size() - static_cast<int>(m_pos));
        else
                tmp.remove(0, static_cast<int>(m_pos));
        tmp.squeeze();

        return tmp;
}

void FormulaScrIter::setIterPos(quint32 pos)
{
        if (pos <= static_cast<quint32>(m_vector->size()))
                m_pos = pos;
        else
                m_pos = static_cast<quint32>(m_vector->size());
}

quint32 FormulaScrIter::getIterPos()
{
        return m_pos;
}
