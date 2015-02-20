#ifndef EGCFORMULAEXPRESSION_H
#define EGCFORMULAEXPRESSION_H

#include <QString>

class EgcExpressionNode;
class EgcBaseExpressionNode;
enum class EgcExpressionNodeType;


/**
 * @brief The EgcNumberResultType defines different types of number results
 */
enum class EgcNumberResultType
{
        StandardType = 0,       ///< unchanged behaviour of the result
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
        EgcFormulaExpression(EgcExpressionNodeType type);
        ///std destructor
        virtual ~EgcFormulaExpression();
        /**
         * @brief getBaseElement returns the base element of a formula
         * @return the root element of the formula
         */
        EgcBaseExpressionNode& getBaseElement(void) const;
        /**
         * @brief getRootElement returns the root (child of base element) element of a formula
         * @return the root element of the formula
         */
        EgcExpressionNode* getRootElement(void) const;
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
         * @brief Is the formula a result of a calculation?
         * @return true if the formula is a result of a calculation
         */
        bool isResult(void);
        /**
         * @brief Is this a result with a number as result
         * @return true if this is a number result
         */
        bool isNumberResult(void);
        /**
         * @brief sets the number of significant digits that is shown in the result of this formula
         * @param digits the number of digits to show in the result of this formula
         */
        void setNumberOfSignificantDigits(quint8 digits);
        /**
         * @brief sets the result type for this formula if the result is a number result
         * @param resultType the result type (engineering, scientific, integer, standard)
         */
        void setNumberResultType(EgcNumberResultType resultType);
private:
        EgcBaseExpressionNode *m_data;          ///< holds a pointer to the root element of the formula tree
        bool m_isResult;                        ///< true if the equation represents a result that comes from the CAS kernel
        bool m_isNumberResult;                  ///< true if it is a result of number type (not variable or algebraic)
        quint8 m_numberSignificantDigits;       ///< number of significant digits of a number result
        EgcNumberResultType m_numberResultType; ///< the style how the number result shall be presented to the user
};

#endif // EGCFORMULAEXPRESSION_H
