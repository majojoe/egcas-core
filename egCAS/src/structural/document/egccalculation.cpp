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

#include <QTimer>
#include "egccalculation.h"
#include "casKernel/egckernelconn.h"
#include "casKernel/egcmaximaconn.h"
#include "entities/egcentity.h"
#include "entities/egcformulaentity.h"
#include "egcnodes.h"

EgcCalculation::EgcCalculation(QObject *parent) : QObject{parent}, m_conn{new EgcMaximaConn()}, m_iterator{nullptr},
        m_kernelStarted{false}, m_computeWhenStarted{false}, m_updateInstantly{true}, m_waitForResult{nullptr}
{
        
        connect(m_conn.data(), SIGNAL(resultReceived(QString)), this, SLOT(resultReceived(QString)));
        connect(m_conn.data(), SIGNAL(errorReceived(QString)), this, SLOT(errorReceived(QString)));
        connect(m_conn.data(), SIGNAL(kernelStarted(void)), this, SLOT(kernelStarted(void)));
        connect(m_conn.data(), SIGNAL(kernelTerminated(void)), this, SLOT(kernelTerminated(void)));
        connect(m_conn.data(), SIGNAL(kernelErrorOccurred(QProcess::ProcessError)), this, 
                SLOT(kernelErrorOccurred(QProcess::ProcessError)));

}

void EgcCalculation::calculate(EgcEntityList& list, bool updateInstantly)
{
        m_iterator.reset(new QListIterator<EgcEntity*>(list.getIterator()));

        m_updateInstantly = updateInstantly;
        if (!m_kernelStarted) {
                m_computeWhenStarted = true;
        } else {
                nextCalculation();
        }

}

void EgcCalculation::nextCalculation(void)
{
        EgcEntity* entity = nullptr;
        //m_iterator = list.getIterator();
        if (m_iterator->hasNext()) {
                entity = m_iterator->next();
                if (entity) {
                        if (entity->getEntityType() == EgcEntityType::Formula)
                                handleCalculation(static_cast<EgcFormulaEntity&>(*entity));
                }
        } else {
#warning what to do if list is at the end? Do we need anything to do?
        }
}

void EgcCalculation::handleCalculation(EgcFormulaEntity& entity)
{
        m_waitForResult = nullptr;
        EgcNode* node = entity.getRootElement();
        if (!node) { //process next formula -> prevent recursion with event
                QTimer::singleShot(0, this, SLOT(nextCalculation()));
                return;
        }

        EgcNodeType type = node->getNodeType();
        switch(type) {
        //send the formula to the cas kernel if it's a definition
        case EgcNodeType::DefinitionNode:
                m_conn->sendCommand(entity.getCASKernelCommand());
                break;

        case EgcNodeType::EqualNode:
                entity.resetResult();
                m_waitForResult = &entity;
                m_conn->sendCommand(entity.getCASKernelCommand());
                break;
        default:
                QTimer::singleShot(0, this, SLOT(nextCalculation()));
                break;
        }
}

void EgcCalculation::resultReceived(QString result)
{
        if (m_waitForResult) {
                EgcNode* tree = m_parser.parseKernelOutput(result);
                if (tree)
                        m_waitForResult->setResult(tree);
        }
}

void EgcCalculation::errorReceived(QString errorMsg)
{
        
}

void EgcCalculation::kernelStarted(void)
{
        m_kernelStarted = true;
        if (m_computeWhenStarted) {
                m_computeWhenStarted = false;
                nextCalculation();
        }
}

void EgcCalculation::kernelTerminated(void)
{
        
}

void EgcCalculation::kernelErrorOccurred(QProcess::ProcessError error)
{
        
}
