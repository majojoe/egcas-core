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
#include "parser/egckernelparser.h"
#include "egcnodes.h"
#include "iterator/egcnodeiterator.h"
#include "entities/egcformulaentity.h"
#include "egcnodecreator.h"
#include "visitor/egcnodevisitor.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"


class EgcasTest_AdvancedTreeOps : public QObject
{
        Q_OBJECT

public:
        EgcasTest_AdvancedTreeOps() {}

private Q_SLOTS:
        void copyTreeTest();
private:
};


void EgcasTest_AdvancedTreeOps::copyTreeTest()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("1+fnc(2+3,4,5,(6))"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcFormulaEntity formula(*tree.take());

        EgcNode *copiedTree;

        EgcBinaryNode* rootNode = static_cast<EgcBinaryNode*>(formula.getBaseElement().getChild(0));
        copiedTree = formula.copy(*rootNode->getChild(1));
        QVERIFY(copiedTree);
        QVERIFY(copiedTree->getNodeType() == EgcNodeType::FunctionNode);

        QVERIFY(formula.paste(*copiedTree, *rootNode->getChild(0)));
        QVERIFY(rootNode->getChild(0)->getNodeType() == EgcNodeType::FunctionNode);
}


QTEST_MAIN(EgcasTest_AdvancedTreeOps)

#include "tst_egcas_advanced_tree_ops.moc"
