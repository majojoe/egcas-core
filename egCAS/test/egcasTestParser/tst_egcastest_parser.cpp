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


class EgcasTest_Parser : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Parser() {}

private Q_SLOTS:
        void basicTestParser();
        void falseTestParser();
        void treeTestParser();
private:
};


void EgcasTest_Parser::basicTestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("(45+a)-n__j5_lm__3+kl__9-js_z"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }

        QVERIFY(!tree.isNull());
}

void EgcasTest_Parser::falseTestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("(45+a)-:n__j5_lm__3+kl__9-js_z"));

        QVERIFY(tree.isNull());
}

void EgcasTest_Parser::treeTestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("(45+a)-n__j5_lm__3+kl__9-js_z"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();               
        }
        QVERIFY(!tree.isNull());

        EgcBaseNode base;
        QVERIFY(base.setChild(0, *tree.take()));
        EgcNodeIterator iter(base);
        EgcNode* nodePointer;

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::RootNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::RootNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::RootNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::ParenthesisNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::RootNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QVERIFY(static_cast<EgcNumberNode*>(nodePointer)->getValue() == QString("45"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("a"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("n_j5"));
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getSubscript() == QString("lm_3"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("kl_9"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("js"));
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getSubscript() == QString("z"));
}

QTEST_MAIN(EgcasTest_Parser)

#include "tst_egcastest_parser.moc"
