#include "egcbinarynode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcBinaryNode::EgcBinaryNode() : m_rightChild(nullptr), m_leftChild(nullptr)
{
}

EgcBinaryNode::EgcBinaryNode(const EgcBinaryNode& orig) : EgcContainerNode(orig)
{
        m_leftChild = nullptr;
        m_rightChild = nullptr;
        EgcNode *originalChildLeft = const_cast<EgcBinaryNode&>(orig).getChild(0);
        EgcNode *originalChildRight = const_cast<EgcBinaryNode&>(orig).getChild(1);
        if (originalChildLeft)
                m_leftChild = originalChildLeft->copy();
        if (originalChildRight)
                m_rightChild = originalChildRight->copy();

        //set the parents also
        if(m_leftChild)
                m_leftChild->provideParent(this);
        if(m_rightChild)
                m_rightChild->provideParent(this);
}

EgcBinaryNode::~EgcBinaryNode()
{
        if (m_leftChild) {
                delete m_leftChild;
                m_leftChild = nullptr;
        }

        if (m_rightChild) {
                delete m_rightChild;
                m_rightChild = nullptr;
        }
}

EgcBinaryNode& EgcBinaryNode::operator=(const EgcBinaryNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_leftChild) {
                delete m_leftChild;
                m_leftChild = nullptr;
        }
        if (m_rightChild) {
                delete m_rightChild;
                m_rightChild = nullptr;
        }
        //and create a new one
        EgcNode *originalChildLeft = rhs.getChild(0);
        EgcNode *originalChildRight = rhs.getChild(1);
        if (originalChildLeft)
                m_leftChild = originalChildLeft->copy();
        if (originalChildRight)
                m_rightChild = originalChildRight->copy();

        return *this;
}

bool EgcBinaryNode::valid(void)
{
        if (m_leftChild && m_rightChild)
                if (m_leftChild->valid() && m_rightChild->valid())
                        return true;

        return false;
}

bool EgcBinaryNode::isBinaryNode(void)
{
        return true;
}

void EgcBinaryNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_leftChild == child)
                m_leftChild = nullptr;
        if (m_rightChild == child)
                m_rightChild = nullptr;
}

void EgcBinaryNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_leftChild == &old_child)
                m_leftChild = &new_child;
        else if (m_rightChild == &old_child)
                m_rightChild = &new_child;
}

EgcNode* EgcBinaryNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_leftChild == &child) {
                m_leftChild = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        } else if (m_rightChild == &child) {
                m_rightChild = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcBinaryNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcBinaryNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_leftChild;
        else if (index == 1)
                return m_rightChild;
        else
                return nullptr;
}

bool EgcBinaryNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = true;

        if (index == 0) {
                if (m_leftChild)
                        delete m_leftChild;
                m_leftChild = const_cast<EgcNode*>(&expression);

                if (m_leftChild)
                        m_leftChild->provideParent(this);
        } else if (index == 1) {
                if (m_rightChild)
                        delete m_rightChild;
                m_rightChild = const_cast<EgcNode*>(&expression);

                if (m_rightChild)
                        m_rightChild->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcBinaryNode::getNumberChildNodes(void) const
{
        return 2;
}

bool EgcBinaryNode::isFirstChild(EgcNode &child) const
{
        if (m_leftChild == &child)
                return true;
        else
                return false;
}

bool EgcBinaryNode::isLastChild(EgcNode &child) const
{
        if (m_rightChild == &child)
                return true;
        else
                return false;
}

EgcNode* EgcBinaryNode::incrementToNextChild(EgcNode &previousChild) const
{
        if (m_leftChild == &previousChild || m_leftChild == nullptr)
                return m_rightChild;
        else
                return nullptr;
}

EgcNode* EgcBinaryNode::decrementToPrevChild(EgcNode &previousChild) const
{
        if (m_rightChild == &previousChild || m_rightChild == nullptr)
                return m_leftChild;
        else
                return nullptr;
}

bool EgcBinaryNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                if (&child == m_rightChild) {
                        index = 1;
                }
                return true;
        }

        return false;
}
