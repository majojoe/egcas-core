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
#include "casKernel/parser/egckernelparser.h"

EgcCalculation::EgcCalculation(QObject *parent) : QObject{parent}, m_conn{new EgcMaximaConn()}, m_iterator{nullptr},
        m_kernelStarted{false}, m_computeWhenStarted{false}, m_updateInstantly{true}, m_parser{new EgcKernelParser()},
        m_result{nullptr}, m_calculationRunning{false}, m_entity{nullptr}, m_paused{false}, m_autoCalc{true},
        m_waitForResult{false}, m_restartAfterResume{false}
{
        
        connect(m_conn.data(), SIGNAL(resultReceived(QString)), this, SLOT(resultReceived(QString)));
        connect(m_conn.data(), SIGNAL(errorReceived(QString)), this, SLOT(errorReceived(QString)));
        connect(m_conn.data(), SIGNAL(kernelStarted(void)), this, SLOT(kernelStarted(void)));
        connect(m_conn.data(), SIGNAL(kernelTerminated(void)), this, SLOT(kernelTerminated(void)));
        connect(m_conn.data(), SIGNAL(kernelErrorOccurred(QProcess::ProcessError)), this, 
                SLOT(kernelErrorOccurred(QProcess::ProcessError)));
        connect(m_conn.data(), SIGNAL(timeoutError(void)), this, SLOT(handleTimeout()));
}

EgcCalculation::~EgcCalculation()
{
}

bool EgcCalculation::calculate(EgcEntityList& list, bool updateInstantly, EgcAbstractFormulaEntity* entity)
{
        m_paused = false;
        m_entity = entity;
        if (m_calculationRunning)
                return false;
        if (!m_autoCalc && entity) // only if auto calculation is active
                return false;

        m_conn->reset();
                
        m_iterator.reset(new QMutableListIterator<EgcEntity*>(list.getIterator()));

        m_updateInstantly = updateInstantly;
        if (!m_kernelStarted) {
                m_computeWhenStarted = true;
        } else {
                nextCalculation();
        }
        
        return true;
}

bool EgcCalculation::restart(void)
{
        m_paused = false;
        m_restartAfterResume = false;
        m_iterator->toFront();
        m_entity = nullptr;
        if (m_calculationRunning)
                return false;
        if (!m_autoCalc && m_entity) // only if auto calculation is active
                return false;

        m_conn->reset();

        nextCalculation();

        return true;
}

void EgcCalculation::nextCalculation(void)
{
        if (!m_calculationRunning)
                m_calculationRunning = true;
        EgcEntity* entity = nullptr;

        if (!m_iterator)
                return;
        
        if (m_iterator->hasNext()) {
                if (    (m_iterator->peekNext() != dynamic_cast<EgcEntity*>(m_entity))
                     || m_paused) {
                        m_paused = false;
                        entity = m_iterator->next();
                        if (entity) {
                                if (entity->getEntityType() == EgcEntityType::Formula)
                                        handleCalculation(static_cast<EgcFormulaEntity&>(*entity));
                                else
                                        triggerNextCalcualtion();
                        } else {
                                triggerNextCalcualtion();
                        }
                } else {
                        m_paused = true;
                }
        } else {
                m_calculationRunning = false;
                m_entity = nullptr;
        }
}

void EgcCalculation::resumeCalculation(void)
{
        if (m_restartAfterResume) {
                restart();
                return;
        }

        if (m_paused)
                triggerNextCalcualtion();
}

void EgcCalculation::handleCalculation(EgcFormulaEntity& entity)
{
        m_result = nullptr;
        EgcNode* node = entity.getRootElement();
        if (!node) { //process next formula -> prevent recursion with event
                triggerNextCalcualtion();
                return;
        }

        EgcNodeType type = node->getNodeType();
        switch(type) {
        //send the formula to the cas kernel if it's a definition
        case EgcNodeType::DefinitionNode:
                m_waitForResult = true;
                m_conn->sendCommand(entity.getCASKernelCommand());
                break;

        case EgcNodeType::EqualNode:
                entity.resetResult();
                m_waitForResult = true;
                m_result = &entity;
                m_conn->sendCommand(entity.getCASKernelCommand());
                break;
        default:
                triggerNextCalcualtion();
                break;
        }
}

void EgcCalculation::triggerNextCalcualtion(void)
{
        if (!m_waitForResult)
                QTimer::singleShot(0, this, SLOT(nextCalculation()));
}

void EgcCalculation::resultReceived(QString result)
{
        m_waitForResult = false;
        if (m_result) {
                EgcNode* tree = m_parser->parseKernelOutput(result);
                if (tree) {
                        m_result->setResult(tree);
                } else {
                        m_result->setErrorMessage(m_parser->getErrorMessage());
                }
                if (m_updateInstantly)
                        m_result->updateView();
        }
        
        //go on to next calculation
        nextCalculation();
}

void EgcCalculation::errorReceived(QString errorMsg)
{
        m_calculationRunning = false;
        m_waitForResult = false;
        
        if (m_result) {
                m_result->setErrorMessage(errorMsg);
                if (m_updateInstantly)
                        m_result->updateView();
        }

        //go on to next calculation (even after an error with the current calculation)
        if (m_waitForResult) {
                nextCalculation();
        } else if (m_kernelStarted) {
                //if an error occurred where it makes no sense to go on with calculation, wait for the user to change
                //s.th.
                m_restartAfterResume = true;
        }
}

void EgcCalculation::kernelStarted(void)
{
        m_kernelStarted = true;
        m_waitForResult = false;
        if (m_computeWhenStarted) {
                m_computeWhenStarted = false;
                nextCalculation();
        }
}

void EgcCalculation::kernelTerminated(void)
{
        m_calculationRunning = false;
        m_waitForResult = false;
        
        emit errorOccurred(EgcKernelErrorType::kernelTerminated, tr("The CAS Kernel has terminated!"));
}

void EgcCalculation::kernelErrorOccurred(QProcess::ProcessError error)
{
        m_calculationRunning = false;
        m_waitForResult = false;

        switch(error) {
        case QProcess::Crashed:
                emit errorOccurred(EgcKernelErrorType::kernelTerminated, tr("The CAS Kernel has terminated!"));
                break;
        case QProcess::FailedToStart:
                emit errorOccurred(EgcKernelErrorType::kernelNotFound, tr("The CAS Kernel failed to start, no kernel found!"));
                break;
        case QProcess::Timedout:
                emit errorOccurred(EgcKernelErrorType::timeout, tr("The CAS Kernel has timed out!"));
                break;
        case QProcess::ReadError:
        case QProcess::WriteError:
                emit errorOccurred(EgcKernelErrorType::rdWrError, tr("A Read/Write error encountered in the CAS kernel!"));
                break;
        default:
                emit errorOccurred(EgcKernelErrorType::unknown, tr("An unknown error encountered in the CAS kernel!"));
                break;
        }
}

void EgcCalculation::handleTimeout(void)
{
        m_waitForResult = false;

        emit errorOccurred(EgcKernelErrorType::timeout, tr("The CAS kernel did not respond within 30s."));
}

void EgcCalculation::setAutoCalculation(bool on)
{
        m_autoCalc = on;
}

void EgcCalculation::deleteEntity(EgcEntity* entity)
{
        if (entity == dynamic_cast<EgcEntity*>(m_entity))
                m_entity = nullptr;
}
