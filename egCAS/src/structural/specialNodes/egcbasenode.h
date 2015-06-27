#ifndef EGCBASENODE_H
#define EGCBASENODE_H

#include "egcunarynode.h"

/**
 * @brief The EgcBaseNode class is a class that always is the base element of an expression.
 */
class EgcBaseNode : public EgcUnaryNode
{
public:
        EgcBaseNode();
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}
protected:
        static const EgcNodeType s_nodeType = EgcNodeType::BaseNode;
private:
        ///copy constructor
        EgcBaseNode(const EgcBaseNode& orig) { (void) orig;}
        ///move constructor
        EgcBaseNode(EgcBaseNode&& orig) {(void) orig;}
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBaseNode& operator=(const EgcBaseNode &rhs) {(void) rhs;}
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class (move semantic)
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBaseNode& operator=(EgcBaseNode&& rhs) {(void) rhs;}
        ///made the copy method private to be not able to use it
        virtual EgcNode* copy(void) {return new (std::nothrow) EgcBaseNode(static_cast<const EgcBaseNode&>(*this));}
        ///made the create method private to be not able to use it
        static EgcNode* create(void) {return new (std::nothrow) EgcBaseNode();}
};

#endif // EGCBASENODE_H
