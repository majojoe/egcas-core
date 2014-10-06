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
        void testIterator();
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
        //verify parent references
        QVERIFY(parent_bin->getParent() == nullptr);
        QVERIFY(child_bin->getParent() == parent_bin);
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

void EgcasTest_Structural::testIterator()
{
        EgcExpressionNode* nodePointer;
        EgcFormulaExpression formula(EgcExpressionNodeType::RootNode);
        EgcRootExpressionNode& rootExpression = static_cast<EgcRootExpressionNode&>(formula.getRootElement());

        EgcExpressionNodeIterator iter(formula);
        QVERIFY(iter.hasNext() == false);
        QVERIFY(iter.hasPrevious() == false);

        auto *rootChildExpression = new EgcRootExpressionNode();
        auto *numberExpression = new EgcNumberExpressionNode();
        auto *numberExpression2 = new EgcNumberExpressionNode();
        numberExpression->setValue(200.1);
        numberExpression2->setValue(90.365);
        rootChildExpression->setLeftChild(*numberExpression);
        rootExpression.setLeftChild(*rootChildExpression);
        rootExpression.setRightChild(*numberExpression2);

        //test hasNext and hasPrevious
        QVERIFY(iter.hasNext() == true);
        QVERIFY(iter.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter.peekNext()) == &rootExpression);
        QVERIFY(&(iter.peekPrevious()) == rootExpression.getRightChild());

        //test next functions
        nodePointer = &(iter.next());
        QVERIFY(nodePointer == &rootExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(iter.hasNext() == false);

        iter.toBack();

        //test previous functions
        QVERIFY(iter.hasPrevious() == true);
        nodePointer = &(iter.previous());
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(iter.hasPrevious() == true);

        nodePointer = &(iter.previous());
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(iter.hasPrevious() == true);

        nodePointer = &(iter.previous());
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(iter.hasPrevious() == true);

        nodePointer = &(iter.previous());
        QVERIFY(nodePointer == &rootExpression);
        QVERIFY(iter.hasPrevious() == false);

        //test overflow
        nodePointer = &(iter.previous());
        QVERIFY(nodePointer == numberExpression2);

        iter.toFront();

        //test next functions
        nodePointer = &(iter.next());
        QVERIFY(nodePointer == &rootExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(iter.hasNext() == true);

        nodePointer = &(iter.next());
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(iter.hasNext() == false);

        //check overflow
        nodePointer = &(iter.next());
        QVERIFY(nodePointer == &rootExpression);

        //test find routines
        iter.toFront();

        QVERIFY(iter.findNext(EgcExpressionNodeType::NumberNode) == true);
        QVERIFY(&(iter.previous()) == numberExpression);

        iter.toBack();

        QVERIFY(iter.findPrevious(EgcExpressionNodeType::NumberNode) == true);
        QVERIFY(&(iter.next()) == numberExpression2);

}

QTEST_MAIN(EgcasTest_Structural)

#include "tst_egcastest_structural.moc"
