#include "egcflexnode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcFlexNode::EgcFlexNode() : m_child(nullptr)
{

}

EgcFlexNode::EgcFlexNode(const EgcFlexNode& orig) : EgcContainerNode(orig)
{
        m_child = nullptr;
        EgcNode *originalChild = const_cast<EgcFlexNode&>(orig).getChild(0);
        if (originalChild)
                m_child = originalChild->copy();

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcFlexNode::~EgcFlexNode()
{
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
}

EgcFlexNode& EgcFlexNode::operator=(const EgcFlexNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
        //and create a new one
        EgcNode *originalChild = rhs.getChild(0);
        if (originalChild)
                m_child = originalChild->copy();

        return *this;
}

bool EgcFlexNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

void EgcFlexNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_child == child)
                m_child = nullptr;
}

bool EgcFlexNode::isLeaf(void) const
{
        if (m_child == nullptr)
                return true;
        else
                return false;
}

void EgcFlexNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_child == &old_child)
                m_child = &new_child;
}

EgcNode* EgcFlexNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_child == &child) {
                m_child = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcFlexNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcFlexNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_child;
        else
                return nullptr;
}

bool EgcFlexNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = true;

        if (index == 0) {
                if (m_child)
                        delete m_child;
                m_child = const_cast<EgcNode*>(&expression);

                //set the parent also
                if(m_child)
                        m_child->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcFlexNode::getNumberChildNodes(void) const
{
        return 1;
}

bool EgcFlexNode::isFirstChild(EgcNode &child) const
{
        if (m_child == &child)
                return true;
        else
                return false;
}

bool EgcFlexNode::isLastChild(EgcNode &child) const
{
        if (m_child == &child)
                return true;
        else
                return false;
}

EgcNode* EgcFlexNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

EgcNode* EgcFlexNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

bool EgcFlexNode::getIndexChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                return true;
        }

        return false;
}

bool EgcFlexNode::isFlexNode(void) const
{
        return true;
}
