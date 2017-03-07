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

#ifndef ACTIONWRAPPER_H
#define ACTIONWRAPPER_H

#include <QObject>
#include <QChar>
#include <QMetaType>
#include "structural/actions/egcaction.h"
#include "structural/actions/egcoperations.h"

/**
 * @brief The EgcActionWrapper class is a wrapper for EgcAction to be able to transport it via QSignalMapper
 */
class ActionWrapper : public QObject
{
        Q_OBJECT
public:
        ActionWrapper(QObject *parent = 0);
        ActionWrapper(EgcAction action, QObject *parent = 0);
        ActionWrapper(const ActionWrapper& rhs);
        virtual ~ActionWrapper();
        /**
         * @brief getAction returns the wrapped action
         * @return the action wrapped
         */
        EgcAction getAction(void);

private:
        EgcAction m_action;     ///< the wrapped action
};


Q_DECLARE_METATYPE(ActionWrapper);

#endif // ACTIONWRAPPER_H
