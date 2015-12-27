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

#include <QString>
#include <QtTest>
#include <iostream>
#include <QScopedPointer>
#include "parser/egckernelparser.h"
#include "egcnodes.h"
#include "iterator/egcnodeiterator.h"
#include "entities/egcformulaentity.h"
#include "egcnodecreator.h"
#include "visitor/egcnodevisitor.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"
#include "egcmaximaconn.h"
#include "iterator/screenHelpers/egcidnodeiter.h"

class EgcasTest_ScrIters : public QObject
{
        Q_OBJECT

public:
        EgcasTest_ScrIters() {}
private Q_SLOTS:
        void testIdNodeIter();
private:
        EgcNode* getTree(QString formula);
        EgcFormulaEntity formula;
        EgcKernelParser parser;
};

EgcNode* EgcasTest_ScrIters::getTree(QString formula)
{
        QScopedPointer<EgcNode> tree(parser.parseKernelOutput(formula));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }

        return tree.take();
}

void EgcasTest_ScrIters::testIdNodeIter(void)
{
        //test equal functions
        EgcFormulaEntity formula;
        formula.setRootElement(getTree("((1+sqrt(5)))/2=1.618"));
        //generate id list
        (void) formula.getMathMlCode();
        EgcMathmlLookup& lookup = formula.getMathmlMappingRef();

        QPair<EgcNode*, quint32> pair;
        foreach(pair, lookup.getList()) {
                qDebug() << "Node: " << pair.first << "Id: " << pair.second;
        }

        EgcIdNodeIter iter(formula);
        iter.toFront();

        while (iter.hasNext()) {
                EgcNode* node = &iter.next();
                qDebug() << "Node: " << node << "id: " << iter.id() << "state: " << static_cast<int>(iter.getLastState());
        }

}



QTEST_MAIN(EgcasTest_ScrIters)

#include "tst_egcastest_screen_iterators.moc"
