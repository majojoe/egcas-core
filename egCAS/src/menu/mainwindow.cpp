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

#include <QMessageBox>
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
#include "menu/egclicenseinfo.h"

#warning remove this after formula input via user interface is available
#include "formulagenerator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui{new Ui::MainWindow}, m_document{new EgcDocument}
{
    m_ui->setupUi(this);
    connect(m_ui->mnu_show_license, SIGNAL(triggered()), this, SLOT(showLicense()));
    connect(m_ui->mnu_calculate, SIGNAL(triggered()), this, SLOT(calculate()));
    m_ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_ui->graphicsView->setScene(m_document->getScene());
    //m_ui->graphicsView->scale(0.5,0.5);

    //add some formulas
    EgcFormulaItem::setBaseFontSize(20);
    EgcFormulaEntity* formula1 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula, 
                                                                               QPointF(100.0, 300.0)));
    FormulaGenerator::getFormulaTree(formula1, "(sqrt(1+_root(2 + _root(3+ _root(4+ _root(5+_root(6+_root(7+_root(A,19),17), 13),11), 7),5),3)))/(_2exponentiale_3^_2pi_3)=x^_2tprime_3");
    EgcFormulaEntity* formula2 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula, 
                                                                               QPointF(100.0, 200.0)));
    FormulaGenerator::getFormulaTree(formula2, "((1+sqrt(5)))/2=_empty");
    formula2->setNumberResultType(EgcNumberResultType::ScientificType);
    formula2->setNumberOfSignificantDigits(4);
    formula2->setFontSize(25);
    
    EgcFormulaEntity* formula3 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(280.0, 150.0)));
    FormulaGenerator::getFormulaTree(formula3, "(1+38)=_empty");

    EgcFormulaEntity* formula4 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(100.0, 460.0)));
    FormulaGenerator::getFormulaTree(formula4, "z:(x-1)/(x+5)");

    EgcFormulaEntity* formula5 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(100.0, 520.0)));
    FormulaGenerator::getFormulaTree(formula5, "_integrate(z,x)=_empty");
    
    EgcFormulaEntity* formula6 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(500.0, 520.0)));
    FormulaGenerator::getFormulaTree(formula6, "_integrate(z,x,0,10)=_empty");

    EgcFormulaEntity* formula7 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(100.0, 610.0)));
    FormulaGenerator::getFormulaTree(formula7, "_diff(z,x,1)=_empty");

    EgcFormulaEntity* formula8 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(500.0, 610.0)));
    FormulaGenerator::getFormulaTree(formula8, "_diff(z,x,3)=_empty");

    EgcFormulaEntity* formula9 = static_cast<EgcFormulaEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Formula,
                                                                               QPointF(100.0, 680.0)));
    FormulaGenerator::getFormulaTree(formula9, "_diff(z,x,5)=_empty");

    //add a text item
    EgcTextEntity* text = static_cast<EgcTextEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Text,
                                                                               QPointF(60.0, 30.0)));
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPixelSize(50);
    text->setFont(font_text);
    text->setText("This is a test text");

    //add a pixmap item
    EgcPixmapEntity* pixmap = static_cast<EgcPixmapEntity*>(m_document->getEntityList()
                                                                ->createEntity(EgcEntityType::Picture,
                                                                               QPointF(600.0, 30.0)));
    pixmap->setFilePath(":res/plane.png");

    QRectF rect(0,0,2100,2900);
    m_document->getScene()->setSceneRect(rect);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLicense(void)
{
        EgcLicenseInfo licenseInfo;
        licenseInfo.exec();
}

void MainWindow::calculate(void)
{
        m_document->calculate();
}
