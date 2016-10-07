/*Copyright (c) 2016, Johannes Maier <maier_jo@gmx.de>
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

#include <QString>
#include <QtTest>
#include <iostream>
#include "parser/egckernelparser.h"
#include "egcnodes.h"
#include "iterator/egcnodeiterator.h"
#include "entities/egcformulaentity.h"
#include "egcnodecreator.h"
#include "visitor/egcnodevisitor.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"
#include "actions/egcaction.h"
#include "iterator/egcscrpositerator.h"
#include "view/egcformulaitem.h"


using namespace std;

class EgcasRearrangePrecedence : public QObject
{
        Q_OBJECT

public:
        EgcasRearrangePrecedence() {}
private:
        void removeParenthesis(EgcFormulaEntity* formula);
private Q_SLOTS:
        void rearrange_rot_right();
        void rearrange_rot_left();
};

void EgcasRearrangePrecedence::removeParenthesis(EgcFormulaEntity* formula)
{
        bool parenthesisFound = false;

        do {
                EgcNodeIterator iter(*formula);
                parenthesisFound = iter.findNext(EgcNodeType::ParenthesisNode);
                if (parenthesisFound) {
                        EgcNode* child = &iter.next();
                        EgcNode* parent = child->getParent();
                        formula->cut(*child);
                        formula->paste(*child, *parent );
                }
        } while(parenthesisFound);
}

void EgcasRearrangePrecedence::rearrange_rot_right()
{                
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("(x+5*4)^3"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcFormulaEntity formula(*tree.take());

        EgcFormulaEntity formula3 = formula;
        formula3.rearrangePrecedence();
        QVERIFY(formula == formula3);

        EgcNodeIterator iter(formula);

        QVERIFY(iter.findNext(EgcNodeType::ParenthesisNode) == true);
        EgcNode* child = formula.cut(iter.next());
        formula.paste(*child, *static_cast<EgcContainerNode*>(formula.getRootElement())->getChild(0) );

        formula.rearrangePrecedence();

        // generate a second tree without parenthesis. This must be the same
        tree.reset(parser.parseKernelOutput("x+5*4^3"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcFormulaEntity formula2(*tree.take());

        removeParenthesis(&formula);
        removeParenthesis(&formula2);
        formula.rearrangePrecedence();

        QVERIFY(formula == formula2);
}

void EgcasRearrangePrecedence::rearrange_rot_left()
{
        EgcKernelParser parser;
        QScopedPointer<EgcNode> tree;
        tree.reset(parser.parseKernelOutput("(3^4)^2"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcFormulaEntity formula(*tree.take());
        EgcFormulaEntity formula3 = formula;
        formula3.rearrangePrecedence();
        QVERIFY(formula == formula3);

        EgcNodeIterator iter(formula);

        QVERIFY(iter.findNext(EgcNodeType::ParenthesisNode) == true);
        EgcNode* child = formula.cut(iter.next());
        formula.paste(*child, *static_cast<EgcContainerNode*>(formula.getRootElement())->getChild(0) );

        formula.rearrangePrecedence();

        // generate a second tree without parenthesis. This must be the same
        tree.reset(parser.parseKernelOutput("3^4^2"));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
        }
        QVERIFY(!tree.isNull());

        EgcFormulaEntity formula2(*tree.take());

        removeParenthesis(&formula);
        removeParenthesis(&formula2);
        formula.rearrangePrecedence();

        QVERIFY(formula == formula2);
}

QTEST_MAIN(EgcasRearrangePrecedence)

#include "tst_rearrange_precedence.moc"
