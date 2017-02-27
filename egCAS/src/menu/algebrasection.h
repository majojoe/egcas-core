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
#ifndef ALGEBRASECTION_H
#define ALGEBRASECTION_H

#include <QWidget>
#include <QVector>
#include "collapsablesectionwidget.h"
#include "mathelement.h"

class QSignalMapper;

class AlgebraSection : public QWidget
{
        Q_OBJECT
public:
        explicit AlgebraSection(QWidget *parent = 0);
        /**
         * @brief setText set text for group button
         * @param text the text in the group button to display
         */
        void setText(const QString & text);
        /**
         * @brief setChecked set the group button checked
         */
        void setChecked(void);
signals:

private slots:
        /**
         * @brief clicked clicked signal remapped from signal mapper
         * @param cmd the command to execute
         */
        void clicked(QString cmd);
private:
        /**
         * @brief initCommandVector initializes the command vector
         */
        static QVector<MathElement> initCommandVector(void);
        static QVector<MathElement> s_buttonVect;       ///< button vector
        CollapsableSectionWidget* m_section;            ///< pointer to the algebra section
        QSignalMapper *m_signalMapper;                  ///< pointer to signal mapper
        quint32 m_nrCoulumns;                           ///< the number of columns in the button section
};

#endif // ALGEBRASECTION_H
