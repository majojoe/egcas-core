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
#include <boost/concept_check.hpp>
#include <QVector>
#include "interpreter.h"
#include "../../structural/egcnodecreator.h"
#include "../../structural/egcnodes.h"

using namespace CASParser;

Interpreter::Interpreter() :
        m_scanner(*this),
        m_parser(m_scanner, *this),
        m_location(0)
{

}

Interpreter::~Interpreter()
{
        deleteDanglingNodes();
}

int Interpreter::parse() {
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
        EgcBinaryNode *node = static_cast<EgcBinaryNode*>(EgcNodeCreator::create(type));
        if (node) {
                node->setChild(0, *node0);
                node->setChild(1, *node1);
                setNotDangling(node0);
                setNotDangling(node1);
        } else {
                delete node0;
                delete node1;
                setNotDangling(node0);
                setNotDangling(node1);
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        addDanglingNode(node);

        return node;
}

EgcNode* Interpreter::addUnaryExpression(EgcNodeType type, EgcNode* node0)
{
        EgcUnaryNode *node = static_cast<EgcUnaryNode*>(EgcNodeCreator::create(type));
        if (node) {
                node->setChild(0, *node0);
                setNotDangling(node0);
        } else {
                delete node0;
                setNotDangling(node0);
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        addDanglingNode(node);

        return node;
}

EgcNode* Interpreter::addStringNode(EgcNodeType type, const std::string& value)
{
        EgcNode *node = static_cast<EgcNode*>(EgcNodeCreator::create(type));
        if (node) {
                switch (type) {
                case EgcNodeType::NumberNode: {
                        EgcNumberNode* tmp = static_cast<EgcNumberNode*>(node);
                        tmp->setValue(QString::fromStdString(value));
                        break;
                }
                case EgcNodeType::VariableNode: {
                        EgcVariableNode* tmp = static_cast<EgcVariableNode*>(node);
                        tmp->setValueRaw(QString::fromStdString(value));
                        break;
                }
                default: /*do nothing*/
                        break;
                }
                addDanglingNode(node);
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
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


EgcNode* Interpreter::addFunction(const std::string& fncName, EgcNode* argList)
{
#warning implement this function
        setNotDangling(argList);
}

EgcNode* Interpreter::addBuiltinFunction(const std::string& fncName, EgcNode* argList)
{
#warning implement this function
        setNotDangling(argList);
}

EgcNode* Interpreter::createArgList(EgcNode* expression)
{
#warning implement this function
        setNotDangling(expression);
}

EgcNode* Interpreter::addArgument(EgcNode* expressionToAdd, EgcNode* argumentList)
{
#warning implement this function
        setNotDangling(expressionToAdd);
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

