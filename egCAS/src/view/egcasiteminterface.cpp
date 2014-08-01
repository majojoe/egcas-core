#include <QPointF>
#include "egcasiteminterface.h"


bool EgcasItemInterface::operator< ( const EgcasItemInterface &rhs)
{
        QPointF op1 = getPos();
        QPointF op2 = rhs.getPos();

        //first have a look at the vertical positions in a worksheet
        if ( op1.y() < op2.y() )
                return true;
        if ( op1.y() > op2.y() )
                return false;

        //if these positions are the same, then have a look at the horizontal positions in the worksheet (writing from left to right)
        if ( op1.x() < op2.x() )
                return true;
        if ( op1.x() > op2.x() )
                return false;

        return true;
}
