#ifndef MATRIXBOX_H
#define MATRIXBOX_H

#include <QDialog>

namespace Ui {
class MatrixBox;
}

class MatrixDimension;

class MatrixBox : public QDialog
{
        Q_OBJECT

public:
        explicit MatrixBox(QWidget *parent = nullptr);
        ~MatrixBox();
        MatrixDimension getDimension(void);

private:
        Ui::MatrixBox *ui;
};

#endif // MATRIXBOX_H
