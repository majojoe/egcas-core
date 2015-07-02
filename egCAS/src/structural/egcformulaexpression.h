/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#ifndef EGCFORMULAEXPRESSION_H
#define EGCFORMULAEXPRESSION_H

#include <QString>
#include <QScopedPointer>
#include "specialNodes/egcbasenode.h"

class EgcNode;
class EgcBaseNode;
enum class EgcNodeType;


/**
 * @brief The EgcNumberResultType defines different types of number results
 */
enum class EgcNumberResultType
{
        NotApplicable = 0,      ///< if this is not applicable
        StandardType,           ///< unchanged behaviour of the result
        IntegerType,            ///< result will be an integer
        EngineeringType,        ///< result will be of engineering type (exponent a multiple of 3)
        ScientificType          ///< scientific notation of the result
};

/**
 * @brief The EgcFormulaExpression class defines a wrapper for a whole equation
 */
class EgcFormulaExpression
{
public:
        /**
         * @brief EgcFormulaExpression std constructor
         * @param type the type of the root element of the formula to generate
         */
        EgcFormulaExpression(EgcNodeType type);
        /**
         * @brief EgcFormulaExpression std constructor
         * @param rootElement a reference to the rootElement of the formula to create (if the root element already exists)
         */
        EgcFormulaExpression(EgcNode& rootElement);
        ///copy constructor
        EgcFormulaExpression(const EgcFormulaExpression& orig);
        ///move constructor
        EgcFormulaExpression(EgcFormulaExpression&& orig);
        ///std destructor
        virtual ~EgcFormulaExpression();
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcFormulaExpression& operator=(const EgcFormulaExpression &rhs);
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcFormulaExpression& operator=(EgcFormulaExpression&& rhs);
        /**
         * @brief getBaseElement returns the base element of a formula
         * @return the root element of the formula
         */
        EgcBaseNode& getBaseElement(void) const;
        /**
         * @brief getRootElement returns the root (child of base element) element of a formula
         * @return the root element of the formula
         */
        EgcNode* getRootElement(void) const;
        /**
         * @brief getMathMlCode returns the mathMl representation for this formula
         * @return the mathMl representation of this formula as a string
         */
        QString getMathMlCode(void);
        /**
         * @brief getCASKernelCommand returns the command to execute the calculation(s) needed for this formula in the
         * used CAS kernel
         * @return the command to be executed in the CAS kernel as a string
         */
        QString getCASKernelCommand(void);
        /**
         * @brief Is the formula a result of a calculation? (Has to be set by the formula(visitor) - when the formula changes)
         * @return true if the formula is a result of a calculation
         */
        bool isResult(void);
        /**
         * @brief Is this a result with a number as result. (Has to set/triggered by the kernel parser)
         * @return true if this is a number result
         */
        bool isNumberResult(void);
        /**
         * @brief sets the number of significant digits that is shown in the result of this formula
         * @param digits the number of digits to show in the result of this formula
         */
        void setNumberOfSignificantDigits(quint8 digits);
        /**
         * @brief returns the number of significant digits set by the user
         * @return the number of significant digits (if set and applicable) or 0 otherwise
         */
        quint8 getNumberOfSignificantDigits(void);
        /**
         * @brief sets the result type for this formula if the result is a number result
         * @param resultType the result type (engineering, scientific, integer, standard)
         */
        void setNumberResultType(EgcNumberResultType resultType);
        /**
         * @brief returns the result type of the result (if one)
         * @return returns EgcNumberResultType::NotApplicable if not a number result type and the number result type
         * set by the user otherwise
         */
        EgcNumberResultType getNumberResultType();
        /**
         * @brief returns the global number of significant digits
         * @return the global significant digits
         */
        static quint8 getStdNrSignificantDigis(void);
        /**
         * @brief set the number of global significant digits (valid in the whole document)
         * @param digits the number of global significant digits
         */
        static void setStdNrSignificantDigis(quint8 digits);
private:
        bool m_isResult;                        ///< true if the equation represents a result that comes from the CAS kernel
        bool m_isNumberResult;                  ///< true if it is a result of number type (not variable or algebraic)
        quint8 m_numberSignificantDigits;       ///< number of significant digits of a number result
        EgcNumberResultType m_numberResultType; ///< the style how the number result shall be presented to the user
        EgcBaseNode m_data;                     ///< holds a pointer to the root element of the formula tree
        static quint8 s_stdNrSignificantDigits; ///< the number of significant digits (in a global mannner (std))
};

#endif // EGCFORMULAEXPRESSION_H
