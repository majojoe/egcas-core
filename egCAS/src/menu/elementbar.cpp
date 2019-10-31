/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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
#include "elementbar.h"
#include "menu/mathsection.h"
#include "view/egcasscene.h"
#include "../structural/actions/egcaction.h"
#include "matrixbox.h"


ElementBar::ElementBar()
{
        qRegisterMetaType<EgcAction>("EgcAction");
}

ElementBar::~ElementBar()
{

}

MathSection* ElementBar::getNewSection(QWidget* parent, QVBoxLayout* barLayout, QString title)
{
        MathSection *section = new (std::nothrow) MathSection(parent);
        if (section) {
                section->setText(title);
                section->setNrColumns(7);
                barLayout->addWidget(section);
        }

        return section;
}

void ElementBar::setupBar(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        setupCalcSection(parent, barLayout, scene);
        setupGreekSection(parent, barLayout, scene);
        setupAnalysisSection(parent, barLayout, scene);
        setupConstantSection(parent, barLayout, scene);
        setupMatrixSection(parent, barLayout, scene);
}

void ElementBar::interceptSignals(EgcAction action)
{
        MatrixBox matrixBox;
        MatrixDimension matrixDimension = matrixBox.getDimension();
        action.m_additionalData = QVariant::fromValue(matrixDimension);

        emit actionTriggered(action);
}

void ElementBar::setupCalcSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        MathSection* section = getNewSection(parent, barLayout, tr("Calc"));
        if (section) {
                section->setChecked();

                section->addElement(MathElement("=", EgcAction(EgcOperations::mathCharOperator, QChar('='))));
                section->addElement(MathElement(":=", EgcAction(EgcOperations::mathCharOperator, QChar(':'))));
                section->addElement(MathElement("+", EgcAction(EgcOperations::mathCharOperator, QChar('+'))));
                section->addElement(MathElement("-", EgcAction(EgcOperations::mathCharOperator, QChar('-'))));
                section->addElement(MathElement("/", EgcAction(EgcOperations::mathCharOperator, QChar('/'))));
                section->addElement(MathElement("*", EgcAction(EgcOperations::mathCharOperator, QChar('*'))));
                section->addElement(MathElement("^", EgcAction(EgcOperations::mathCharOperator, QChar('^'))));
                section->addElement(MathElement("±", EgcAction(EgcOperations::mathCharOperator, QChar(177))));
                section->addElement(MathElement("√", EgcAction(EgcOperations::mathCharOperator, QChar(8730))));
                section->addElement(MathElement("ln", EgcAction(EgcOperations::internalFunction, InternalFunctionType::natLogarithm)));
                section->addElement(MathElement("log", EgcAction(EgcOperations::internalFunction, InternalFunctionType::logarithm)));
                section->addElement(MathElement("sin", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("sin"))));
                section->addElement(MathElement("cos", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("cos"))));
                section->addElement(MathElement("tan", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("tan"))));
                section->addElement(MathElement("(", EgcAction(EgcOperations::mathCharOperator, QChar('('))));
                section->addElement(MathElement(")", EgcAction(EgcOperations::mathCharOperator, QChar(')'))));
                section->addElement(MathElement("f(x)", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString(""))));
                section->addElement(MathElement(",", EgcAction(EgcOperations::mathCharOperator, QChar(','))));
                section->addElement(MathElement("asin", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("asin"))));
                section->addElement(MathElement("acos", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("acos"))));
                section->addElement(MathElement("atan", EgcAction(EgcOperations::mathFunction, OpModificators::standard, LookModificators::standard, QString("atan"))));
        }

        bool ass_ret;
        ass_ret = connect(section, &MathSection::actionTriggered, scene, &EgCasScene::routeAction);
        Q_ASSERT(ass_ret == true);
}

void ElementBar::setupGreekSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        MathSection* section = getNewSection(parent, barLayout, tr("Greek"));
        if (section) {
                section->setChecked();

                section->addElement(MathElement("Α", EgcAction(EgcOperations::alnumKeyPressed, QChar(913))));
                section->addElement(MathElement("Β", EgcAction(EgcOperations::alnumKeyPressed, QChar(914))));
                section->addElement(MathElement("Γ", EgcAction(EgcOperations::alnumKeyPressed, QChar(915))));
                section->addElement(MathElement("Δ", EgcAction(EgcOperations::alnumKeyPressed, QChar(916))));
                section->addElement(MathElement("Ε", EgcAction(EgcOperations::alnumKeyPressed, QChar(917))));
                section->addElement(MathElement("Ζ", EgcAction(EgcOperations::alnumKeyPressed, QChar(918))));
                section->addElement(MathElement("Η", EgcAction(EgcOperations::alnumKeyPressed, QChar(919))));
                section->addElement(MathElement("Θ", EgcAction(EgcOperations::alnumKeyPressed, QChar(920))));
                section->addElement(MathElement("Ι", EgcAction(EgcOperations::alnumKeyPressed, QChar(921))));
                section->addElement(MathElement("Κ", EgcAction(EgcOperations::alnumKeyPressed, QChar(922))));
                section->addElement(MathElement("Λ", EgcAction(EgcOperations::alnumKeyPressed, QChar(923))));
                section->addElement(MathElement("Μ", EgcAction(EgcOperations::alnumKeyPressed, QChar(924))));
                section->addElement(MathElement("Ν", EgcAction(EgcOperations::alnumKeyPressed, QChar(925))));
                section->addElement(MathElement("Ξ", EgcAction(EgcOperations::alnumKeyPressed, QChar(926))));
                section->addElement(MathElement("Ο", EgcAction(EgcOperations::alnumKeyPressed, QChar(927))));
                section->addElement(MathElement("Π", EgcAction(EgcOperations::alnumKeyPressed, QChar(928))));
                section->addElement(MathElement("Ρ", EgcAction(EgcOperations::alnumKeyPressed, QChar(929))));
                section->addElement(MathElement("Σ", EgcAction(EgcOperations::alnumKeyPressed, QChar(931))));
                section->addElement(MathElement("Τ", EgcAction(EgcOperations::alnumKeyPressed, QChar(932))));
                section->addElement(MathElement("Υ", EgcAction(EgcOperations::alnumKeyPressed, QChar(933))));
                section->addElement(MathElement("Φ", EgcAction(EgcOperations::alnumKeyPressed, QChar(934))));
                section->addElement(MathElement("Χ", EgcAction(EgcOperations::alnumKeyPressed, QChar(935))));
                section->addElement(MathElement("Ψ", EgcAction(EgcOperations::alnumKeyPressed, QChar(936))));
                section->addElement(MathElement("Ω", EgcAction(EgcOperations::alnumKeyPressed, QChar(937))));

                section->newRow();

                section->addElement(MathElement("α", EgcAction(EgcOperations::alnumKeyPressed, QChar(945))));
                section->addElement(MathElement("β", EgcAction(EgcOperations::alnumKeyPressed, QChar(946))));
                section->addElement(MathElement("γ", EgcAction(EgcOperations::alnumKeyPressed, QChar(947))));
                section->addElement(MathElement("δ", EgcAction(EgcOperations::alnumKeyPressed, QChar(948))));
                section->addElement(MathElement("ε", EgcAction(EgcOperations::alnumKeyPressed, QChar(949))));
                section->addElement(MathElement("ζ", EgcAction(EgcOperations::alnumKeyPressed, QChar(950))));
                section->addElement(MathElement("η", EgcAction(EgcOperations::alnumKeyPressed, QChar(951))));
                section->addElement(MathElement("θ", EgcAction(EgcOperations::alnumKeyPressed, QChar(952))));
                section->addElement(MathElement("ι", EgcAction(EgcOperations::alnumKeyPressed, QChar(953))));
                section->addElement(MathElement("κ", EgcAction(EgcOperations::alnumKeyPressed, QChar(954))));
                section->addElement(MathElement("λ", EgcAction(EgcOperations::alnumKeyPressed, QChar(955))));
                section->addElement(MathElement("μ", EgcAction(EgcOperations::alnumKeyPressed, QChar(956))));
                section->addElement(MathElement("ν", EgcAction(EgcOperations::alnumKeyPressed, QChar(957))));
                section->addElement(MathElement("ξ", EgcAction(EgcOperations::alnumKeyPressed, QChar(958))));
                section->addElement(MathElement("ο", EgcAction(EgcOperations::alnumKeyPressed, QChar(959))));
                section->addElement(MathElement("π", EgcAction(EgcOperations::alnumKeyPressed, QChar(960))));
                section->addElement(MathElement("ρ", EgcAction(EgcOperations::alnumKeyPressed, QChar(961))));
                section->addElement(MathElement("ς", EgcAction(EgcOperations::alnumKeyPressed, QChar(962))));
                section->addElement(MathElement("σ", EgcAction(EgcOperations::alnumKeyPressed, QChar(963))));
                section->addElement(MathElement("τ", EgcAction(EgcOperations::alnumKeyPressed, QChar(964))));
                section->addElement(MathElement("υ", EgcAction(EgcOperations::alnumKeyPressed, QChar(965))));
                section->addElement(MathElement("φ", EgcAction(EgcOperations::alnumKeyPressed, QChar(966))));
                section->addElement(MathElement("χ", EgcAction(EgcOperations::alnumKeyPressed, QChar(967))));
                section->addElement(MathElement("ψ", EgcAction(EgcOperations::alnumKeyPressed, QChar(968))));
                section->addElement(MathElement("ω", EgcAction(EgcOperations::alnumKeyPressed, QChar(969))));
        }

        bool ass_ret;
        ass_ret = connect(section, &MathSection::actionTriggered, scene, &EgCasScene::routeAction);
        Q_ASSERT(ass_ret == true);
}

void ElementBar::setupAnalysisSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        MathSection* section = getNewSection(parent, barLayout, tr("Analysis"));
        if (section) {
                section->setChecked();
                section->addElement(MathElement(":/res/fnc/integral.png",
                                                EgcAction(EgcOperations::internalFunction, InternalFunctionType::integral, OpModificators::standard),
                                                true));
                section->addElement(MathElement(":/res/fnc/integral_definite.png",
                                                EgcAction(EgcOperations::internalFunction, InternalFunctionType::integral,
                                                          OpModificators::definiteIntegral), true));
                section->addElement(MathElement(":/res/fnc/differential_lagrange_1.png",
                                                EgcAction(EgcOperations::internalFunction, InternalFunctionType::differential,
                                                          OpModificators::standard, LookModificators::differential_lagrange_notation_1), true));
                section->newRow();
                section->addElement(MathElement(":/res/fnc/differential_lagrange_2.png",
                                                EgcAction(EgcOperations::internalFunction, InternalFunctionType::differential,
                                                          OpModificators::standard, LookModificators::differential_lagrange_notation_2), true));
                section->addElement(MathElement(":/res/fnc/differential_lagrange_3.png",
                                                EgcAction(EgcOperations::internalFunction, InternalFunctionType::differential,
                                                          OpModificators::standard, LookModificators::differential_lagrange_notation_3), true));
                section->addElement(MathElement(":/res/fnc/differential_leibnitz.png", EgcAction(EgcOperations::internalFunction,
                                                                          InternalFunctionType::differential), true));
                section->newRow();

        }

        //Analysis
        //Matrix
        //Graphics
        //Symbolic
        //Logic
        //Common

        bool ass_ret;
        ass_ret = connect(section, &MathSection::actionTriggered, scene, &EgCasScene::routeAction);
        Q_ASSERT(ass_ret == true);
}

void ElementBar::setupConstantSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        MathSection* section = getNewSection(parent, barLayout, tr("Constants"));
        if (section) {
                section->setChecked();

                section->addElement(MathElement("π", EgcAction(EgcOperations::constant, Constants::pi), false));
                section->addElement(MathElement("e", EgcAction(EgcOperations::constant, Constants::e), false));
                section->addElement(MathElement("∞", EgcAction(EgcOperations::constant, Constants::infinity), false));
                section->addElement(MathElement("i", EgcAction(EgcOperations::constant, Constants::i), false));
        }

        bool ass_ret;
        ass_ret = connect(section, &MathSection::actionTriggered, scene, &EgCasScene::routeAction);
        Q_ASSERT(ass_ret == true);
}

void ElementBar::setupMatrixSection(QWidget* parent, QVBoxLayout* barLayout, EgCasScene* scene)
{
        MathSection* section = getNewSection(parent, barLayout, tr("Vector/Matrix"));
        if (section) {
                section->setChecked();

                section->addElement(MathElement("M", EgcAction(EgcOperations::insertMatrix,  0, 0), false));
        }

        bool ass_ret;
        ass_ret = connect(section, &MathSection::actionTriggered, this, &ElementBar::interceptSignals);
        Q_ASSERT(ass_ret == true);
        ass_ret = connect(this, &ElementBar::actionTriggered, scene, &EgCasScene::routeAction);
        Q_ASSERT(ass_ret == true);
}
