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
         */
        void calculate(EgcEntityList& list);
private slots:
        //some slots for connecting the results of the cas kernel
        void resultReceived(QString result);
        void errorReceived(QString errorMsg);
        void kernelStarted(void);
        void kernelTerminated(void);
        void kernelErrorOccurred(QProcess::ProcessError error);   
private:
        QScopedPointer<EgcKernelConn> m_conn;     ///< the connection to the cas kernel
};

#endif // EGCCALCULATION_H
