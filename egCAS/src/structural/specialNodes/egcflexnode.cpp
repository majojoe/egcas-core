#include <QScopedPointer>
#include "egcflexnode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcFlexNode::EgcFlexNode() : m_childs(1)
{

}

EgcFlexNode::EgcFlexNode(const EgcFlexNode& orig) : EgcContainerNode(orig)
{
        m_childs.clear();
        EgcNode *originalChild;
        quint32 i;
        quint32 cnt = orig.m_childs.count();
        QScopedPointer<EgcNode> child;
        for (i = 0; i < cnt; i++) {
                originalChild = orig.getChild(i);
                if (originalChild)
                        child.reset(originalChild->copy());

                //set the parent also
                if(child.data()) {
                        child->provideParent(this);
                        m_childs.append(child.take());
                }
        }

        if (m_childs.empty()) {
                m_childs.resize(1);
                m_childs[0] = nullptr;
        }
}

EgcFlexNode::~EgcFlexNode()
{
        quint32 i;
        quint32 cnt = m_childs.count();
        EgcNode* child;

        if (!m_childs.empty()) {
                for (i = 0; i < cnt; i++) {
                        child = m_childs.at(i);
                        delete child;
                        child = nullptr;
                }
                m_childs.clear();
        }
        m_childs.resize(1);
        m_childs[0] = nullptr;
}

EgcFlexNode& EgcFlexNode::operator=(const EgcFlexNode &rhs)
{
        quint32 i;
        quint32 cnt = m_childs.count();
        EgcNode* child;
        quint32 cntRhs = rhs.getNumberChildNodes();

        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (!m_childs.empty()) {
                for (i = 0; i < cnt; i++) {
                        child = m_childs.at(i);
                        delete child;
                }
                m_childs.clear();
        }

        //and create a new one
        if (cntRhs != 0) {
                for (i = 0; i < cntRhs; i++) {
                        child = rhs.getChild(0);
                        QScopedPointer<EgcNode> childCopy;
                        if (child)
                                childCopy.reset(child->copy());
                        m_childs.append(childCopy.take());
                }
        }

        return *this;
}

bool EgcFlexNode::valid(void)
{
        quint32 i;
        quint32 cnt = m_childs.count();
        EgcNode* child;

        if (m_childs.empty())
                return false;

        for (i = 0; i < cnt; i++) {
                child = m_childs.at(i);
                if (child)
                        if (!child->valid())
                                return false;
        }

        return true;
}

void EgcFlexNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        int ind = m_childs.indexOf(child);
        if (ind > 0) {
                m_childs[ind] = nullptr;
        }
}

void EgcFlexNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        int ind = m_childs.indexOf(&old_child);
        if (ind > 0) {
                m_childs[ind] = &new_child;
        }
}

EgcNode* EgcFlexNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        int ind = m_childs.indexOf(&child);
        if (ind > 0) {
                m_childs[ind] = nullptr;
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
        if (m_childs.empty())
                return nullptr;

        if (index < m_childs.count()) {
                return m_childs.at(index);
        } else {
                return nullptr;
        }
}

bool EgcFlexNode::setChild(quint32 index, const EgcNode& expression)
{
        quint32 i;

        quint32 count = (quint32)m_childs.count();
        if (index >= count) {
                m_childs.resize(index + 1);
        }
        quint32 countNew = (quint32)m_childs.count();

        for (i = count; i < countNew; i++) {
                m_childs[i] = nullptr;
        }

        if (m_childs[index]) {
                delete m_childs[index];
        }

        m_childs[index] = const_cast<EgcNode*>(&expression);

        //set the parent also
        if(m_childs[index])
                m_childs[index]->provideParent(this);

        return true;
}

quint32 EgcFlexNode::getNumberChildNodes(void) const
{
        return (quint32)m_childs.count();
}

bool EgcFlexNode::isFirstChild(EgcNode &child) const
{
        if (m_childs.empty())
                return false;

        if (m_childs.at(0) == &child)
                return true;
        else
                return false;
}

bool EgcFlexNode::isLastChild(EgcNode &child) const
{
        if (m_childs.empty())
                return false;

        if (m_childs.at(m_childs.count() - 1) == &child)
                return true;
        else
                return false;
}

EgcNode* EgcFlexNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;
        int tempIndex = m_childs.indexOf(&previousChild);
        quint32 i;
        quint32 index = (quint32)tempIndex;
        quint32 nrChilds = m_childs.count();

        if (m_childs.empty() || tempIndex < 0)
                return nullptr;

        for (i = index + 1; i < nrChilds; i++) {
                if (m_childs.at(i))
                        return m_childs.at(i);
        }

        return nullptr;
}

EgcNode* EgcFlexNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;
        int tempIndex = m_childs.indexOf(&previousChild);
        quint32 i;
        quint32 index = (quint32)tempIndex;

        if (m_childs.empty() || tempIndex < 0)
                return nullptr;

        for (i = index - 1; i < index; i--) {
                if (m_childs.at(i))
                        return m_childs.at(i);
        }

        return nullptr;
}

bool EgcFlexNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                int ind = m_childs.indexOf(&child);
                if (ind >= 0) {
                        index = (quint32)ind;
                        return true;
                }
        }

        return false;
}

bool EgcFlexNode::isFlexNode(void) const
{
        return true;
}

bool EgcFlexNode::insert(quint32 index, EgcNode& node)
{
        bool retval = true;
        quint32 count = (quint32)m_childs.count();

        if (index > count)
                return false;

        m_childs.insert((int)index, &node);

        count = (quint32)m_childs.count();
        if (index >= 0 && index < count) {
                //set the parent also
                if(m_childs[index])
                        m_childs[index]->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

bool EgcFlexNode::remove(quint32 index)
{
        bool retval = true;

        if (index < m_childs.count())
                m_childs.remove(index);
        else
                retval = false;

        return retval;
}
