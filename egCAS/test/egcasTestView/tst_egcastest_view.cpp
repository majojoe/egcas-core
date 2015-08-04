/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

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

        item1->setPos(QPointF(65665465.21565, 6516.32115610));
        item2->setPos(QPointF(65665465.21566, 6516.32115610));
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
