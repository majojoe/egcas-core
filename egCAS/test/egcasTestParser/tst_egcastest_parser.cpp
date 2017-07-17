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
#include "casKernel/parser/abstractkernelparser.h"
#include "casKernel/parser/restructparserprovider.h"

//implementation of some mock classes for restruct parser
class EgcTestKernelParser : public AbstractKernelParser
{
public:
        EgcTestKernelParser() {}
        virtual ~EgcTestKernelParser() {}
        virtual EgcNode* restructureFormula(const QString& strToParse, NodeIterReStructData& iterData, int* errCode) override {return nullptr;};
};

AbstractKernelParser* RestructParserProvider::s_parser = nullptr;
RestructParserProvider::RestructParserProvider()
{
        if (s_parser == nullptr)
                s_parser = new EgcTestKernelParser();
}
RestructParserProvider::~RestructParserProvider() {}
AbstractKernelParser* RestructParserProvider::getRestructParser(void) { return s_parser;}



class EgcasTest_Parser : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Parser() {}

private Q_SLOTS:
        void basicTestParser();
        void falseTestParser();
        void treeTestParser();
        void fncTreeTestParser();
        void fncOperations1TestParser();
        void fncOperations2TestParser();
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
        tree.reset(parser.parseKernelOutput("(45+a)-:n__j5_1lm__3+kl__9-js_1z"));

        QVERIFY(tree.isNull());
}

void EgcasTest_Parser::treeTestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        //variable n_j&int;5 (integral encoding) decoded as n__j_2x222B_35
        tree.reset(parser.parseKernelOutput("(45+a)-n__j_2x222B_35_1lm__3+kl__9-js_1z"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();               
        }
        QVERIFY(!tree.isNull());

        EgcBaseNode base;
        QVERIFY(base.setChild(0, *tree.take()));
        EgcNodeIterator iter(base);
        EgcNode* nodePointer;

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::MinusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::MinusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::ParenthesisNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

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
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("n_j∫5"));
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getSubscript() == QString("lm_3"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("kl_9"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("js"));
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getSubscript() == QString("z"));
}

void EgcasTest_Parser::fncTreeTestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("k+function1(45+a,Pt,3,546.1)"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcBaseNode base;
        QVERIFY(base.setChild(0, *tree.take()));
        EgcNodeIterator iter(base);
        EgcNode* nodePointer;

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("k"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::FunctionNode);

        nodePointer = &(iter.next());
        //Plus sign in first function argument
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QCOMPARE(static_cast<EgcNumberNode*>(nodePointer)->getValue(), QString("45"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QCOMPARE(static_cast<EgcVariableNode*>(nodePointer)->getValue(), QString("a"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QCOMPARE(static_cast<EgcVariableNode*>(nodePointer)->getValue(), QString("Pt"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QCOMPARE(static_cast<EgcNumberNode*>(nodePointer)->getValue(), QString("3"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QCOMPARE(static_cast<EgcNumberNode*>(nodePointer)->getValue(), QString("546.1"));
}

void EgcasTest_Parser::fncOperations1TestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("k=-x^45+a*3"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcBaseNode base;
        QVERIFY(base.setChild(0, *tree.take()));
        EgcNodeIterator iter(base);
        EgcNode* nodePointer;

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EqualNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableNode*>(nodePointer)->getValue() == QString("k"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::PlusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::ExponentNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::UnaryMinusNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QCOMPARE(static_cast<EgcVariableNode*>(nodePointer)->getValue(), QString("x"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QCOMPARE(static_cast<EgcNumberNode*>(nodePointer)->getValue(), QString("45"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::MultiplicationNode);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::VariableNode);
        QCOMPARE(static_cast<EgcVariableNode*>(nodePointer)->getValue(), QString("a"));

        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        nodePointer = &(iter.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::NumberNode);
        QCOMPARE(static_cast<EgcNumberNode*>(nodePointer)->getValue(), QString("3"));
}

void EgcasTest_Parser::fncOperations2TestParser()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("ost:rn^45.8+a/3"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcBaseNode base;
        QVERIFY(base.setChild(0, *tree.take()));

        EgcFormulaEntity formula(*base.takeOwnership(*base.getChild(0)));
        QVERIFY(formula.getCASKernelCommand().contains("ost:((rn)^((45.8)))+(((a))/((3)))") == true);
}


QTEST_MAIN(EgcasTest_Parser)

#include "tst_egcastest_parser.moc"
