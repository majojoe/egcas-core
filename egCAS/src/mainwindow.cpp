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
    QString dummy("<math  display=\"block\"><mrow><mfrac><msqrt><mn>1</mn><mo>+</mo><mroot><mrow><mn>2</mn><mo>+</mo><mroot><mrow><mn>3</mn><mo>+</mo><mroot><mrow><mn>4</mn><mo>+</mo><mroot><mrow><mn>5</mn><mo>+</mo><mroot><mrow><mn>6</mn><mo>+</mo><mroot><mrow><mn>7</mn><mo>+</mo><mroot><mi>A</mi><mn>19</mn></mroot></mrow><mn>17</mn></mroot></mrow><mn>13</mn></mroot></mrow><mn>11</mn></mroot></mrow><mn>7</mn></mroot></mrow><mn>5</mn></mroot></mrow><mn>3</mn></mroot></msqrt><msup><mi>&exponentiale;</mi><mi>&pi;</mi></msup></mfrac><mo>=</mo><msup><mi>x</mi><mo style=\"font-size: larger;\">&tprime;</mo></msup></mrow></math>");
    QString dummy2("<math><mfrac><mrow><mn> 1 </mn><mo> + </mo><msqrt><mn> 5 </mn></msqrt></mrow><mn> 2 </mn></mfrac></math>");
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setScene(scene);
    //ui->graphicsView->scale(0.5,0.5);
    FormulaItem::setBaseFontSize(30);
    FormulaItem *formula = new FormulaItem(dummy, QPointF(120.0, 350.0));
    formula->setFontSize(40);
    scene->addItem(formula);
    scene->addItem(new FormulaItem(dummy2, QPointF(120.0, 500.0)));
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPixelSize(50);

    EgcTextItem *simpleText = new EgcTextItem(QString("a second text test"));
    simpleText->setFont(font_text);
    simpleText->setPos(60.0, 500.0);
    scene->addItem(simpleText);
    scene->addText(QString("This is a test text"), font_text);

    QPixmap pix(":img/plane.png");
    //pix->setDevicePixelRatio(0.5);
    pix = pix.scaled(1500, 1500, Qt::KeepAspectRatio);
    EgcPixmapItem *pixmap = new EgcPixmapItem(pix);
    pixmap->setPos(900.0, 200.0);
    scene->addItem(pixmap);


    ResizeHandle *resizeHandle = new ResizeHandle(QSizeF(80.0, 80.0));
    scene->addItem(resizeHandle);

    QRectF rect(0,0,2100,2900);
    scene->setSceneRect(rect);
}

MainWindow::~MainWindow()
{
    delete ui;
}
