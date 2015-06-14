#include <QScopedPointer>
#include "egccontainernode.h"

EgcContainerNode::EgcContainerNode()
{
}

EgcContainerNode::EgcContainerNode(const EgcContainerNode& orig)
{
        m_parent = nullptr;
}

EgcContainerNode::~EgcContainerNode()
{
}

bool EgcContainerNode::isContainer(void) const
{
        return true;
}

void EgcContainerNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        (void) old_child;
        (void) new_child;
}

EgcNode* EgcContainerNode::getChild(quint32 index) const
{
        (void) index;

        return nullptr;
}

bool EgcContainerNode::setChild(quint32 index, const EgcNode& expression)
{
        (void) index;
        (void) expression;
}

quint32 EgcContainerNode::getNumberChildNodes(void) const
{
        return 0;
}

bool EgcContainerNode::isFirstChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::isLastChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        (void) child;
        (void) index;

        return false;
}

bool EgcContainerNode::transferProperties(EgcContainerNode &from)
{
        bool retval = false;
        quint32 nrChildNodes = this->getNumberChildNodes();
        quint32 nrChildsFrom = from.getNumberChildNodes();
        quint32 i;

        if (!this->isFlexNode()) {
                if (from.getNumberChildNodes() != nrChildNodes)
                        return false;
        }

        //test if all childs are null
        for (i = 0; i < nrChildNodes; i++) {
                if (this->getChild(i) != nullptr)
                        return false;
        }

        EgcNode* child;
        QScopedPointer<EgcNode> tempChild;
        for (i = 0; i < nrChildsFrom; i++) {
                child = from.getChild(i);
                if (child) {
                        tempChild.reset(from.takeOwnership(*child));
                        this->setChild(i, *tempChild.take());
                }
        }
        m_parent = from.getParent();
        m_parent->adjustChildPointers(from, *this);
        from.m_parent = nullptr;
        retval = true;

        return retval;
}
