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


//#define PRINT_DBG_DATA

class EgcasTest_ScrIters : public QObject
{
        Q_OBJECT

public:
        EgcasTest_ScrIters() {}
private Q_SLOTS:
        void testIdNodeIter();
        void testIdNodeIterFW();
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
        formula.setRootElement(getTree("(1+sqrt(5))/2=1.618"));
        //generate id list
        (void) formula.getMathMlCode();
        EgcMathmlLookup& lookup = formula.getMathmlMappingRef();

#ifdef PRINT_DBG_DATA
        qDebug() << "===================================================";
        qDebug() << "print all mathml id's in the lookup table";
#endif //#ifdef PRINT_DBG_DATA

        QPair<EgcNode*, quint32> pair;
        foreach(pair, lookup.getList()) {
#ifdef PRINT_DBG_DATA
                qDebug() << "Node: " << pair.first << "Id: " << pair.second;
#endif //#ifdef PRINT_DBG_DATA
        }

        EgcIdNodeIter iter(formula);
        iter.toFront();

#ifdef PRINT_DBG_DATA
        qDebug() << "===================================================";
        qDebug() << "iterate forward: iteration over all nodes with a valid mathml id";
#endif //#ifdef PRINT_DBG_DATA
        while (iter.hasNext()) {
                EgcNode* node = &iter.next();
#ifdef PRINT_DBG_DATA
                qDebug() << "Node: " << node << "id: " << iter.id() << "state: " << static_cast<int>(iter.getState());
#endif //#ifdef PRINT_DBG_DATA
        }

        EgcNode* node;
        iter.toFront();

        QCOMPARE(iter.getNode().getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::RootNode);
        QCOMPARE(static_cast<int>(iter.id()), 4);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 5
        QCOMPARE(static_cast<int>(iter.id()), 2);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 5
        QCOMPARE(static_cast<int>(iter.id()), 2);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::RootNode);
        QCOMPARE(static_cast<int>(iter.id()), 4);
        QCOMPARE(iter.getState(), EgcIteratorState::RightIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 2
        QCOMPARE(static_cast<int>(iter.id()), 7);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 2
        QCOMPARE(static_cast<int>(iter.id()), 7);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::RightIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1.618
        QCOMPARE(static_cast<int>(iter.id()), 9);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1.618
        QCOMPARE(static_cast<int>(iter.id()), 9);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        // we iterate over the formula now from the end to the beginning
        //"((1+sqrt(5)))/2=1.618"

        iter.toBack();

#ifdef PRINT_DBG_DATA
        qDebug() << "===================================================";
        qDebug() << "iterate backward: iteration over all nodes with a valid mathml id";
#endif //#ifdef PRINT_DBG_DATA
        while (iter.hasPrevious()) {
                node = &iter.previous();
#ifdef PRINT_DBG_DATA
                qDebug() << "Node: " << node << "id: " << iter.id() << "state: " << static_cast<int>(iter.getState());
#endif //#ifdef PRINT_DBG_DATA
        }

        iter.toBack();

        QCOMPARE(iter.getNode().getNodeType(), EgcNodeType::NumberNode); //value 1.618
        QCOMPARE(static_cast<int>(iter.id()), 9);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1.618
        QCOMPARE(static_cast<int>(iter.id()), 9);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::RightIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 2
        QCOMPARE(static_cast<int>(iter.id()), 7);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 2
        QCOMPARE(static_cast<int>(iter.id()), 7);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::RootNode);
        QCOMPARE(static_cast<int>(iter.id()), 4);
        QCOMPARE(iter.getState(), EgcIteratorState::RightIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 5
        QCOMPARE(static_cast<int>(iter.id()), 2);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 5
        QCOMPARE(static_cast<int>(iter.id()), 2);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::RootNode);
        QCOMPARE(static_cast<int>(iter.id()), 4);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode); //value 1
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);
}

void EgcasTest_ScrIters::testIdNodeIterFW(void)
{
        //test equal functions
        EgcFormulaEntity formula;
        formula.setRootElement(getTree("(1+sqrt(5))/2=1.618"));
        //generate id list
        (void) formula.getMathMlCode();

        EgcIdNodeIter iter(formula);
        iter.toFront();
        EgcNode* node;

        //iterate 1 step forward and 1 step backwards
        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode);
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        //iterate 3 steps forward, 2 steps backwards and 4 steps forward
        iter.toFront();
        QCOMPARE(iter.getNode().getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode);
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasPrevious())
                node = &iter.previous();
        QCOMPARE(node->getNodeType(), EgcNodeType::DivisionNode);
        QCOMPARE(static_cast<int>(iter.id()), 8);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode);
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), false);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::NumberNode);
        QCOMPARE(static_cast<int>(iter.id()), 1);
        QCOMPARE(iter.getState(), EgcIteratorState::MiddleIteration);
        QCOMPARE(iter.rightSide(), true);

        if (iter.hasNext())
                node = &iter.next();
        QCOMPARE(node->getNodeType(), EgcNodeType::RootNode);
        QCOMPARE(static_cast<int>(iter.id()), 4);
        QCOMPARE(iter.getState(), EgcIteratorState::LeftIteration);
}

QTEST_MAIN(EgcasTest_ScrIters)

#include "tst_egcastest_screen_iterators.moc"
