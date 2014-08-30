#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/formulaitem.h"
#include "view/egctextitem.h"
#include "view/egcpixmapitem.h"
#include "view/resizehandle.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new EgCasScene(this);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setScene(scene);
    //ui->graphicsView->scale(0.5,0.5);

    //add some formulas
    FormulaItem::setBaseFontSize(30);
    FormulaItem *formulaItem = scene->addFormula(QString("<math  display=\"block\"><mrow><mfrac><msqrt><mn>1</mn><mo>+</mo><mroot><mrow><mn>2</mn><mo>+</mo><mroot><mrow><mn>3</mn><mo>+</mo><mroot><mrow><mn>4</mn><mo>+</mo><mroot><mrow><mn>5</mn><mo>+</mo><mroot><mrow><mn>6</mn><mo>+</mo><mroot><mrow><mn>7</mn><mo>+</mo><mroot><mi>A</mi><mn>19</mn></mroot></mrow><mn>17</mn></mroot></mrow><mn>13</mn></mroot></mrow><mn>11</mn></mroot></mrow><mn>7</mn></mroot></mrow><mn>5</mn></mroot></mrow><mn>3</mn></mroot></msqrt><msup><mi>&exponentiale;</mi><mi>&pi;</mi></msup></mfrac><mo>=</mo><msup><mi>x</mi><mo style=\"font-size: larger;\">&tprime;</mo></msup></mrow></math>"), 40);
    formulaItem->setPos(QPointF(120.0, 350.0));
    FormulaItem *formulaItem2 = scene->addFormula(QString("<math><mfrac><mrow><mn> 1 </mn><mo> + </mo><msqrt><mn> 5 </mn></msqrt></mrow><mn> 2 </mn></mfrac></math>"), 40);
    formulaItem2->setPos(QPointF(120.0, 200.0));

    //add a text item
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPixelSize(50);
    QGraphicsTextItem *text = scene->addText(QString("This is a test text"), font_text);
    text->setPos(60.0, 30.0);

    //add a pixmap item
    QPixmap pix(":img/plane.png");
    QGraphicsPixmapItem *pixItem = scene->addPixmap(pix);
    pixItem->setPos(600.0, 30.0);

    QRectF rect(0,0,2100,2900);
    scene->setSceneRect(rect);
}

MainWindow::~MainWindow()
{
    delete ui;
}
