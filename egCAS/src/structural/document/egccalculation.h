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

#ifndef EGCCALCULATION_H
#define EGCCALCULATION_H

#include <QObject>
#include "entities/egcentitylist.h"
#include "casKernel/egcmaximaconn.h"
#include "casKernel/parser/egckernelparser.h"

class EgcFormulaEntity;

/**
 * @brief The EgcCalculation class handles the calculation of the document.
 */
class EgcCalculation : public QObject
{
        Q_OBJECT
public:
        EgcCalculation(QObject *parent = 0);
        /**
         * @brief calculate start calculation of the given list
         * @param list the list to use for the calculations
         * @param updateInstantly if true the view will be updated instantly after calculating. If false, the update
         * will happen after resuming the calculation.
         */
        void calculate(EgcEntityList& list, bool updateInstantly = true);
//        /**
//         * @brief resumeCalculation calculate all remaining formulas in the list (given with calculate) until all
//         * formulas are calculated or there is another lock on a formula.
//         * This method is for resuming a stopped calculation that has been interrupted upon a lock on a formula.
//         */
//        void resumeCalculation(void);
private slots:
        //some slots for connecting the results of the cas kernel
        void resultReceived(QString result);
        void errorReceived(QString errorMsg);
        void kernelStarted(void);
        void kernelTerminated(void);
        void kernelErrorOccurred(QProcess::ProcessError error);   
        /**
         * @brief nextCalculation triggers the next calculation as long as all formulas are calculated or a formula has
         * a lock
         */
        void nextCalculation(void);
private:
        /**
         * @brief handleCalculation computes the calculation for the current formula
         * @param entity a reference to the formula currently computed
         */
        void handleCalculation(EgcFormulaEntity& entity);

        QScopedPointer<EgcKernelConn> m_conn;   ///< the connection to the cas kernel
        QScopedPointer<QListIterator<EgcEntity*>> m_iterator;   ///< iterator that operates on the entity list we are calculating on
        bool m_kernelStarted;   ///< indicates that cas kernel has started and can calculate formulas now
        bool m_computeWhenStarted;              ///< begin with computation when the kernel has started
        bool m_updateInstantly;                 ///< when true, update the view instantly, otherwise it's updated after resuming the calculation
        EgcFormulaEntity* m_waitForResult;      ///< a pointer to the formula entity that is currently being calculated
        EgcKernelParser m_parser;
};

#endif // EGCCALCULATION_H
