#ifndef EGCEXPRESSIONNODECREATOR_H
#define EGCEXPRESSIONNODECREATOR_H

class EgcExpressionNode;
enum class EgcExpressionNodeType;

/**
 * @brief The EgcExpressionNodeCreator class is a class that encapsulates a lightweight
 * factory like mechanism to copy or create expression nodes.
 */
class EgcExpressionNodeCreator
{
public:
        /**
         * @brief copy makes a copy of the node given.
         * @param node a reference to the node from which a copy will be made of.
         * @return a pointer to the newly created expression node
         */
        static EgcExpressionNode* copy(EgcExpressionNode& node);
        /**
         * @brief create creates a node of the same type as the node given
         * @param node a reference to the node from which type the newly expression generated will be of.
         * @return a pointer to the newly created expression node
         */
        static EgcExpressionNode* create(EgcExpressionNodeType type);
private:
        EgcExpressionNodeCreator(){}
};

#endif // EGCEXPRESSIONNODECREATOR_H
