#include <QString>
#include <QtTest>
#include <iostream>
#include "egckernelparser.h"
#include "egcnodes.h"
#include "egcexpressionnodeiterator.h"
#include "egcformulaexpression.h"
#include "egcexpressionnodecreator.h"
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
        int res;

        EgcKernelParser parser;
        EgcBaseExpressionNode *tree;
        res = parser.parseKernelOutput("(45+a)-n__j5_lm__3+kl__9-js_z", &tree);
        if (!res) {
                std::cout << parser.getErrorMessage().toStdString();
        }

        delete tree;

        QVERIFY(res != 0);
}

void EgcasTest_Parser::falseTestParser()
{
        int res;

        EgcKernelParser parser;
        EgcBaseExpressionNode *tree;
        res = parser.parseKernelOutput("(45+a)-:n__j5_lm__3+kl__9-js_z", &tree);

        delete tree;

        QVERIFY(res == 0);
}

void EgcasTest_Parser::treeTestParser()
{
        int res;

        EgcKernelParser parser;
        EgcBaseExpressionNode *tree;
        res = parser.parseKernelOutput("(45+a)-n__j5_lm__3+kl__9-js_z", &tree);
        if (!res) {
                std::cout << parser.getErrorMessage().toStdString();
        }


        EgcExpressionNodeIterator iter(*tree);
        EgcNodeIteratorState stepState;
        EgcExpressionNode* nodePointer;

        //test next functions (node 1)
        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::BaseNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::RootNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::RootNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::RootNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::ParenthesisNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::RootNode);

        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::NumberNode);
        QVERIFY(static_cast<EgcNumberExpressionNode*>(nodePointer)->getValue() == QString("45"));

        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getValue() == QString("a"));

        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getValue() == QString("n_j5"));
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getSubscript() == QString("lm_3"));

        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getValue() == QString("kl_9"));

        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        nodePointer = &(iter.next(stepState));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::VariableNode);
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getValue() == QString("js"));
        QVERIFY(static_cast<EgcVariableExpressionNode*>(nodePointer)->getSubscript() == QString("z"));

        delete tree;

        QVERIFY(res != 0);
}

QTEST_MAIN(EgcasTest_Parser)

#include "tst_egcastest_parser.moc"
