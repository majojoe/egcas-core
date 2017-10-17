/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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
#ifndef ELEMENTBAR_H
#define ELEMENTBAR_H

#include <QObject>
#include <QVBoxLayout>
#include <QString>

class MathSection;
class EgCasScene;

/**
 * @brief The ElementBar class is to setup the element bar of the main window
 */
class ElementBar : public QObject
{
        Q_OBJECT
public:
        /**
         * @brief setupBar setup the bar layout of the main window
         * @param barLayout the layout where to insert the sections in
         * @param scene pointer to the scene where to send the actions
         */
        static void setupBar(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene);
signals:

public slots:
private:
        /**
         * @brief getNewSection prepares a new section
         * @param parent pointer to parent
         * @param barLayout pointer to the layout where the section is inserted in
         * @param title the title of the section
         * @return nullptr in case anything went wrong, or a pointer to the new section if successful
         */
        static MathSection* getNewSection(QWidget* parent, QVBoxLayout* barLayout, QString title);
        /**
         * @brief setupAlgebraSection setup algebra section
         */
        static void setupCalcSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene);
        /**
         * @brief setupGreekSection setup greek section
         */
        static void setupGreekSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene);
        /**
         * @brief setupAnalysisSection setup analysis section
         */
        static void setupAnalysisSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene);

        explicit ElementBar();
        virtual ~ElementBar();
};

#endif // ELEMENTBAR_H
