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
#ifndef COLLAPSABLESECTIONWIDGET_H
#define COLLAPSABLESECTIONWIDGET_H

#include <QObject>
#include <QWidget>

class QVBoxLayout;
class QPushButton;
class QGridLayout;
class QBoxLayout;

class CollapsableSectionWidget : public QWidget
{
        Q_OBJECT
public:

        /**
         * @brief The CollapsableSectionLayout enum is for choosing the layout of the collapseable section
         */
        enum CollapsableSectionLayout {grid, vertical, horizontal};

        CollapsableSectionWidget(CollapsableSectionLayout layout, QWidget *parent = nullptr);
        virtual ~CollapsableSectionWidget();
        /**
         * @brief setText set text for group button
         * @param text the text in the group button to display
         */
        virtual void setText(const QString & text);
        /**
         * @brief addWidget add a widget to the grid layout of the widget (only if a grid layout has been chosen)
         *
         * If rowSpan and/or columnSpan is -1, then the layout will extend to the bottom and/or right edge, respectively.
         *
         * @param widget the widget to add
         * @param fromRow row where to start widget layout
         * @param fromColumn column where to start widget layout
         * @param rowSpan over how many rows the widget will span
         * @param columnSpan over how many columns the widget will span
         */
        virtual void addWidget(QWidget *widget, int fromRow, int fromColumn, int rowSpan, int columnSpan);
        /**
         * @brief addWidget a widget if no grid layout has been chosen
         * @param widget the widget to add
         * @param stretch if the widget shall be stretched with the given stretch factor
         */
        virtual void addWidget(QWidget *widget, int stretch = 0);

private slots:
        /**
         * @brief setVisibility internal slot to be connected to the group button of this widget
         * @param visible if true the widget will be visible, if false not
         */
        void setVisibility(bool visible);
private:
        QWidget *sectionWidget;                     ///< widget that contains the grid layout
        QVBoxLayout *vLayout;                       ///< layout of this widget
        QPushButton *btn_collapse;                  ///< group button of this widget (shows/hides the widgets added)
        QGridLayout *gridLayout;                    ///< layout of the collapsable section
        QBoxLayout *boxLayout;                      ///< layout of the collapsable section
};

#endif // COLLAPSABLESECTIONWIDGET_H
