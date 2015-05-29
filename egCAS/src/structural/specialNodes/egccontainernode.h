#ifndef EGCCONTAINERNODE_H
#define EGCCONTAINERNODE_H

#include <new>
#include <QtGlobal>
#include "egcnode.h"

/**
 * @brief The EgcContainerNode class is a base class for a container expression that (binary or unary expression).
 */
class EgcContainerNode : public EgcNode
{
public:
        ///std contructor
        EgcContainerNode();
        ///copy constructor
        EgcContainerNode(const EgcContainerNode& orig);
        ///destructor
        virtual ~EgcContainerNode() = 0;
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void) const;
        /**
         * @brief transferChilds transfers all properties (childs and parent) from the node "from" to the node "to".
         * After this operation has succeeded the node "from" has no childs or parent anymore. The operation is only
         * possible if the node "to" has no childs at all and the nodes "from" and "to" are of the same type (all are
         * unary or binary types). The node "to" takes ownership of the childs of the node "from".
         * @param to the node to transfer the childs to.
         * @return true if the operation succeeded, false if the nodes are of different types or the node "to" is not
         * empty.
         */
        virtual bool transferPropertiesTo(EgcNode &to) = 0;
        /**
         * @brief adjustChildPointers adjust the child pointers of the current object to point to the new child given.
         * ATTENTION: use this with care since the operation doesn't take care about the old childs. The caller must
         * assure that the old child will be properly deleted -> leak otherwise. It is very unlikely that you will need
         * this function outside the container classes.
         * @param old_child the old child that shall be adjusted to the new one
         * @param new_child child pointers of the current object will be adjusted to this child object.
         */
        virtual void adjustChildPointers(EgcNode &old_child, EgcNode &new_child) = 0;
        /**
         * @brief takeOwnership takes ownership of the child given. The user is responsible for deleting the child.
         * If the user doesn't handle the child properly a leak will occur.
         * @param child the child to take ownership over.
         * @return a pointer to the child the ownership taken.
         */
        virtual EgcNode* takeOwnership(EgcNode &child) = 0;
        /**
         * @brief getChild get a pointer to the child at index. If index is > getNumberChildNodes() - 1, the returned
         * pointer is NULL.
         * @param index the index where the child is. E.g. a binary node has a left child 0 and a right child with index
         * 1 (the index starts at 0)
         * @return a pointer to the child at the index position
         */
        virtual EgcNode* getChild(quint32 index) const = 0;
        /**
         * @brief setChild set the given expression as a child at position index. If
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param child the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, const EgcNode& child) = 0;
        /**
         * @brief getNumberChildNodes returns the number of child nodes a container can have. Even the childs that are
         * null are counted in this context. Only the possibility to hold a number of childs counts. E.g. a binary
         * expression will always return 2 even if one or both of the child pointers are null.
         * @return the number of childs a container can hold.
         */
        virtual quint32 getNumberChildNodes(void) const = 0;
};

#endif // EGCCONTAINERNODE_H

