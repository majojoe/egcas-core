#ifndef EGCNODECREATOR_H
#define EGCNODECREATOR_H

class EgcNode;
enum class EgcNodeType;

/**
 * @brief The EgcNodeCreator class is a class that encapsulates a lightweight
 * factory like mechanism to copy or create expression nodes.
 */
class EgcNodeCreator
{
public:
        /**
         * @brief create creates a node of the same type as the node given
         * @param node a reference to the node from which type the newly expression generated will be of.
         * @return a pointer to the newly created expression node
         */
        static EgcNode* create(EgcNodeType type);
private:
        EgcNodeCreator(){}
};

#endif // EGCNODECREATOR_H
