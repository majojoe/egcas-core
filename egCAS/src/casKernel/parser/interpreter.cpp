/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
 * Copyright (c) 2015 Johannes Maier <maier_jo@gmx.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include <sstream>
#include <QVector>
#include <QScopedPointer>
#include "interpreter.h"
#include "../../structural/egcnodecreator.h"
#include "../../structural/egcnodes.h"

using namespace CASParser;

Interpreter::Interpreter() :
        m_scanner(*this),
        m_parser(m_scanner, *this),
        m_location(0),
        m_iterPointer1(nullptr),
        m_iterPointer2(nullptr),
        m_iterPointer3(nullptr)
{

}

Interpreter::~Interpreter()
{
        deleteDanglingNodes();
}

int Interpreter::parse(bool parseKernelResult)
{
        m_parseKernelResult = parseKernelResult;
        m_iterPointer1 = nullptr;
        m_iterPointer2 = nullptr;
        m_iterPointer3 = nullptr;
        m_location = 0;
        return m_parser.parse();
}

void Interpreter::clear() {
        m_location = 0;
        m_rootNode.reset(nullptr);
        deleteDanglingNodes();
#if (EGC_PARSER_DEBUG >= 3)
        m_parser.set_debug_level(3);
#endif //#if (EGC_PARSER_DEBUG >= 3)
}

void Interpreter::switchInputStream(std::istream *is) {
        m_scanner.switch_streams(is, NULL);
}

void Interpreter::increaseLocation(unsigned int loc) {
        m_location += loc;
#if (EGC_PARSER_DEBUG >= 3)
        cout << "increaseLocation(): " << loc << ", total = " << m_location << endl;
#endif //#if (EGC_PARSER_DEBUG >= 3)
}

unsigned int Interpreter::location() const {
        return m_location;
}

EgcNode* Interpreter::addBinaryExpression(EgcNodeType type, EgcNode* node0,
                                                    EgcNode* node1)
{
        QScopedPointer<EgcBinaryNode> node;
        node.reset(static_cast<EgcBinaryNode*>(EgcNodeCreator::create(type)));
        QScopedPointer<EgcNode> node0Tmp(node0);
        QScopedPointer<EgcNode> node1Tmp(node1);
        setNotDangling(node0);
        setNotDangling(node1);
        if (!node.isNull() && node0 && node1) {
                node->setChild(0, *node0Tmp.take());
                node->setChild(1, *node1Tmp.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

EgcNode* Interpreter::addUnaryExpression(EgcNodeType type, EgcNode* node0)
{
        QScopedPointer<EgcUnaryNode> node(static_cast<EgcUnaryNode*>(EgcNodeCreator::create(type)));
        QScopedPointer<EgcNode> node0Tmp(node0);
        setNotDangling(node0);
        if (!node.isNull() && node0) {
                node->setChild(0, *node0Tmp.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

bool Interpreter::removeParenthesisChild(EgcNode& parenthesisNode)
{
        if (parenthesisNode.getNodeType() != EgcNodeType::ParenthesisNode)
                return false;
        EgcContainerNode &pNode = static_cast<EgcContainerNode&>(parenthesisNode);

        EgcContainerNode *parent = parenthesisNode.getParent();
        if (!parent)
                return false;
        quint32 i;
        if (!parent->getIndexOfChild(parenthesisNode, i))
                return false;
        EgcNode* child;
        QScopedPointer<EgcNode> tempChild;
        child = pNode.getChild(0);
        if (child) {
                tempChild.reset(pNode.takeOwnership(*child));
                parent->setChild(i, *tempChild.take());
        }
        parent->setChild(i, *child);

        return true;
}

EgcNode* Interpreter::addStringNode(EgcNodeType type, const std::string& value)
{
        EgcNode *nodePtr = nullptr;
        QScopedPointer<EgcNode> node(static_cast<EgcNode*>(EgcNodeCreator::create(type)));
        if (!node.isNull()) {
                switch (type) {
                case EgcNodeType::NumberNode: {
                        EgcNumberNode* tmp = static_cast<EgcNumberNode*>(node.data());
                        tmp->setValue(QString::fromStdString(value));
                        break;
                }
                case EgcNodeType::VariableNode: {
                        EgcVariableNode* tmp = static_cast<EgcVariableNode*>(node.data());
                        tmp->setStuffedVar(QString::fromStdString(value));
                        break;
                }
                default: /*do nothing*/
                        break;
                }
                nodePtr = node.data();
                addDanglingNode(node.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }


        return nodePtr;
}

void Interpreter::setRootNode(EgcNode* node)
{
        m_rootNode.reset(node);
        setNotDangling(node);
}

EgcNode* Interpreter::getRootNode(void)
{
        return m_rootNode.take();
}


EgcNode* Interpreter::addFunction(const std::string& fncName, EgcArgumentsNode* argList)
{
        if (argList) {
                EgcNode* node = changeFlexExpressionType(EgcNodeType::FunctionNode, argList);
                EgcFunctionNode* function = static_cast<EgcFunctionNode*>(node);
                function->setName(QString::fromStdString(fncName));
                return static_cast<EgcNode*> (function);
        }

        return static_cast<EgcNode*> (argList);
}

EgcNode* Interpreter::addBuiltinFunction(const std::string& fncName, EgcArgumentsNode* argList)
{
        if (argList) {
                EgcNode* node = changeFlexExpressionType(EgcNodeType::FunctionNode, argList);
                EgcFunctionNode* function = static_cast<EgcFunctionNode*>(node);
                QString name = QString::fromStdString(fncName);
                if (name == QString("log"))
                        name = QString("ln");
                function->setName(name);
                return static_cast<EgcNode*> (function);
        }

        return static_cast<EgcNode*> (argList);
}

EgcNode* Interpreter::updateIterator(EgcNode* node0, int i)
{
        if (i == 1)
                m_iterPointer1 = node0;
        if (i == 2)
                m_iterPointer2 = node0;
        if (i == 3)
                m_iterPointer3 = node0;

        return node0;
}

EgcArgumentsNode* Interpreter::createArgList(EgcNode* expression)
{
        QScopedPointer<EgcArgumentsNode> node(static_cast<EgcArgumentsNode*>(EgcNodeCreator::create(EgcNodeType::ArgumentsNode)));
        QScopedPointer<EgcNode> exprPtr(expression);
        setNotDangling(expression);
        if (!node.isNull()) {
                node->setChild(0, *exprPtr.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcArgumentsNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

EgcArgumentsNode* Interpreter::addArgument(EgcNode* expressionToAdd, EgcArgumentsNode* argumentList)
{
        QScopedPointer<EgcNode> exprToAdd(expressionToAdd);
        setNotDangling(expressionToAdd);
        if (argumentList && expressionToAdd) {
                if (argumentList->insert(0, *exprToAdd.data()))
                        (void) exprToAdd.take();
        }

        return argumentList;
}

void Interpreter::addDanglingNode(EgcNode* node)
{
        m_danglingNodes.insert(node);
}

void Interpreter::setNotDangling(EgcNode* node)
{
        m_danglingNodes.remove(node);
}

void Interpreter::deleteDanglingNodes(void)
{
        foreach (EgcNode* node, m_danglingNodes) {
                delete node;
        }
        m_danglingNodes.clear();
}

EgcNode* Interpreter::addSqrtExpression(EgcNode* node0)
{
        QScopedPointer<EgcNumberNode> nExp(static_cast<EgcNumberNode*>(EgcNodeCreator::create(EgcNodeType::NumberNode)));
        nExp->setValue("2");
        QScopedPointer<EgcNode> sqrt(addBinaryExpression(EgcNodeType::RootNode, node0, nExp.take()));
        
        return sqrt.take();        
}

EgcNode* Interpreter::addUnaryStructParenth(EgcNode* node)
{
        return node;
}

EgcNode* Interpreter::addDivisionExpression(EgcNode* node0, EgcNode* node1)
{
        EgcNode* retval;

        if (!m_parseKernelResult) {
                retval = addBinaryExpression(EgcNodeType::DivisionNode, node0, node1);
        } else {
                retval = addBinaryExpression(EgcNodeType::DivisionNode, node0, node1);
                if (node0) {
                        if (node0->getNodeType() == EgcNodeType::ParenthesisNode) {
                                (void) removeParenthesisChild(*node0);
                        }
                }
                if (node1) {
                        if (node1->getNodeType() == EgcNodeType::ParenthesisNode) {
                                (void) removeParenthesisChild(*node1);
                        }
                }
        }

        return retval;
}

EgcNode* Interpreter::addEmptyNode(void)
{
        EgcNode *node = EgcNodeCreator::create(EgcNodeType::EmptyNode);
        if (node) {
                addDanglingNode(node);
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

EgcNode* Interpreter::changeFlexExpressionType(EgcNodeType type, EgcArgumentsNode* argList)
{
        QScopedPointer<EgcFncContainerNode> node(static_cast<EgcFncContainerNode*>(EgcNodeCreator::create(type)));
        assert(node->isFlexNode());
        if (node->isFlexNode()) {
                if (argList)
                        node->transferArgs(*argList);
                delete argList;
                setNotDangling(argList);
                EgcFlexNode *nodePtr = node.data();
                addDanglingNode(node.take());

                return nodePtr;
        } else {
                return argList;
        }
}

EgcNode* Interpreter::addDifferentialExpression(EgcArgumentsNode* argList)
{
        EgcNode* node = changeFlexExpressionType(EgcNodeType::DifferentialNode, argList);
        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(node);
        EgcNode* derivative = diff->getChild(2);
        if (derivative->getNodeType() == EgcNodeType::NumberNode)
                diff->setNrDerivative(static_cast<EgcNumberNode*>(derivative)->getValue().toUInt());
        diff->remove(2);

        return node;
}

EgcNode* Interpreter::getIteratorNode(int i)
{
        if (i == 1)
                return m_iterPointer1;
        if (i == 2)
                return m_iterPointer2;
        if (i == 3)
                return m_iterPointer3;

        return nullptr;
}
