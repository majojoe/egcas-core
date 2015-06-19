#include "egcunarynode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcUnaryNode::EgcUnaryNode() : m_child(nullptr)
{

}

EgcUnaryNode::EgcUnaryNode(const EgcUnaryNode& orig) : EgcContainerNode(orig)
{
        EgcNode *originalChild = const_cast<EgcUnaryNode&>(orig).getChild(0);
        if (originalChild)
                m_child.reset(originalChild->copy());

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcUnaryNode::~EgcUnaryNode()
{
}

EgcUnaryNode& EgcUnaryNode::operator=(const EgcUnaryNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //and create a new one
        EgcNode *originalChild = rhs.getChild(0);
        if (originalChild)
                m_child.reset(originalChild->copy());

        return *this;
}

bool EgcUnaryNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

bool EgcUnaryNode::isUnaryNode(void)
{
        return true;
}

void EgcUnaryNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_child.data() == child)
                m_child.reset(nullptr);
}

void EgcUnaryNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_child.data() == &old_child) {
                (void) m_child.take();
                m_child.reset(&new_child);
        }
}

EgcNode* EgcUnaryNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_child.data() == &child) {
                retval = m_child.take();
                retval->provideParent(nullptr);
        }

        return retval;
}

void EgcUnaryNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcUnaryNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_child.data();
        else
                return nullptr;
}

bool EgcUnaryNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = true;

        if (index == 0) {
                m_child.reset(const_cast<EgcNode*>(&expression));

                //set the parent also
                if(m_child)
                        m_child->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcUnaryNode::getNumberChildNodes(void) const
{
        return 1;
}

bool EgcUnaryNode::isFirstChild(EgcNode &child) const
{
        if (m_child.data() == &child)
                return true;
        else
                return false;
}

bool EgcUnaryNode::isLastChild(EgcNode &child) const
{
        if (m_child.data() == &child)
                return true;
        else
                return false;
}

EgcNode* EgcUnaryNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

EgcNode* EgcUnaryNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

bool EgcUnaryNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                return true;
        }

        return false;
}
