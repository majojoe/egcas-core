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

#ifndef EGCDIFFERENTIALNODE_H
#define EGCDIFFERENTIALNODE_H

#include <QString>
#include "egcfnccontainernode.h"


/**
 * @brief The EgcDifferentialNode class represents a differential
 */
class EgcDifferentialNode : public EgcFncContainerNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcDifferentialNode, EgcNodeType::DifferentialNode);
public:

        /**
         * @brief The DifferentialType enum the visualisation type of the differential
         */
        enum class DifferentialType {
                lagrange1 = 1, lagrange2 = 2, lagrange3 = 3, leibnitz = 0
        };

        EgcDifferentialNode();
        /**
         * @brief setNrDerivative set the derivative level (1st, 2nd, ...).
         * @param derivative the level of derivative (1, 2, 3, ...)
         */
        void setNrDerivative(quint8 derivative);
        /**
         * @brief getNrDerivative returns if it is a 1st, 2nd, 3rd or 4th derivative
         * @return the level of derivative (1, 2, 3, ...)
         */
        quint8 getNrDerivative(void) const;
        /**
         * @brief getDifferentialType returns the differential type of this differential
         * @return the differential type used for visualisation of this differential
         */
        DifferentialType getDifferentialType(void) const;
        /**
         * @brief setDifferentialType set the differential type of this differential
         * @param type the differential type to set
         */
        void setDifferentialType(DifferentialType type);

protected:
        quint8 m_derivative;
        DifferentialType m_differentialType;

};

#endif // EGCDIFFERENTIALNODE_H
