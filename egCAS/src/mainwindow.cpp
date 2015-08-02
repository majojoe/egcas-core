/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/egcformulaitem.h"
#include "view/egctextitem.h"
#include "view/egcpixmapitem.h"
#include "view/resizehandle.h"
#include "document/egcdocument.h"
#include "entities/egcformulaentity.h"
#include "entities/egctextentity.h"
#include "entities/egcpixmapentity.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui{new Ui::MainWindow}, m_document{new EgcDocument}
{
    m_ui->setupUi(this);
    m_ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_ui->graphicsView->setScene(m_document->getScene());
    //m_ui->graphicsView->scale(0.5,0.5);

    //add some formulas
    EgcFormulaItem::setBaseFontSize(30);
    EgcFormulaEntity* formula1 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula, 
                                                                               QPointF(120.0, 350.0)));
    formula1->setFontSize(40);
    EgcFormulaEntity* formula2 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula, 
                                                                               QPointF(120.0, 200.0)));
    formula2->setFontSize(40);
    
    
    
    EgcFormulaItem *formulaItem = m_document->getScene()->addFormula(QString("<math  display=\"block\"><mrow><mfrac><msqrt><mn>1</mn><mo>+</mo><mroot><mrow><mn>2</mn><mo>+</mo><mroot><mrow><mn>3</mn><mo>+</mo><mroot><mrow><mn>4</mn><mo>+</mo><mroot><mrow><mn>5</mn><mo>+</mo><mroot><mrow><mn>6</mn><mo>+</mo><mroot><mrow><mn>7</mn><mo>+</mo><mroot><mi>A</mi><mn>19</mn></mroot></mrow><mn>17</mn></mroot></mrow><mn>13</mn></mroot></mrow><mn>11</mn></mroot></mrow><mn>7</mn></mroot></mrow><mn>5</mn></mroot></mrow><mn>3</mn></mroot></msqrt><msup><mi>&exponentiale;</mi><mi>&pi;</mi></msup></mfrac><mo>=</mo><msup><mi>x</mi><mo style=\"font-size: larger;\">&tprime;</mo></msup></mrow></math>"), 40, QPointF(120.0, 350.0));
    EgcFormulaItem *formulaItem2 = m_document->getScene()->addFormula(QString("<math><mfrac><mrow><mn> 1 </mn><mo> + </mo><msqrt><mn> 5 </mn></msqrt></mrow><mn> 2 </mn></mfrac></math>"), 40, QPointF(100.0, 200.0));

    //add a text item
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPixelSize(50);
    QGraphicsTextItem *text = m_document->getScene()->addText(QString("This is a test text"), font_text);
    text->setPos(60.0, 30.0);

    //add a pixmap item
    QPixmap pix(":img/plane.png");
    QGraphicsPixmapItem *pixItem = m_document->getScene()->addPixmap(pix);
    pixItem->setPos(600.0, 30.0);

    QRectF rect(0,0,2100,2900);
    m_document->getScene()->setSceneRect(rect);

}

MainWindow::~MainWindow()
{
}
