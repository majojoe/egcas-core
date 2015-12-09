/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#ifndef EGCMATHMLVISITOR_H
#define EGCMATHMLVISITOR_H

#include <QString>
#include <QSet>
#include "egcnodevisitor.h"
#include "egcmathmllookup.h"

/**
 * @brief The EgcMathMlVisitor class is a visitor class for parsing the tree and output expressions formatted for the
 * used cas kernel.
 */

class EgcMathMlVisitor : public EgcNodeVisitor
{
public:
        /**
         * @brief EgcNodeVisitor std constructor for the visitor
         * @param formula the formula to be parsed
         */
        EgcMathMlVisitor(EgcFormulaEntity& formula);
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcBinaryNode* binary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcUnaryNode* unary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param flex the node with the information to be extracted.
         */
        virtual void visit(EgcFlexNode* flex) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcNode* node) override;
        /**
         * @brief getResult returns the result of the traversion of the tree
         * @return the result of the traversion as string
         */
        virtual QString getResult(void) override;
        /**
         * @brief setPrettyPrint sets the pretty print mode. E.g. in case of fractions/Divisions any additional
         * parenthesis will be ommitted if pretty print is activated
         * @param prettyPrint if true (default), pretty print is activated, false otherwise
         */
        void setPrettyPrint(bool prettyPrint);
        /**
         * @brief prettyPrint returns wether pretty print is activated or not
         * @return true if pretty print is activated, false otherwise
         */
        bool prettyPrint(void);
        /**
         * @brief suppressIfChildType suppress rendering of child object if child has given type 
         * @param node the current node to render (can be binary, unary or flex node)
         * @param index the index of the child to test and suppress
         * @param type the type of the child to test and suppress
         */
        void suppressIfChildType(const EgcNode* node, quint32 index, EgcNodeType type);
        /**
         * @brief suppressIfChildValue suppress rendering of child object if child has given type and given value
         * @param node the current node to render (can be binary, unary or flex node)
         * @param index the index of the child to test and suppress
         * @param type the type of the child to test and suppress
         * @param val the value of the child to test and supppress
         */
        void suppressIfChildValue(const EgcNode* node, quint32 index, EgcNodeType type, QString val);
        /**
         * @brief getId returns the id string to be added to the mathml code to be able to identify the node later
         * @param node the node to be identified via the id
         * @return the mathml string with the id
         */
        QString getId(EgcNode* node);

private:
        bool m_prettyPrint;             ///< activates pretty printing e.g. in case of a fraction remove the parenthesis
        quint32 m_idCounter;            ///< the id counter
        EgcMathmlLookup& m_lookup;       ///< lookup for mapping id's in node pointers
};

#endif // EGCMATHMLVISITOR_H
