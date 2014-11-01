#ifndef EGCFORMULAEXPRESSION_H
#define EGCFORMULAEXPRESSION_H

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
private:
        EgcBaseExpressionNode *m_data;              ///< holds a pointer to the root element of the formula tree
};

#endif // EGCFORMULAEXPRESSION_H
