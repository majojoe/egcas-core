#ifndef EGCASITEMINTERFACE_H
#define EGCASITEMINTERFACE_H

class QPointF;

/**
 * @brief The EgcasItemInterface class implements an interface for all classes that are derived from QGraphicsItem
 */
class EgcasItemInterface
{
public:
        ///std contructor
        EgcasItemInterface() {}
        ///std destructor
        ~EgcasItemInterface() {}
        /**
         * @brief getPos needs to be overwritten by subclasses to get the position of the item ()
         * @return the Position of the item
         */
        virtual QPointF getPos( void ) const = 0;
        /**
         * @brief operator< operator overload of this class to be able to sort the items according to their position
         * @param rhs the right hand side operator
         * @return true if this object is smaller than  the given object
         */
        bool operator< ( const EgcasItemInterface &rhs);
};

#endif // EGCASITEMINTERFACE_H
