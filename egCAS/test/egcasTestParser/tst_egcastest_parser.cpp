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
