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
#include <iostream>
#include <QScopedPointer>
#include "parser/egckernelparser.h"
#include "entities/egcformulaentity.h"
#include "formulagenerator.h"
#include "utils/egcutfcodepoint.h"

FormulaGenerator::FormulaGenerator()
{
}


bool FormulaGenerator::getFormulaTree(EgcFormulaEntity* formula, QString formulaText)
{
        QString tmp = EgcUtfCodepoint::encodeToXml(formulaText);

        // ampersands and followning '#' in special symbols are replaced by "_2" for use in calculation kernel
        tmp = tmp.replace("&#", "_2");
        // ";" in special symbols are replaced by "_3" for use in calculation kernel
        tmp = tmp.replace(";", "_3");

        EgcKernelParser parser;

        QScopedPointer<EgcNode> tree(parser.parseKernelOutput(tmp));
        if (tree.isNull()) {
                std::cout << parser.getErrorMessage().toStdString();
                return false;
        }

        formula->setRootElement(tree.take());
        formula->updateView();

        return true;
}

