#include "egcunaryexpressionnode.h"
#include "../egcexpressionnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcUnaryExpressionNode::EgcUnaryExpressionNode() : m_child(nullptr)
{

}

EgcUnaryExpressionNode::EgcUnaryExpressionNode(const EgcUnaryExpressionNode& orig)
{
        m_child = nullptr;
        EgcExpressionNode *originalChild = const_cast<EgcUnaryExpressionNode&>(orig).getChild();
        if (originalChild)
                m_child = originalChild->copy();

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcUnaryExpressionNode::~EgcUnaryExpressionNode()
{
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
}

void EgcUnaryExpressionNode::setChild(const EgcExpressionNode& expression)
{
        if (m_child)
                delete m_child;
        m_child = const_cast<EgcExpressionNode*>(&expression);

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcExpressionNode* EgcUnaryExpressionNode::getChild(void) const
{
        return m_child;
}

EgcUnaryExpressionNode& EgcUnaryExpressionNode::operator=(const EgcUnaryExpressionNode &rhs)
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
        EgcExpressionNode *originalChild = rhs.getChild();
        if (originalChild)
                m_child = originalChild->copy();

        return *this;
}

bool EgcUnaryExpressionNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

bool EgcUnaryExpressionNode::isUnaryExpression(void)
{
        return true;
}

void EgcUnaryExpressionNode::notifyContainerOnChildDeletion(EgcExpressionNode* child)
{
        if (m_child == child)
                m_child = nullptr;
}

bool EgcUnaryExpressionNode::isLeaf(void) const
{
        if (m_child == nullptr)
                return true;
        else
                return false;
}

bool EgcUnaryExpressionNode::transferPropertiesTo(EgcExpressionNode &to)
{
        bool retval = false;
        EgcUnaryExpressionNode &to_una = static_cast<EgcUnaryExpressionNode&>(to);

        if (to_una.m_child == nullptr) {
                if (to.isUnaryExpression()) {
                        EgcContainerNode *parent_container;
                        retval = true;
                        to_una.m_child = m_child;
                        to_una.m_parent = m_parent;
                        parent_container = static_cast<EgcContainerNode*>(m_parent);
                        parent_container->adjustChildPointers(*this, to);
                        m_child = nullptr;
                        m_parent = nullptr;
                }
        }

        return retval;
}

void EgcUnaryExpressionNode::adjustChildPointers(EgcExpressionNode &old_child, EgcExpressionNode &new_child)
{
        if (m_child == &old_child)
                m_child = &new_child;
}

EgcExpressionNode* EgcUnaryExpressionNode::takeOwnership(EgcExpressionNode &child)
{
        EgcExpressionNode* retval = nullptr;

        if (m_child == &child) {
                m_child = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcUnaryExpressionNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}
