#include "matrixbox.h"
#include "ui_matrixbox.h"
#include "../types/matrixdimension.h"

MatrixBox::MatrixBox(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::MatrixBox)
{
        ui->setupUi(this);
}

MatrixBox::~MatrixBox()
{
        delete ui;
}

MatrixDimension MatrixBox::getDimension()
{
        this->exec();
        quint32 cols = static_cast<quint32>(this->ui->spinBox_columns->value());
        quint32 rows = static_cast<quint32>(this->ui->spinBox_rows->value());
        return MatrixDimension(cols, rows);
}
