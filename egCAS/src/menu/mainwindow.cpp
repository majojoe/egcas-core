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
#include "menu/egclicenseinfo.h"
#include "menu/elementbar.h"
#include "menu/precisionbox.h"
#include "menu/resulttype.h"
#include "menu/mathfont.h"
#include "menu/textfont.h"
#include <QMessageBox>
#include <QSpacerItem>
#include <QComboBox>
#include <QFileDialog>
#include <QDir>

#warning remove this after formula input via user interface is available
#include "formulagenerator.h"


#define MAP_COMBO_TO_PRECISION(prec)  ((prec == 0) ? 0 : (prec + 1))
#define MAP_PRECISION_TO_COMBO(prec)  ((prec == 0) ? 0 : (prec - 1))

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui{new Ui::MainWindow}, m_document{new EgcDocument}, m_precision{nullptr}
{
    m_ui->setupUi(this);
    m_ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_ui->graphicsView->setScene(m_document->getScene());

    //setup toolbar and other stuff
    setupConnections();
    setupToolbar();
    setupElementBar();

    //add some formulas
    EgcFormulaEntity* formula1 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 480.0)));
    FormulaGenerator::getFormulaTree(formula1, "_{sqrt(1+_root(3,2 + _root(5,3+ _root(7,4+ _root(11,5+_root(13,6+_root(17,7+_root(19,A))))))))_}/_{ⅇ^π_}=x^‴");
    EgcFormulaEntity* formula2 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 350.0)));
    FormulaGenerator::getFormulaTree(formula2, "_{-1+sqrt(5)_}/_{2_}=_empty");
    formula2->setNumberResultType(EgcNumberResultType::ScientificType);
    formula2->setNumberOfSignificantDigits(4);
    
    EgcFormulaEntity* formula3 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(330.0, 300.0)));
    FormulaGenerator::getFormulaTree(formula3, "(1+38)=_empty");
    formula3->setNumberResultType(EgcNumberResultType::EngineeringType);

    EgcFormulaEntity* formula4 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 610.0)));
    FormulaGenerator::getFormulaTree(formula4, "z_120:_{x-1_}/_{x+5_}");

    EgcFormulaEntity* formula5 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 670.0)));
    FormulaGenerator::getFormulaTree(formula5, "_integrate(z_120,x)=_empty");
    
    EgcFormulaEntity* formula6 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(650.0, 670.0)));
    FormulaGenerator::getFormulaTree(formula6, "_integrate(0,10,z_120,x)=_empty");

    EgcFormulaEntity* formula7 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 760.0)));
    FormulaGenerator::getFormulaTree(formula7, "_diff(1,z_120,x,1)=_empty");

    EgcFormulaEntity* formula8 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(650.0, 760.0)));
    FormulaGenerator::getFormulaTree(formula8, "_diff(3,z_120,x,3)=_empty");

    EgcFormulaEntity* formula9 = static_cast<EgcFormulaEntity*>(m_document->createEntity(EgcEntityType::Formula,
                                                                                         QPointF(250.0, 830.0)));
    FormulaGenerator::getFormulaTree(formula9, "_diff(0,z_120,x,5)=_empty");

    //add a text item
    EgcTextEntity* text = static_cast<EgcTextEntity*>(m_document->createEntity(EgcEntityType::Text,
                                                                               QPointF(60.0, 30.0)));
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPointSize(50);
    text->setFont(font_text);
    text->setText("This is a test text");

    //add a pixmap item
    EgcPixmapEntity* pixmap = static_cast<EgcPixmapEntity*>(m_document->createEntity(EgcEntityType::Picture,
                                                                                     QPointF(750.0, 180.0)));
    pixmap->setFilePath(":res/plane.png");

    QRectF rect(0,0,2100, 2900);    
    m_document->getScene()->setSceneRect(rect);
    //show the window maximized
    showMaximized();

    calculate();    
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLicense(void)
{
        EgcLicenseInfo licenseInfo;
        licenseInfo.exec();
}

void MainWindow::showInfo(void)
{
        QMessageBox msgBox;

        msgBox.setText(QString(tr("Version: ")) + QString(EGCAS_VERSION) + QString(tr(" (pre-alpha - non-production)\n"))
                       + QString(tr("Build Date: ")) + QString("%1 %2").arg(__DATE__).arg(__TIME__));
        msgBox.exec();
}

void MainWindow::calculate(void)
{
        m_document->startCalulation();
}

void MainWindow::autoCalculation(bool on)
{
        m_document->setAutoCalculation(on);
        if (on)
                calculate();
}

void MainWindow::newPage(void)
{

}

void MainWindow::setupConnections(void)
{
        connect(m_ui->mnu_show_license, SIGNAL(triggered()), this, SLOT(showLicense()));
        connect(m_ui->mnu_show_info, SIGNAL(triggered()), this, SLOT(showInfo()));
        connect(m_ui->mnu_autoCalc, SIGNAL(triggered(bool)), this, SLOT(autoCalculation(bool)));
        connect(m_ui->mnu_CalculateDocument, SIGNAL(triggered()), this, SLOT(calculate()));
        connect(m_ui->mnu_new_page, SIGNAL(triggered()), this, SLOT(newPage()));
        connect(m_ui->mnu_insert_graphic, SIGNAL(triggered()), this, SLOT(insertGraphic()));
        connect(m_ui->mnu_insert_text, SIGNAL(triggered()), this, SLOT(insertText()));
}

void MainWindow::setupToolbar()
{
        //setup main toolbar
        m_ui->mainToolBar->layout()->setContentsMargins(0,0,5,0);
        //m_ui->mainToolBar->addAction(m_ui->mnu_new_page);
        m_ui->mainToolBar->addAction(m_ui->mnu_insert_graphic);
        m_ui->mainToolBar->addAction(m_ui->mnu_insert_text);
        //setup math toolbar
        m_ui->mathToolBar->layout()->setContentsMargins(0,0,5,0);
        m_ui->mathToolBar->addAction(m_ui->mnu_autoCalc);
        m_ui->mathToolBar->addSeparator();
        m_precision = new PrecisionBox(m_document.data(), m_ui->mathToolBar, this);
        m_resulttype = new ResultType(m_document.data(), m_ui->mathToolBar, this);
        m_mathFont = new MathFont(m_document.data(), m_ui->mathToolBar, this);
        m_textFont = new TextFont(m_document.data(), m_ui->mainToolBar, this);
}


void MainWindow::setupElementBar(void)
{
        ElementBar::setupBar(this, m_ui->elmentBarLayout, dynamic_cast<EgCasScene*>(m_ui->graphicsView->scene()));

        // add a spacer at the end of the bar
        QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        m_ui->elmentBarLayout->addItem(spacer);
}

void MainWindow::insertGraphic(void)
{
        static QString directory = QDir::homePath();
        QPointF lastPos = m_document->getLastCursorPosition();
        QString fileName = QFileDialog::getOpenFileName(this, tr("insert graphic"), directory,
                                                        tr("Images (*.gif *.bmp *.jpg *.jpeg *.png)"));

        if (!fileName.isNull()) {
                QFileInfo fileInfo(fileName);
                directory = fileInfo.absolutePath();
                EgcPixmapEntity* pixmap = static_cast<EgcPixmapEntity*>(m_document->createEntity(EgcEntityType::Picture,
                                                                                                 lastPos));
                pixmap->setFilePath(fileName);
        }
}

void MainWindow::insertText(void)
{
        QPointF lastPos = m_document->getLastCursorPosition();

        EgcTextEntity* txt = static_cast<EgcTextEntity*>(m_document->createEntity(EgcEntityType::Text,
                                                                                                 lastPos));
        txt->setEditMode();

}
