#ifndef EGCFORMULAEXPRESSION_H
#define EGCFORMULAEXPRESSION_H

#include <QString>

class EgcExpressionNode;
class EgcBaseExpressionNode;
enum class EgcExpressionNodeType;

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
private:
        EgcBaseExpressionNode *m_data;              ///< holds a pointer to the root element of the formula tree
};

#endif // EGCFORMULAEXPRESSION_H
