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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QScopedPointer>
#include "view/egcasscene.h"

namespace Ui {
class MainWindow;
}
class EgcDocument;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
        void showLicense(void);
        void showInfo(void);
        void calculate(void);
        void autoCalculation(bool on);
        void newPage(void);
        void precBox(int prec);
private slots:
        /**
         * @brief onSelectionChange is called if the selection of a formula or any other entity changes
         */
        void onSelectionChange(void);
private:
        /**
         * @brief setPrecision set the precision of a formula or the document
         * @param prec the precision to set 0 (standard), 2-16(number of digits)
         */
        void setPrecision(int prec);
        /**
         * @brief setupConnections setup all connections to slots that are neccessary
         */
        void setupConnections(void);
        /**
         * @brief setupToolbar setup the toolbar
         */
        void setupToolbar(void);
        /**
         * @brief setupPrecisionSpinBox setup a spin Box for adjusting the precision for a calculation
         */
        void setupPrecisionComboBox(void);
        /**
         * @brief setupElementBar setup the left bar with the math buttons
         */
        void setupElementBar(void);

        QScopedPointer<Ui::MainWindow> m_ui;
        QScopedPointer<EgcDocument> m_document;
        QComboBox* m_digits;
};

#endif // MAINWINDOW_H
