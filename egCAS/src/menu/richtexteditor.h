/*
Copyright (c) 2014 - 2018, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
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
#ifndef RICHTEXTEDITOR_H
#define RICHTEXTEDITOR_H

#include <QWidget>
#include <QFont>

class QDialog;
class MRichTextEdit;
class QGridLayout;
class QPushButton;

class RichTextEditor : public QWidget
{
        Q_OBJECT
public:
        RichTextEditor(QWidget* parent = nullptr);
        virtual ~RichTextEditor();
        /**
         * @brief exec executes the rich text editor and returns the html text entered
         * @param initialText the initial text to set when starting editing
         * @return the html text entered
         */
        QString exec(QString initialText);
        /**
         * @brief setGenericFont set the font used by this class if nothing else is set for the object
         * @param font the font to set
         */
        static void setGenericFont(const QFont& font);

public slots:
        void ok_clicked(void);
        void cancel_clicked(void);
private:
        QDialog *m_dialog;
        MRichTextEdit *m_rte;
        QGridLayout *m_gl;
        QPushButton *m_ok_btn;
        QPushButton *m_cancel_btn;
        QString m_initialText;
        static QFont s_genericFont;                     ///< the generic font to use for all text entities
};

#endif // RICHTEXTEDITOR_H
