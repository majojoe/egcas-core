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
#ifndef RESULTTYPE_H
#define RESULTTYPE_H

#include <QScopedPointer>
#include <QWidget>
#include "entities/egcformulaentity.h"

class QComboBox;
class EgcDocument;
class QToolBar;


class ResultType : public QWidget
{
        Q_OBJECT
public:
        ResultType(EgcDocument* doc, QToolBar* toolbar, QWidget* parent = nullptr);
        ~ResultType();
public slots:
        void resultTypeBox(QString text);
private slots:
        /**
         * @brief onSelectionChange is called if the selection of a formula or any other entity changes
         */
        void onSelectionChange(void);
private:
        /**
         * @brief mapToType map the given text to an result type
         * @param text the text to map to the result type
         * @return the result type mapped
         */
        EgcNumberResultType mapToType(QString text);
        /**
         * @brief mapToIndex map the given type to an box index
         * @param type the result type to map to an index
         * @return the index mapped
         */
        int mapToIndex(EgcNumberResultType type);

        Q_DISABLE_COPY(ResultType)
        /**
         * @brief setResultType set the number result type of a formula
         * @param result the type to set (StandardType, IntegerType, EngineeringType or ScientificType)
         */
        void setResultType(EgcNumberResultType result);

        QComboBox* m_box;
        EgcDocument* m_document;
        EgcFormulaEntity* m_lastSelectedFormula;
};

#endif // RESULTTYPE_H
