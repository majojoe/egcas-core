#include "worksheet.h"

explict Worksheet::Worksheet(QObject*parent) :
        QObject(parent)
{
        m_grid = QSizeF(30.0, 30.0);
}

Worksheet::~Worksheet()
{

}

QSizeF Worksheet::grid()
{
        return m_grid;
}

void Worksheet::setGrid(QSizeF grid)
{
        m_grid = grid;
}

QMargins Worksheet::margins()
{
        return m_margins;
}

void Worksheet::setMargins(QMargins margins)
{
        m_margins = margins;
}

QSizeF Worksheet::dimensions()
{
        return m_dimensions;
}

void Worksheet::setDimensions(QSizeF dimensions)
{
        m_dimensions = dimensions;
}
