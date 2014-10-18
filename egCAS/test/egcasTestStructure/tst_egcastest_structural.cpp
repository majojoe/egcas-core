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
private:
        EgcExpressionNode* addChild(EgcExpressionNode&parent, EgcExpressionNodeType type, qreal number = 0);
        EgcExpressionNode* addLeftChild(EgcExpressionNode&parent, EgcExpressionNodeType type, qreal number = 0);
        EgcExpressionNode* addRightChild(EgcExpressionNode&parent, EgcExpressionNodeType type, qreal number = 0);
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

        EgcExpressionNode* nodePointer;
        EgcFormulaExpression formula(EgcExpressionNodeType::RootNode);
        EgcRootExpressionNode& rootExpression = static_cast<EgcRootExpressionNode&>(formula.getRootElement());

        EgcExpressionNodeIterator iter(formula);
        QVERIFY(iter.hasNext() == false);
        QVERIFY(iter.hasPrevious() == false);

        auto *rootChildExpression = new EgcRootExpressionNode();
        auto *numberExpression = new EgcNumberExpressionNode();
        auto *numberExpression2 = new EgcNumberExpressionNode();
        QVERIFY(rootExpression.isLeaf() == true);
        QVERIFY(numberExpression->isLeaf() == true);
        numberExpression->setValue(200.1);
        numberExpression2->setValue(90.365);
        rootChildExpression->setLeftChild(*numberExpression);
        rootExpression.setLeftChild(*rootChildExpression);
        QVERIFY(rootExpression.isLeaf() == false);
        rootExpression.setRightChild(*numberExpression2);
        QVERIFY(rootExpression.isLeaf() == false);

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
                                /          \               /       \
                              /              \           /               \
                        |---|             |---|        |---|             |---|
                        | 3 |             | 6 |        | 8 |             | 9 |
                        |---|             |---|        |---|             |---|
                      /        \                                        /        \
                    /            \                                    /             \
              |---|             |---|                             |---|             |---|
              | 4 |             | 5 |                             | 10|             | 12|
              |---|             |---|                             |---|             |---|
                                                                    |
                                                                  |---|
                                                                  | 11|
                                                                  |---|
        */

        //test iterator with big structure        
        EgcFormulaExpression formula2(EgcExpressionNodeType::RootNode);
        EgcExpressionNode* rootExpression2 = &(formula2.getRootElement());
        EgcExpressionNode* node2;
        EgcExpressionNode* node3;
        EgcExpressionNode* node7;
        EgcExpressionNode* node9;
        EgcExpressionNode* node10;

        EgcExpressionNodeIterator iter2(formula2);

        node2 = addLeftChild(*rootExpression2, EgcExpressionNodeType::RootNode);
        QVERIFY(node2 != nullptr);

        QVERIFY(addRightChild(*node2, EgcExpressionNodeType::NumberNode, 6.0) != nullptr);

        node3 = addLeftChild(*node2, EgcExpressionNodeType::RootNode);

        QVERIFY(addLeftChild(*node3, EgcExpressionNodeType::NumberNode, 4.0) != nullptr);
        QVERIFY(addRightChild(*node3, EgcExpressionNodeType::NumberNode, 5.0) != nullptr);

        node7 = addRightChild(*rootExpression2, EgcExpressionNodeType::RootNode);
        QVERIFY(node7 != nullptr);

        QVERIFY(addLeftChild(*node7, EgcExpressionNodeType::NumberNode, 8.0) != nullptr);

        node9 = addRightChild(*node7, EgcExpressionNodeType::RootNode);
        QVERIFY(node9 != nullptr);

        node10 = addLeftChild(*node9, EgcExpressionNodeType::ParenthesisNode);
        QVERIFY(node10 != nullptr);

        QVERIFY(addChild(*node10, EgcExpressionNodeType::NumberNode, 11.0) != nullptr);

        QVERIFY(addRightChild(*node9, EgcExpressionNodeType::NumberNode, 12.0) != nullptr);
        
        EgcBinaryExpressionNode* binaryNode;

        //test hasNext and hasPrevious
        QVERIFY(iter2.hasNext() == true);
        QVERIFY(iter2.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter2.peekNext()) == rootExpression2);

        //test next functions (node 1)
        nodePointer = &(iter2.next());
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter2.hasNext() == true);
        
        //node 2
        nodePointer = &(iter2.next());
        QVERIFY(nodePointer == node2);

        //node 3
        nodePointer = &(iter2.next());
        QVERIFY(nodePointer == node3);

        //node4
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 5
        nodePointer = &(iter2.next());
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 6
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 7
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 8
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 9
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 10
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 11
        nodePointer = &(iter2.next());
        EgcUnaryExpressionNode* unaryNode = static_cast<EgcUnaryExpressionNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild());

        //node 12
        nodePointer = &(iter2.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        nodePointer = &(iter2.next());
        QVERIFY(nodePointer == rootExpression2);



        //test the same with previous functions
        iter2.toBack();

        //node 12
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 11
        nodePointer = &(iter2.previous());
        unaryNode = static_cast<EgcUnaryExpressionNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild());

        //node 10
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 9
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 8
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 7
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 6
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node 5
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //node4
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 3
        nodePointer = &(iter2.previous());
        QVERIFY(nodePointer == node3);

        //node 2
        nodePointer = &(iter2.previous());
        QVERIFY(nodePointer == node2);

        //node 1
        nodePointer = &(iter2.previous());
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter2.hasPrevious() == false);

        //the previous node must now be node 12
        nodePointer = &(iter2.previous());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());

        //test constructor for node
        EgcExpressionNodeIterator iter3(*node3);
        QVERIFY(&(iter3.peekNext()) == node3);

        //test incrementToNextNonChildNode
        EgcExpressionNodeIterator iter4(formula2);
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        //next node must be node6
        QVERIFY(&(iter4.incrementToNextNonChildNode(*node3)) == binaryNode->getRightChild());


        //-----------------------------------------------------------------------------
        //test step iterator
        //-----------------------------------------------------------------------------
        EgcStepNodeIterator iter5(formula2);

        //test hasNext and hasPrevious
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(iter5.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter5.peekNext()) == rootExpression2);

        //test next functions (node 1)
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);

        //node 2
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node2);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);

        //node 3
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node3);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);

        //node4
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 3
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node3);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 5
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 3
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node3);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::RightIteration);

        //node 2
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node2);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 6
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 2
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == node2);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::RightIteration);

        //node 1
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 7
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);

        //node 8
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 7
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 9
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);


        //node 10
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::LeftIteration);

        //node 11
        nodePointer = &(iter5.next());
        unaryNode = static_cast<EgcUnaryExpressionNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 10
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getLeftChild());

        //node 9
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 12
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::MiddleIteration);

        //node 9
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::RightIteration);

        //node 7
        nodePointer = &(iter5.next());
        binaryNode = static_cast<EgcBinaryExpressionNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getRightChild());
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::RightIteration);

        //node 1
        nodePointer = &(iter5.next());
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.getIterationState() == EgcStepIteratorState::RightIteration);
}

EgcExpressionNode*EgcasTest_Structural::addChild(EgcExpressionNode& parent, EgcExpressionNodeType type, qreal number)
{
        if (parent.isUnaryExpression()) {
                EgcUnaryExpressionNode& node = static_cast<EgcUnaryExpressionNode&>(parent);
                if (node.getChild() == nullptr) {
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
        }

        return nullptr;
}

EgcExpressionNode*EgcasTest_Structural::addLeftChild(EgcExpressionNode& parent, EgcExpressionNodeType type, qreal number)
{
        if (parent.isBinaryExpression()) {
                EgcBinaryExpressionNode& node = static_cast<EgcBinaryExpressionNode&>(parent);
                if (node.getLeftChild() == nullptr) {
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
        }

        return nullptr;
}

EgcExpressionNode* EgcasTest_Structural::addRightChild(EgcExpressionNode& parent, EgcExpressionNodeType type, qreal number)
{
        if (parent.isBinaryExpression()) {
                EgcBinaryExpressionNode& node = static_cast<EgcBinaryExpressionNode&>(parent);
                if (node.getRightChild() == nullptr) {
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
        }

        return nullptr;
}


QTEST_MAIN(EgcasTest_Structural)

#include "tst_egcastest_structural.moc"
