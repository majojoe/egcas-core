/*
Copyright (c) 2014 - 2018, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
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
#ifndef VISITORHELPER_H
#define VISITORHELPER_H

#include "egcnodevisitor.h"
#include <QString>
#include <QStack>
#include <QSet>
#include <QRegularExpression>
//#include "../iterator/egcnodeiterator.h"

class VisitorHelper : public EgcNodeVisitor
{
public:
        VisitorHelper(EgcFormulaEntity& formula);
        virtual ~VisitorHelper();
protected:
        /**
         * @brief assembleResult assemble the result string of a node
         * @param formatString the format string that contains placeholders for result strings of the childs like
         * QString does. E.g. "<mrow>%1<mo>&CenterDot;</mo>%2</mrow>" is an example for a mathml multiplication
         * @param node the node we are currently operating on
         */
        virtual void assembleResult(QString formatString, EgcNode* node);
        /**
         * @brief assembleResult assemble the result string of a node. This is a convinience version of the above
         * function. The function can be used in cases when there are many (variable) arguments, e.g. for functions.
         * E.g. it can be used with the following mathml function:
         * assembleResult("<mrow><mi>FNC_NAME</mi><mo>&ApplyFunction;</mo><mrow><mo>(</mo><mrow>", "<mo>,</mo>",
         * "</mrow><mo>)</mo></mrow></mrow>", node);
         * @param startString the start string of a node (if any - give an empty string if this is not used)
         * @param seperationString the separation string that is used as seperation of each child node
         * @param endString the end string of the node
         * @param node the node we are currently operating on
         */
        virtual void assembleResult(QString startString, QString seperationString, QString endString, EgcNode* node);
        /**
         * @brief assembleResult assemble the result string of a node. This is a version e.g. for functions where the
         * first node contains the name of the function. The left part of the start string would be given as first
         * argument, the right part of the start string as second argument.
         * E.g. it can be used with the following mathml function:
         * assembleResult("<mrow><mi>FNC_NAME</mi><mo>&ApplyFunction;</mo><mrow><mo>(</mo><mrow>", "<mo>,</mo>",
         * "</mrow><mo>)</mo></mrow></mrow>", node);
         * @param lStartString the left part of the start string of a node
         * @param rStartString the right part of the start string of a node (e.g. part starting with
         * </mi><mo>&ApplyFunction;) in above example
         * @param seperationString the separation string that is used as seperation of each child node
         * @param endString the end string of the node
         * @param node the node we are currently operating on
         */
        virtual void assembleResult(QString lStartString, QString rStartString, QString seperationString,
                                            QString endString, EgcNode* node);

        /**
         * @brief pushToStack push results to the stack. This function (and not the QStack push function) must be used
         * to push stuff onto the stack, since here also suppression conditions will be checked.
         * @param str the string to push
         * @param node the node we are currently operating on
         */
        virtual void pushToStack(QString str, EgcNode* node);
        /**
         * @brief getChildToSuppress returns a pointer to the child to suppress
         * @param node a pointer to the parent node we want the child to supress
         * @param index the index at which to supress the child
         * @return a pointer to the child to supress, or a nullpointer if there is no child to supress
         */
        virtual EgcNode* getChildToSuppress(const EgcNode* node, quint32 index);
        /**
         * @brief deleteFromStack delete stack objects from stack. This can make sense when a parent has full access to
         * its child values
         * @param nrStackObjects the number of stack objects to remove from stack
         */
        virtual void deleteFromStack(quint32 nrStackObjects);
        /**
         * @brief getAssembleArguments get the arguments for the node to assemble the result
         * @param node the node for witch to find the arguments to assemble
         * @return a vector with all the arguments
         */
        virtual QVector<QString> getAssembleArguments(EgcNode* node);
        /**
         * @brief modifyNodeString this function can be implemented in a subclass and modify the string assembled,
         * before it is further processed
         * @param nodeString the nodeString to be modified
         * @param node the node associated with the nodeString to modify
         * @return returns the modified string
         */
        virtual QString& modifyNodeString(QString &nodeString, EgcNode* node);

        static QRegularExpression s_argRegex;   ///< regex that searches for argument placeholders
        static bool s_regexInitalized;          ///< initialize regex?
        QSet<EgcNode*> m_suppressList;  ///< a list with pointers EgcNode elements that shall not be rendered

};

#endif // VISITORHELPER_H
