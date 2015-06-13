#ifndef EGCNODE_H
#define EGCNODE_H

#include "egcnode_gen.h"

class EgcContainerNode;
class EgcNodeVisitor;

/** macro for setting the expression type of a class. Change this if you want to have the type
 * changed in a subclass */
#define EGC_SET_EXPRESSION_TYPE(classname, type)                                                                       \
public:                                                                                                                \
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}                                     \
        virtual EgcNode* copy(void) {                                                                        \
                return new (std::nothrow) classname(static_cast<const classname&>(*this));                             \
        }                                                                                                              \
        static EgcNode* create(void) {return new (std::nothrow) classname();}                                \
protected:                                                                                                             \
        static const EgcNodeType s_nodeType = type

/**
 * @brief The EgcNode class defines the base class for all expressions
 * (multiplication, substraction, ...) in the expression tree.
 * The class is built up as composite pattern.
 */
class EgcNode
{
public:
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}
        virtual EgcNode* copy(void) {return nullptr;}
        static EgcNode* create() {return nullptr;}
protected:
        static const EgcNodeType s_nodeType = EgcNodeType::NodeUndefined;

public:
        EgcNode();
        virtual ~EgcNode() = 0;
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * An expression is valid if all nodes are valid.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void) const;
        /**
         * @brief isUnaryExpression returns if the current element is a unary expression (container) or not
         * @return true if it is a unary expression, false otherwise
         */
        virtual bool isUnaryNode(void);
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @return true if it is a binary expression, false otherwise
         */
        virtual bool isBinaryNode(void);
        /**
         * @brief isFlexNode returns if the current element is a flex node (container) or not
         * @return true if it is a flex node, false otherwise
         */
        virtual bool isFlexNode(void) const;
        /**
         * @brief getParent returns a pointer to the parent node
         * @return a pointer to the parent node or NULL if this is the root element
         */
        EgcContainerNode* getParent(void);
        /**
         * @brief isParent checks if the given node is the parent of this node
         * @param parent the node to check wether it is the parent of the current node or not.
         * @return true if the given node is the parent, false otherwise
         */
        bool isChild(EgcNode& parent);
        /**
         * @brief provideParent THIS IS NO USER INTERFACE! DO NOT USE THAT FROM USER POINT OF VIEW!
         * This is used to set the parent pointers in the implementation of the tree.
         * @param parent a pointer to the parent of this node. This must be a container pointer.
         */
        void provideParent(EgcContainerNode* parent);
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcNode* child) { (void)child; }
        /**
         * @brief isLeaf checks if the current node is a leaf (there are no childs)
         * @return true if it is a leaf, false otherwise
         */
        virtual bool isLeaf(void) const;
        /**
         * @brief accept takes a visitor and calls the given visitor back to be able to extract data from this node
         * @param visitor the visitor to call back
         */
        virtual void accept(EgcNodeVisitor *visitor);

protected:
        EgcContainerNode *m_parent;    ///< pointer to the parent (is needed for traversing the tree)
};

#endif // EGCNODE_H
