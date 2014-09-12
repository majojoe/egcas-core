#include <QString>
#include <QtTest>
#include "tst_egcastest_structural.h"

class EgcasTest_Structural : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Structural() {}

private Q_SLOTS:
        void testChildDeletion();
        void testCopyConstructors();
};


bool EgcUnaryExpressionNodeTestChild::deleted = false;
bool EgcBinaryExpressionNodeTestChild::deleted = false;
bool EgcBinaryExpressionNodeTestChild2::deleted = false;

void EgcasTest_Structural::testChildDeletion()
{
        //check if childs getting deleted
        EgcUnaryExpressionNodeTest *parent = new EgcUnaryExpressionNodeTest();
        EgcUnaryExpressionNodeTestChild *child = new EgcUnaryExpressionNodeTestChild();

        parent->setChild(*child);

        QVERIFY(child->deleted == false);
        delete(parent);
        QVERIFY(child->deleted == true);

        //do the same with the binary class
        EgcBinaryExpressionNodeTest *parent_bin = new EgcBinaryExpressionNodeTest();
        EgcBinaryExpressionNodeTestChild *child_bin = new EgcBinaryExpressionNodeTestChild();
        EgcBinaryExpressionNodeTestChild2 *child2_bin = new EgcBinaryExpressionNodeTestChild2();

        parent_bin->setLeftChild(*child_bin);
        parent_bin->setRightChild(*child2_bin);

        QVERIFY(child_bin->deleted == false);
        QVERIFY(child2_bin->deleted == false);
        delete(parent_bin);
        QVERIFY(child_bin->deleted == true);
        QVERIFY(child2_bin->deleted == true);
}

void EgcasTest_Structural::testCopyConstructors()
{
        //test copy constructor of binary expression
        EgcRootExpressionNode rootExpression;
        auto *rootChildExpression = new EgcRootExpressionNode();
        auto *numberExpression = new EgcNumberExpressionNode();
        auto *numberExpression2 = new EgcNumberExpressionNode();
        numberExpression->setValue(200.1);
        numberExpression2->setValue(90.365);
        rootChildExpression->setLeftChild(*numberExpression);
        rootExpression.setLeftChild(*rootChildExpression);
        rootExpression.setRightChild(*numberExpression2);

        EgcRootExpressionNode copyExpression(rootExpression);

        auto *copyChild = static_cast<EgcBinaryExpressionNode*>(copyExpression.getLeftChild());
        auto *numberChild1 = static_cast<EgcNumberExpressionNode*>(copyChild->getLeftChild());
        auto *numberChild2 = static_cast<EgcNumberExpressionNode*>(copyExpression.getRightChild());
        QVERIFY(numberChild1->getValue() == 200.1);
        QVERIFY(numberChild2->getValue() == 90.365);
        QVERIFY(copyChild->valid() == false);
        QVERIFY(numberChild2->valid() == true);

}

QTEST_MAIN(EgcasTest_Structural)

#include "tst_egcastest_structural.moc"
