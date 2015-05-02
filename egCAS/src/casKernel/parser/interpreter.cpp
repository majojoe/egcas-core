/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
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
#include "../../structural/egcexpressionnodecreator.h"
#include "../../structural/egcnodes.h"

using namespace CASParser;

Interpreter::Interpreter() :
        m_scanner(*this),
        m_baseNode(nullptr),
        m_parser(m_scanner, *this),
        m_location(0)
{

}

Interpreter::~Interpreter()
{
        delete m_baseNode;
        m_baseNode = nullptr;
}

int Interpreter::parse() {
        m_location = 0;
        return m_parser.parse();
}

void Interpreter::clear() {
        m_location = 0;
        delete m_baseNode;
        m_baseNode = nullptr;
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

EgcExpressionNode* Interpreter::addBinaryExpression(EgcExpressionNodeType type, EgcExpressionNode* node0,
                                                    EgcExpressionNode* node1)
{
        EgcBinaryExpressionNode *node = static_cast<EgcBinaryExpressionNode*>(EgcExpressionNodeCreator::create(type));
        if (node) {
                node->setLeftChild(*node0);
                node->setRightChild(*node1);
        } else {
#warning improve error handling
                delete node0;
                delete node1;
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

EgcExpressionNode* Interpreter::addUnaryExpression(EgcExpressionNodeType type, EgcExpressionNode* node0)
{
        EgcUnaryExpressionNode *node = static_cast<EgcUnaryExpressionNode*>(EgcExpressionNodeCreator::create(type));
        if (node) {
                node->setChild(*node0);
        } else {
#warning improve error handling
                delete node0;
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

EgcExpressionNode* Interpreter::addStringNode(EgcExpressionNodeType type, const std::string& value)
{
        EgcExpressionNode *node = static_cast<EgcExpressionNode*>(EgcExpressionNodeCreator::create(type));
        if (node) {
                switch (type) {
                case EgcExpressionNodeType::NumberNode: {
                        EgcNumberExpressionNode* tmp = static_cast<EgcNumberExpressionNode*>(node);
                        tmp->setValue(QString::fromStdString(value));
                        break;
                }
                case EgcExpressionNodeType::VariableNode: {
                        EgcVariableExpressionNode* tmp = static_cast<EgcVariableExpressionNode*>(node);
                        tmp->setValueRaw(QString::fromStdString(value));
                        break;
                }
                default: /*do nothing*/
                        break;
                }
        } else {
#warning improve error handling
                delete node;
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

void Interpreter::createBaseNode(EgcExpressionNode* node)
{
        m_baseNode = static_cast<EgcBaseExpressionNode*>(EgcExpressionNodeCreator::create(EgcExpressionNodeType::BaseNode));
        if (m_baseNode) {
                m_baseNode->setChild(*node);
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
}

EgcBaseExpressionNode* Interpreter::getBaseNode(void)
{
        EgcBaseExpressionNode *baseNode = m_baseNode;
        m_baseNode = nullptr;
        return baseNode;
}


EgcExpressionNode* Interpreter::addFunction(const std::string& fncName, EgcExpressionNode* argList)
{
#warning implement this function
}

EgcExpressionNode* Interpreter::addBuiltinFunction(const std::string& fncName, EgcExpressionNode* argList)
{
#warning implement this function
}

EgcExpressionNode* Interpreter::createArgList(EgcExpressionNode* expression)
{
#warning implement this function
}

EgcExpressionNode* Interpreter::addArgument(EgcExpressionNode* expressionToAdd, EgcExpressionNode* argumentList)
{
#warning implement this function
}
