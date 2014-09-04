#include <QString>
#include <QtTest>
#include "../../src/view/egcformulaitem.h"

class EgcasTest_View : public QObject
{
        Q_OBJECT

public:
        EgcasTest_View();

private Q_SLOTS:
        void testSceneItemSorting();
};

EgcasTest_View::EgcasTest_View()
{
}

void EgcasTest_View::testSceneItemSorting()
{
        QString dummy("");
        EgcFormulaItem *item1 = new EgcFormulaItem(dummy, QPointF(120.001, 2000.00003), 20);
        EgcFormulaItem *item2 = new EgcFormulaItem(dummy, QPointF(120.001, 2000.00004), 20);
        //test the bigger and smaller operator overload of the items (for y coordinates)
        QVERIFY(item1 < item2);
        QVERIFY(!(item2 < item1));
        QVERIFY(!(item2 < item2));
        QVERIFY(!(item1 < item1));

        item1->setPos(65665465.21565, 6516.32115610);
        item2->setPos(65665465.21566, 6516.32115610);
        //test the bigger and smaller operator overload of the items (for x coordinates)
        QVERIFY(item1->pos().x() == 65665465.21565);
        QVERIFY(item1->pos().y() == 6516.32115610);
        QVERIFY(item2->pos().x() == 65665465.21566);
        QVERIFY(item2->pos().y() == 6516.32115610);

        QVERIFY(item1 < item2);
        QVERIFY(!(item2 < item1));
        QVERIFY(!(item2 < item2));
        QVERIFY(!(item1 < item1));

        delete(item1);
        delete(item2);
}

QTEST_MAIN(EgcasTest_View)

#include "tst_egcastest_view.moc"
