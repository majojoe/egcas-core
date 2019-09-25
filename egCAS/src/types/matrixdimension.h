#ifndef MATRIXDIMENSION_H
#define MATRIXDIMENSION_H

#include <QMetaType>

class MatrixDimension {
public:
        MatrixDimension() : m_column_count{0}, m_row_count{0} {}
        MatrixDimension(quint32 column_count, quint32 row_count) : m_column_count{column_count}, m_row_count{row_count} {}
        quint32 rowCount(void) {return m_row_count;}
        quint32 columnCount(void) {return m_column_count;}
private:
        quint32 m_column_count;
        quint32 m_row_count;
};

Q_DECLARE_METATYPE(MatrixDimension)

#endif // MATRIXDIMENSION_H
