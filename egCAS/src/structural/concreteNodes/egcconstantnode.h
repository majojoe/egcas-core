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

#ifndef EGCVARIABLENODE_H
#define EGCVARIABLENODE_H

#include "../actions/egcoperations.h"
#include <QString>
#include <QRegularExpression>
#include "../specialNodes/egcnode.h"
#include "structural/specialNodes/egcflexnode.h"

/**
 * @brief The EgcConstantNode class is a class that holds the leafes with variable names.
 *
 */
class EgcConstantNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcConstantNode, EgcNodeType::ConstantNode);

public:
        ///std constructor
        EgcConstantNode();
        ///constructor for constant
        EgcConstantNode(Constants constant);
        ///constructor for constant string representation
        EgcConstantNode(QString representation);
        ///destructor
        virtual ~EgcConstantNode() override;
        /**
         * @brief decode decode a Constant enum from a string representation
         * @param representation the string representation for a constant enum
         * @param constant the constant that was found from the representation
         * @return true if a enum was found from the representation
         */
        static bool decode(QString representation, Constants &constant);
        /**
         * @brief encode generate a string representation from a constant enum
         * @param constant the constant for which to generate a string representation
         * @return the string representation for the constant
         */
        static QString encode(Constants constant);
        /**
         * @brief setValue set the variable name (value)
         * @param varName the variable name as a string
         * @param subscript the subscript of the variable if any
         */
        bool setConstantFromName(const QString representation);
        /**
         * @brief getValue returns the constant of the object
         * @return the constant
         */
        virtual Constants getValue(void) const;
        /**
         * @brief setValue sets the constant of the object
         * @param constant the constant
         */
        virtual void setValue(Constants constant);
        /**
         * @brief getStringRepresentation returns the string representation for a constant enum that is saved in the object of this class
         * @return the string representation
         */
        QString getStringRepresentation(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void) override;
        /**
         * @brief operator== comparison operator overload
         * @param node the node to compare against
         * @return true if the trees are equal
         */
        virtual bool operator==(const EgcNode& node) const override;
        /**
         * @brief isOperation checks if the node is a operation. There are also nodes that are containers, but no operations
         * @return true if the node is an operation, false otherwise
         */
        virtual bool isOperation(void) const override;
        /**
         * @brief interface for serializing a class
         * @param stream the stream to use for serializing this class
         * @param properties object with all neccessary information for serializing
         */
        virtual void serialize(QXmlStreamWriter& stream, SerializerProperties& properties) override;
        /**
         * @brief deserialize interface for deserializing a class
         * @param stream the stream to use for deserializing this class
         * @param properties object with all neccessary information for deserializing
         */
        virtual void deserialize(QXmlStreamReader& stream, SerializerProperties &properties) override;

protected:

        Constants m_constant;             ///< enum variable that holds the constant type
};

#endif // EGCVARIABLENODE_H
