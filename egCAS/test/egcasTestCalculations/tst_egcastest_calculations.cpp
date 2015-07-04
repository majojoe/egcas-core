/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <QString>
#include <QtTest>
#include <iostream>
#include "egckernelparser.h"
#include "egcnodes.h"
#include "egcnodeiterator.h"
#include "egcformulaexpression.h"
#include "egcnodecreator.h"
#include "egcnodevisitor.h"
#include "egcmaximavisitor.h"
#include "egcmathmlvisitor.h"
#include "casKernel/egcmaximaconn.h"
#include "egcformulaexpression.h"


class EgcasTest_Calculation : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Calculation() {}

private Q_SLOTS:
        void basicTestCalculation();
private:
};


void EgcasTest_Calculation::basicTestCalculation()
{
        EgcFormulaExpression formula1;
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree1;
        tree1.reset(parser.parseKernelOutput("x:33.1"));
        if (tree1.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree1.isNull());

        // this shall be possible
        formula1 = tree1.take();

        QScopedPointer<EgcNode> tree2;
        tree2.reset(parser.parseKernelOutput("x^3+36-8*651.984"));
        if (tree2.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree2.isNull());

        EgcMaximaConn *conn = new (std::nothrow) EgcMaximaConn("/usr/bin/maxima", this);

        conn->sendCommand(formula1.getCASKernelCommand());
}



QTEST_MAIN(EgcasTest_Calculation)

#include "tst_egcastest_calculations.moc"
