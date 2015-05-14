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
        void testTransferProperties();
        void testInsertDelete();
        void testMaximaVisitor();
private:
        EgcExpressionNode* addChild(EgcExpressionNode&parent, EgcExpressionNodeType type, QString number = "0");
        EgcExpressionNode* addLeftChild(EgcExpressionNode&parent, EgcExpressionNodeType type, QString number = "0");
        EgcExpressionNode* addRightChild(EgcExpressionNode&parent, EgcExpressionNodeType type, QString number = "0");
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
        numberExpression->setValue(QString("200.1"));
        numberExpression2->setValue("90.365");
        rootChildExpression->setLeftChild(*numberExpression);
        rootExpression.setLeftChild(*rootChildExpression);
        rootExpression.setRightChild(*numberExpression2);

        EgcRootExpressionNode copyExpression(rootExpression);

        auto *copyChild = static_cast<EgcBinaryExpressionNode*>(copyExpression.getLeftChild());
        auto *numberChild1 = static_cast<EgcNumberExpressionNode*>(copyChild->getLeftChild());
        auto *numberChild2 = static_cast<EgcNumberExpressionNode*>(copyExpression.getRightChild());
        QVERIFY(numberChild1->getValue() == "200.1");
        QVERIFY(numberChild2->getValue() == "90.365");
        QVERIFY(copyChild->valid() == false);
        QVERIFY(numberChild2->valid() == true);

}

void EgcasTest_Structural::testTransferProperties()
{

        /*  This tree is tested within the transfer test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 5 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 4 |
                                |---|       |---|
        */

        EgcFormulaExpression formula(EgcExpressionNodeType::RootNode);

        EgcRootExpressionNode* node1 = static_cast<EgcRootExpressionNode*>(formula.getRootElement());
        auto *node2 = new EgcRootExpressionNode();
        auto *node3 = new EgcNumberExpressionNode();
        auto *node4 = new EgcNumberExpressionNode();
        auto *node5 = new EgcNumberExpressionNode();
        node3->setValue("3");
        node4->setValue("4");
        node5->setValue("5");
        node1->setLeftChild(*node2);
        node1->setRightChild(*node5);
        node2->setLeftChild(*node3);
        node2->setRightChild(*node4);

        bool retval;
        EgcRootExpressionNode *transferNode1 = static_cast<EgcRootExpressionNode*>(
                                        EgcExpressionNodeCreator::create(EgcExpressionNodeType::RootNode));
        transferNode1->setLeftChild(*(new EgcEmptyExpressionNode()));
        retval = node2->transferPropertiesTo(*transferNode1);
        QVERIFY(retval == false);
        EgcRootExpressionNode *transferNode2 = static_cast<EgcRootExpressionNode*>(
                                        EgcExpressionNodeCreator::create(EgcExpressionNodeType::RootNode));
        transferNode2->setRightChild(*(new EgcEmptyExpressionNode()));
        retval = node2->transferPropertiesTo(*transferNode2);
        QVERIFY(retval == false);
        EgcParenthesisExpressionNode *transferNode3 = static_cast<EgcParenthesisExpressionNode*>(
                                        EgcExpressionNodeCreator::create(EgcExpressionNodeType::ParenthesisNode));
        transferNode3->setChild(*(new EgcEmptyExpressionNode()));
        retval = node2->transferPropertiesTo(*transferNode3);
        QVERIFY(retval == false);
        EgcRootExpressionNode *transferNode4 = static_cast<EgcRootExpressionNode*>(
                                        EgcExpressionNodeCreator::create(EgcExpressionNodeType::RootNode));
        retval = node2->transferPropertiesTo(*transferNode4);
        QVERIFY(retval == true);

        QVERIFY(node2->getLeftChild() == nullptr);
        QVERIFY(node2->getRightChild() == nullptr);
        QVERIFY(node2->getParent() == nullptr);
        QVERIFY(node1->getLeftChild() == transferNode4);

        QVERIFY((static_cast<EgcNumberExpressionNode*>(transferNode4->getLeftChild()))->getValue() == "3");
        QVERIFY((static_cast<EgcNumberExpressionNode*>(transferNode4->getRightChild()))->getValue() == "4");
        QVERIFY(transferNode4->getParent() == node1);

        delete(node2);

        QVERIFY((static_cast<EgcNumberExpressionNode*>(transferNode4->getLeftChild()))->getValue() == "3");
        QVERIFY((static_cast<EgcNumberExpressionNode*>(transferNode4->getRightChild()))->getValue() == "4");
        QVERIFY(transferNode4->getParent() == node1);
}

void EgcasTest_Structural::testIterator()
{
        //-----------------------------------------------------------------------------
        /*This tree is tested within the iterator test below
        //-----------------------------------------------------------------------------

                                               |---|
                                               | 1 |  (Numbernode)
                                               |---|
        */

        //test iterator with big structure
        EgcFormulaExpression formula3(EgcExpressionNodeType::NumberNode);
        EgcNodeIteratorState stepState;
        EgcExpressionNode* nodePointer;

        //test step iterator
        EgcExpressionNodeIterator iter7(formula3);

        iter7.toFront();

        QVERIFY(iter7.hasNext() == true);
        QVERIFY(iter7.hasPrevious() == false);
        nodePointer = &(iter7.next(stepState));
        QVERIFY(nodePointer == formula3.getRootElement());
        QVERIFY(iter7.hasNext() == false);

        nodePointer = &(iter7.next(stepState));
        QVERIFY(nodePointer == formula3.getRootElement());

        nodePointer = &(iter7.next(stepState));
        QVERIFY(nodePointer == formula3.getRootElement());

        iter7.toBack();

        QVERIFY(iter7.hasPrevious() == true);
        QVERIFY(iter7.hasNext() == false);
        nodePointer = &(iter7.previous(stepState));
        QVERIFY(nodePointer == formula3.getRootElement());
        QVERIFY(iter7.hasPrevious() == false);

        nodePointer = &(iter7.previous(stepState));
        QVERIFY(nodePointer == formula3.getRootElement());


        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       nullptr
                                | 3 |
                                |---|
        */

        EgcFormulaExpression formula(EgcExpressionNodeType::RootNode);

        EgcExpressionNodeIterator iter(formula);
        QVERIFY(iter.hasNext() == true);
        QVERIFY(iter.hasPrevious() == false);

        EgcRootExpressionNode* rootExpression = static_cast<EgcRootExpressionNode*>(formula.getRootElement());
        auto *rootChildExpression = new EgcRootExpressionNode();
        auto *numberExpression = new EgcNumberExpressionNode();
        auto *numberExpression2 = new EgcNumberExpressionNode();
        QVERIFY(numberExpression->isLeaf() == true);
        numberExpression->setValue("200.1");
        numberExpression2->setValue("90.365");
        rootChildExpression->setLeftChild(*numberExpression);
        rootExpression->setLeftChild(*rootChildExpression);
        QVERIFY(rootExpression->isLeaf() == false);
        rootExpression->setRightChild(*numberExpression2);
        QVERIFY(rootExpression->isLeaf() == false);

        //-------------------test this structure with iterator--------------------


        EgcExpressionNodeIterator iter6(formula);

        //test hasNext and hasPrevious
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(iter6.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter6.peekNext()) == rootExpression);
        QVERIFY(&(iter6.peekPrevious()) == rootExpression);

        //test next functions (node 1)
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 3
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 4
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter6.hasNext() == false);

        iter6.toBack();

        //node 1
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 4
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 1
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 3
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 1
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == false);

        //test overflow
        nodePointer = &(iter6.previous(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);
        QVERIFY(iter6.hasNext() == true);

        iter6.toFront();

        //node 1
        QVERIFY(iter6.hasPrevious() == false);
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 3
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 4
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter6.hasNext() == false);

        //check overflow
        nodePointer = &(iter6.next(stepState));
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //test find routines
        iter6.toFront();

        QVERIFY(iter6.findNext(EgcExpressionNodeType::NumberNode) == true);
        QVERIFY(&(iter6.previous(stepState)) == numberExpression);

        iter6.toBack();

        QVERIFY(iter6.findPrevious(EgcExpressionNodeType::NumberNode) == true);
        QVERIFY(&(iter6.next(stepState)) == numberExpression2);


        //-----------------------------------------------------------------------------
        /*This tree is tested within the iterator test below
        //-----------------------------------------------------------------------------
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 7 |
                                       |---|             |---|
                                   /        \               /       \
                                  /          \           /            \
                              |---|         |---|      |---|         |---|
                              | 3 |         | 6 |      | 8 |         | 9 |
                              |---|         |---|      |---|         |---|
                             /  \                                  /        \
                            /    \                               /             \
                         |---|  |---|                      |---|             |---|
                         | 4 |  | 5 |                      | 10|             | 12|
                         |---|  |---|                      |---|             |---|
                                                             |
                                                           |---|
                                                           | 11|
                                                           |---|
        */

        //test iterator with big structure        
        EgcFormulaExpression formula2(EgcExpressionNodeType::RootNode);

        EgcExpressionNode* rootExpression2;
        EgcExpressionNode* node2;
        EgcExpressionNode* node3;
        EgcExpressionNode* node7;
        EgcExpressionNode* node9;
        EgcExpressionNode* node10;
        EgcExpressionNode* node4;
        EgcExpressionNode* node5;
        EgcExpressionNode* node6;
        EgcExpressionNode* node8;
        EgcExpressionNode* node11;
        EgcExpressionNode* node12;

        rootExpression2 = static_cast<EgcRootExpressionNode*>(formula2.getRootElement());
        node2 = addLeftChild(*rootExpression2, EgcExpressionNodeType::RootNode);
        QVERIFY(node2 != nullptr);

        QVERIFY(addRightChild(*node2, EgcExpressionNodeType::NumberNode, "6.0") != nullptr);
        node6 = static_cast<EgcBinaryExpressionNode*>(node2)->getRightChild();

        node3 = addLeftChild(*node2, EgcExpressionNodeType::RootNode);

        QVERIFY(addLeftChild(*node3, EgcExpressionNodeType::NumberNode, "4.0") != nullptr);
        QVERIFY(addRightChild(*node3, EgcExpressionNodeType::NumberNode, "5.0") != nullptr);
        node4 = static_cast<EgcBinaryExpressionNode*>(node3)->getLeftChild();
        node5 = static_cast<EgcBinaryExpressionNode*>(node3)->getRightChild();

        node7 = addRightChild(*rootExpression2, EgcExpressionNodeType::RootNode);
        QVERIFY(node7 != nullptr);        

        QVERIFY(addLeftChild(*node7, EgcExpressionNodeType::NumberNode, "8.0") != nullptr);
        node8 = static_cast<EgcBinaryExpressionNode*>(node7)->getLeftChild();

        node9 = addRightChild(*node7, EgcExpressionNodeType::RootNode);
        QVERIFY(node9 != nullptr);

        node10 = addLeftChild(*node9, EgcExpressionNodeType::ParenthesisNode);
        QVERIFY(node10 != nullptr);

        QVERIFY(addChild(*node10, EgcExpressionNodeType::NumberNode, "11.0") != nullptr);
        node11 = static_cast<EgcUnaryExpressionNode*>(node10)->getChild();

        QVERIFY(addRightChild(*node9, EgcExpressionNodeType::NumberNode, "12.0") != nullptr);
        node12 = static_cast<EgcBinaryExpressionNode*>(node9)->getRightChild();
        
        EgcBinaryExpressionNode* binaryNode;

        //-----------------------------------------------------------------------------
        //test step iterator
        //-----------------------------------------------------------------------------
        EgcExpressionNodeIterator iter5(formula2);

        //test hasNext and hasPrevious
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(iter5.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter5.peekNext()) == rootExpression2);

        //test next functions (node 1)
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node4
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 5
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 6
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 8
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 10
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 11
        nodePointer = &(iter5.next(stepState));
        EgcUnaryExpressionNode* unaryNode = static_cast<EgcUnaryExpressionNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 10
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 12
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == false);

        //jump over to the start if one doesn't look at hasNext
        //node 1
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //test the previous function
        iter5.toBack();

        //-----------------------------------------------------------------------------
        //test step iterator backwards
        //-----------------------------------------------------------------------------

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == false);
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 12
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 10
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 11
        nodePointer = &(iter5.previous(stepState));
        unaryNode = static_cast<EgcUnaryExpressionNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 10
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 8
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 6
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 5
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node4
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == false);

        //jump over to the start if one doesn't look at hasPrevious
        //node 1
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //test turnarounds (from previous to next)
        iter5.toBack();

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == false);
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.next(stepState));
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == true);
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == false);

        //test turnarounds (from next to previous)
        iter5.toFront();

        //test next functions (node 1)
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous(stepState));
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcNodeIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == false);

        // ------------------------------------------------------
        //           test nextParent
        // ------------------------------------------------------
        iter5.toFront();
        iter5.next(stepState);
        iter5.next(stepState);
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node3);
        //is now between node 3 and 4
        EgcExpressionNode *parentNode = &(iter5.nextParent());
        QVERIFY(parentNode == node2);
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node6);

        iter5.toFront();
        parentNode = &(iter5.nextParent());
        QVERIFY(parentNode == rootExpression2);

        iter5.toFront();
        while (nodePointer != node11) {
                nodePointer = &(iter5.next(stepState));
        }
        parentNode = &(iter5.nextParent());
        QVERIFY(parentNode == node10);
        nodePointer = &(iter5.next(stepState));
        QVERIFY(nodePointer == node9);
        QVERIFY(stepState == EgcNodeIteratorState::MiddleIteration);
}

void EgcasTest_Structural::testInsertDelete()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 5 |
                                |---|       |---|
        */

        EgcFormulaExpression formula4(EgcExpressionNodeType::RootNode);
        EgcNodeIteratorState state;
        EgcExpressionNode *nodePointer;
        EgcExpressionNodeIterator iter8(formula4);
        EgcExpressionNode *node1;
        EgcExpressionNode *node3;
        EgcExpressionNode *node2;
        EgcExpressionNode *node4;
        EgcExpressionNode *node5;

        nodePointer = &(iter8.next(state));
        node1 = nodePointer;
        iter8.insert(EgcExpressionNodeType::RootNode);
        nodePointer = &(iter8.next(state));
        node2 = nodePointer;
        nodePointer = &(iter8.next(state));
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcExpressionNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberExpressionNode*>(nodePointer)->setValue("3.452");
        node3 = nodePointer;
        nodePointer = &(iter8.next(state));
        nodePointer = &(iter8.next(state));
        node5 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcExpressionNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberExpressionNode*>(nodePointer)->setValue("5.647");
        node5 = nodePointer;
        node4 = static_cast<EgcBinaryExpressionNode*>(node1)->getRightChild();

        //test if the nodes are in the order intended
        iter8.toFront();
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::LeftIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcNodeIteratorState::LeftIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node3);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node5);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcNodeIteratorState::RightIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node4);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::RightIteration);


        //now we remove node 2
        iter8.toFront();
        (void) iter8.next(state);
        (void) iter8.next(state);
        iter8.remove();

        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == formula4.getRootElement());
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);

        //test if the nodes are in the order intended
        iter8.toFront();
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::LeftIteration);
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::EmptyNode);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node4);
        QVERIFY(state == EgcNodeIteratorState::MiddleIteration);
        nodePointer = &(iter8.next(state));
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcNodeIteratorState::RightIteration);




}

void EgcasTest_Structural::testMaximaVisitor()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 5 |
                                |---|       |---|
        */

        EgcFormulaExpression formula4(EgcExpressionNodeType::RootNode);
        EgcNodeIteratorState state;
        EgcExpressionNode *nodePointer;
        EgcExpressionNodeIterator iter8(formula4);
        EgcExpressionNode *node1;
        EgcExpressionNode *node3;
        EgcExpressionNode *node2;
        EgcExpressionNode *node4;
        EgcExpressionNode *node5;

        nodePointer = &(iter8.next(state));
        node1 = nodePointer;
        iter8.insert(EgcExpressionNodeType::RootNode);
        nodePointer = &(iter8.next(state));
        node2 = nodePointer;
        nodePointer = &(iter8.next(state));
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcExpressionNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberExpressionNode*>(nodePointer)->setValue("30.452");
        node3 = nodePointer;
        nodePointer = &(iter8.next(state));
        nodePointer = &(iter8.next(state));
        node5 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcExpressionNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcExpressionNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberExpressionNode*>(nodePointer)->setValue("3");
        node5 = nodePointer;
        nodePointer = &(iter8.next(state));
        nodePointer = &(iter8.next(state));
        nodePointer = &(iter8.next(state));
        nodePointer = iter8.replace(*nodePointer, EgcExpressionNodeType::NumberNode);
        static_cast<EgcNumberExpressionNode*>(nodePointer)->setValue("2");
        node4 = nodePointer;

        //test copy constructor of the formula expression
        EgcFormulaExpression formula5 = EgcFormulaExpression(formula4);
        EgcExpressionNode *node1_5 = formula5.getRootElement();
        QVERIFY(node1 != node1_5);
        nodePointer = static_cast<EgcRootExpressionNode*>(node1_5)->getRightChild();
        QVERIFY(static_cast<EgcNumberExpressionNode*>(nodePointer)->getValue() == "2");

        //test maxima visitor
        EgcMaximaVisitor maximaVisitor(formula4);
        QString result(maximaVisitor.getResult());
        QVERIFY(result == QString("((30.452)^(1/3))^(1/2);\n"));
        QVERIFY(formula4.getCASKernelCommand() == QString("((30.452)^(1/3))^(1/2);\n"));

        //test maxima visitor with copied formula
        EgcMaximaVisitor maximaVisitor2(formula5);
        QString result2(maximaVisitor2.getResult());
        QVERIFY(result2 == QString("((30.452)^(1/3))^(1/2);\n"));
        QVERIFY(formula5.getCASKernelCommand() == QString("((30.452)^(1/3))^(1/2);\n"));

        //test math ml visitor
        EgcMathMlVisitor mathMlVisitor(formula4);
        result = mathMlVisitor.getResult();
        QVERIFY(result == QString("<math><mroot><mrow><mroot><mrow><mn>30.452</mn></mrow><mrow><mn>3</mn></mrow></mroot></mrow><mrow><mn>2</mn></mrow></mroot></math>"));
        QVERIFY(formula4.getMathMlCode() == QString("<math><mroot><mrow><mroot><mrow><mn>30.452</mn></mrow><mrow><mn>3</mn></mrow></mroot></mrow><mrow><mn>2</mn></mrow></mroot></math>"));
}

EgcExpressionNode*EgcasTest_Structural::addChild(EgcExpressionNode& parent, EgcExpressionNodeType type, QString number)
{
        if (parent.isUnaryExpression()) {
                EgcUnaryExpressionNode& node = static_cast<EgcUnaryExpressionNode&>(parent);
                if (   type == EgcExpressionNodeType::RootNode
                                || type == EgcExpressionNodeType::NumberNode
                                || type == EgcExpressionNodeType::ParenthesisNode) {
                        node.setChild(*EgcExpressionNodeCreator::create(type));
                        if (node.getChild() != nullptr) {
                                if (    type == EgcExpressionNodeType::RootNode
                                                || type == EgcExpressionNodeType::ParenthesisNode) {
                                        return node.getChild();
                                } else {
                                        EgcNumberExpressionNode* num = static_cast<EgcNumberExpressionNode*>(node.getChild());
                                        num->setValue(number);
                                        return node.getChild();
                                }

                        }
                }
        }

        return nullptr;
}

EgcExpressionNode*EgcasTest_Structural::addLeftChild(EgcExpressionNode& parent, EgcExpressionNodeType type, QString number)
{
        if (parent.isBinaryExpression()) {
                EgcBinaryExpressionNode& node = static_cast<EgcBinaryExpressionNode&>(parent);
                if (   type == EgcExpressionNodeType::RootNode
                                || type == EgcExpressionNodeType::NumberNode
                                || type == EgcExpressionNodeType::ParenthesisNode) {
                        node.setLeftChild(*EgcExpressionNodeCreator::create(type));
                        if (node.getLeftChild() != nullptr) {
                                if (    type == EgcExpressionNodeType::RootNode
                                                || type == EgcExpressionNodeType::ParenthesisNode) {
                                        return node.getLeftChild();
                                } else {
                                        EgcNumberExpressionNode* num = static_cast<EgcNumberExpressionNode*>(node.getLeftChild());
                                        num->setValue(number);
                                        return node.getLeftChild();
                                }

                        }
                }
        }

        return nullptr;
}

EgcExpressionNode* EgcasTest_Structural::addRightChild(EgcExpressionNode& parent, EgcExpressionNodeType type, QString number)
{
        if (parent.isBinaryExpression()) {
                EgcBinaryExpressionNode& node = static_cast<EgcBinaryExpressionNode&>(parent);
                if (   type == EgcExpressionNodeType::RootNode
                                || type == EgcExpressionNodeType::NumberNode
                                || type == EgcExpressionNodeType::ParenthesisNode) {
                        node.setRightChild(*EgcExpressionNodeCreator::create(type));
                        if (node.getRightChild() != nullptr) {
                                if (    type == EgcExpressionNodeType::RootNode
                                                || type == EgcExpressionNodeType::ParenthesisNode) {
                                        return node.getRightChild();
                                } else {
                                        EgcNumberExpressionNode* num = static_cast<EgcNumberExpressionNode*>(node.getRightChild());
                                        num->setValue(number);
                                        return node.getRightChild();
                                }

                        }
                }
        }

        return nullptr;
}


QTEST_MAIN(EgcasTest_Structural)

#include "tst_egcastest_structural.moc"
