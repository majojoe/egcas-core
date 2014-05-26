#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    //QString dummy("<math  display=\"block\"><mrow><mfrac><msqrt><mn>1</mn><mo>+</mo><mroot><mrow><mn>2</mn><mo>+</mo><mroot><mrow><mn>3</mn><mo>+</mo><mroot><mrow><mn>4</mn><mo>+</mo><mroot><mrow><mn>5</mn><mo>+</mo><mroot><mrow><mn>6</mn><mo>+</mo><mroot><mrow><mn>7</mn><mo>+</mo><mroot><mi>A</mi><mn>19</mn></mroot></mrow><mn>17</mn></mroot></mrow><mn>13</mn></mroot></mrow><mn>11</mn></mroot></mrow><mn>7</mn></mroot></mrow><mn>5</mn></mroot></mrow><mn>3</mn></mroot></msqrt><msup><mi>&exponentiale;</mi><mi>&pi;</mi></msup></mfrac><mo>=</mo><msup><mi>x</mi><mo style=\"font-size: larger;\">&tprime;</mo></msup></mrow></math>");
    QString dummy("<math><mfrac><mrow><mn> 1 </mn><mo> + </mo><msqrt><mn> 5 </mn></msqrt></mrow><mn> 2 </mn></mfrac></math>");
    scene->addItem(new FormulaItem(dummy, QPointF(100.0, 100.0)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
