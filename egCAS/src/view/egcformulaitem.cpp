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

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <libegcas/eg_mml_document.h>
#include <QRegularExpression>
#include "egcformulaitem.h"
#include "egcasscene.h"
#include "entities/egcabstractformulaentity.h"
#include "egcabstractformulaitem.h"
#include "egcscreenpos.h"
#include "actions/egcactionmapper.h"
#include "egcitemtypes.h"

quint8 EgcFormulaItem::s_baseFontSize = 20;
QRegularExpression EgcFormulaItem::s_alnumKeyFilter = QRegularExpression("[._0-9a-zA-ZΆ-ώ]+");
bool EgcFormulaItem::s_regexInitialized = false;

EgcFormulaItem::EgcFormulaItem(QGraphicsItem *parent) :
    QGraphicsItem{parent}, m_entity{nullptr}, m_posChanged{false}, m_contentChanged{false},
    m_startPoint{QPointF(0.0, 0.0)}, m_movePossible{false}, m_editingActivated{false}, m_errMsgItem{nullptr}
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        m_mathMlDoc.reset(new EgMathMLDocument());
        m_mathMlDoc->setBaseFontPixelSize(s_baseFontSize);
        m_screenPos.reset(new EgcScreenPos());
        if (!s_regexInitialized) {
                s_regexInitialized = true;
                s_alnumKeyFilter.optimize();
        }

        m_errMsgItem = new QGraphicsTextItem("", this);
        m_errMsgItem->setDefaultTextColor(QColor(Qt::red));
        QFont font;
        font.setPointSizeF(10.0);
        m_errMsgItem->setFont(font);
        m_errMsgItem->hide();
}

EgcFormulaItem::~EgcFormulaItem()
{

}

EgcFormulaItem::EgcFormulaItem(const QString &formula, QPointF point, QGraphicsItem *parent) :
        EgcFormulaItem{parent}
{
        setFormulaText(formula);
        QGraphicsItem::setPos(point);
}

EgcFormulaItem::EgcFormulaItem(const QPointF point, QGraphicsItem *parent) :
        EgcFormulaItem{parent}
{
        QGraphicsItem::setPos(point);
}

void EgcFormulaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        (void) option;
        (void) widget;

        if (!m_entity)
                m_mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(s_baseFontSize));
        else
                m_mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(m_entity->getFontSize()));

        QRectF formulaRect(QPointF(0,0), m_mathMlDoc->size());
        m_mathMlDoc->paint( painter, formulaRect.topLeft() );
        QVector<EgRenderingPosition> positions = m_mathMlDoc->getRenderingPositions();
        m_screenPos->setPositions(positions);

        if (hasFocus()) {
#ifdef DEBUG_SCENE_RENDERING_POS
                QVector<EgRenderingPosition> renderingPosition = m_mathMlDoc->getRenderingPositions();

                EgRenderingPosition position;
                Qt::GlobalColor color = Qt::gray;
                foreach (position, renderingPosition) {
                        if (position.m_subPos == 0) {
                                painter->setPen(Qt::darkGray);
                                painter->drawRect(position.m_itemRect);
                        } else {
                                painter->setPen(color);
                                painter->drawRect(position.m_itemRect);
                        }

                        if (color == Qt::darkYellow)
                                color = Qt::gray;
                        else
                                color = static_cast<Qt::GlobalColor>(static_cast<int>(color) + 1);
                }
#endif //#ifdef DEBUG_SCENE_RENDERING_POS

                painter->save();
                painter->setPen( QPen(Qt::black,
                                      qreal( 1 ),
                                      Qt::PenStyle( Qt::DashLine ) ) );
                painter->drawRect( formulaRect );
                painter->restore();
                
                //if the formula has focus, send a notification change
                if (m_entity) {
                        if (m_contentChanged) {
                                m_contentChanged = false;
                                m_entity->itemChanged(EgcItemChangeType::contentChanged);
                        }
                }
        }
}

QRectF EgcFormulaItem::boundingRect() const
{
        //the start point is the bottom left point of the formula
        QRectF bounds(QPointF(0,0), m_mathMlDoc->size());

        return bounds.adjusted(0, -1.0, +1.0, +1.0);
}

void EgcFormulaItem::setFormulaText(const QString &formula)
{
        m_mathMlDoc->setContent(formula);
}

void EgcFormulaItem::mousePressEvent(QGraphicsSceneMouseEvent*event)
{        
        if (!m_movePossible) {
                m_startPoint = pos();
                m_movePossible = true;
        }
        if (m_editingActivated) {
                setCursorAt(event->pos());
        }

        QGraphicsItem::mousePressEvent(event);
}

void EgcFormulaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
        m_movePossible = false;
        if (m_posChanged) {
                m_posChanged = false;
                //signal entity that item position has changed
                if (m_entity)
                        m_entity->itemChanged(EgcItemChangeType::posChanged);
        }

        QGraphicsItem::mouseReleaseEvent(event);
        if (m_editingActivated)
                setSelected(false);
}

void EgcFormulaItem::setEditMode(bool edit)
{
        if (edit) {
                if (!m_entity)
                        return;
                EgcAction action;
                action.m_op = EgcOperations::formulaActivated;
                m_entity->handleAction(action);
                m_editingActivated = true;
                setFocus(Qt::OtherFocusReason);
                setSelected(false);
                EgCasScene* scn = getEgcScene();
                if (scn) {
                        scn->document().startCalulation(m_entity);
                }
        } else {
                if (!m_entity)
                        return;
                EgcAction action;
                action.m_op = EgcOperations::formulaDeactivated;
                m_entity->handleAction(action);
                EgCasScene* scn = getEgcScene();
                if (scn) {
                        scn->hideFormulaCursors();
                        scn->document().resumeCalculation();
                }
                m_editingActivated = false;
        }
}

void EgcFormulaItem::setErrorMessage(QString msg)
{
        m_errMsgItem->setHtml(QString("<div style='background:rgba(220, 220, 220, 50%);'>") + msg);
}

void EgcFormulaItem::clearErrorMessage()
{
        m_errMsgItem->setHtml("");
}

void EgcFormulaItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
        if (!m_editingActivated) {
                clearFocus();
                setFocus(Qt::OtherFocusReason);
                setEditMode(true);
                setCursorAt(event->pos());
                event->accept();
        }

        QGraphicsItem::mouseDoubleClickEvent(event);
}

void EgcFormulaItem::setCursorAt(QPointF pos)
{
        EgRenderingPosition rendPos = m_screenPos->getMathmlIdAtPos(pos);
        bool rightSide = false;
        if (pos.x() > rendPos.m_itemRect.x() + (rendPos.m_itemRect.width() / 2.0))
                rightSide = true;

        m_entity->setCursorPos(rendPos.m_nodeId, rendPos.m_subPos, rightSide);
}

QVariant EgcFormulaItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
        EgCasScene* scn = getEgcScene();
        if (change == ItemPositionChange && scn) {
                // value is the new position.
                QPointF point = value.toPointF();
                if ((m_startPoint - point).manhattanLength() > (scn->grid().grid().width() / 2)) {
                        // hide the cursors
                        hideCursors();
                        m_posChanged = true;
                        ensureVisibility();
                        return snap(point);
                } else {
                        return m_startPoint;
                }
        }

        //

        return QGraphicsItem::itemChange(change, value);
}

void EgcFormulaItem::setEntity(EgcAbstractFormulaEntity* entity)
{
        m_entity = entity;
}

QPointF EgcFormulaItem::getPosition( void ) const
{
        return pos();
}

void EgcFormulaItem::setPos(const QPointF &pos)
{
        QGraphicsItem::setPos(snap(pos));
}

void EgcFormulaItem::updateView(void)
{
        if (!m_entity)
                return;
        
        prepareGeometryChange();
        m_contentChanged = true;
        m_mathMlDoc->setContent(m_entity->getMathMlCode());
        update();
        if (m_errMsgItem) {
                if (!m_errMsgItem->toHtml().isEmpty()) {
                        m_errMsgItem->show();
                        m_errMsgItem->setPos(QPointF(0.0, this->boundingRect().height() - m_errMsgItem->font().pointSizeF() + 5.0 ));
                } else {
                        m_errMsgItem->hide();
                }
        }
}

EgCasScene* EgcFormulaItem::getEgcScene(void)
{
        QGraphicsScene *scene = this->scene();
        if (scene) {
                return static_cast<EgCasScene*>(scene);
        }

        return nullptr;
}

const EgcScreenPos& EgcFormulaItem::getScreenPos(void) const
{
        return *m_screenPos.data();
}

void EgcFormulaItem::focusInEvent(QFocusEvent * event)
{
        if (event->reason() == Qt::ActiveWindowFocusReason) {
                setSelected(false);
                return;
        }
        if (event->reason() == Qt::OtherFocusReason) {
                setEditMode(true);
                event->accept();
        }

        QGraphicsItem::focusInEvent(event);
}

void EgcFormulaItem::focusOutEvent(QFocusEvent * event)
{
        if (event->reason() == Qt::ActiveWindowFocusReason)
                return;

        if (m_editingActivated) {
                setEditMode(false);
                event->accept();
        }

        QGraphicsItem::focusOutEvent(event);
}

void EgcFormulaItem::keyPressEvent(QKeyEvent * event)
{
        int key = event->key();
        EgcAction action;

        if (!m_entity)
                return;

        action = EgcActionMapper::map(event);

        switch (key) {
        case Qt::Key_Up:
        case Qt::Key_Down:
                keyCursorKeyHandler(event);
                break;
        case Qt::Key_Right:
                if (m_entity->cursorAtEnd())
                        keyCursorKeyHandler(event);
                else
                        m_entity->handleAction(action);
                break;
        case Qt::Key_Left:
                if (m_entity->cursorAtBegin())
                        keyCursorKeyHandler(event);
                else
                        m_entity->handleAction(action);
                break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
                keyCursorKeyHandler(event);
                break;
        default:
                if (action.m_op != EgcOperations::noAction)
                        m_entity->handleAction(action);
                break;
        }
}

void EgcFormulaItem::keyReleaseEvent(QKeyEvent * event)
{
        (void) event;
}

void EgcFormulaItem::showUnderline(quint32 mathmlId)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, 0);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setUnderlineCursor(QLineF(mapToScene(renderPos.m_itemRect.bottomLeft()),
                                                       mapToScene(renderPos.m_itemRect.bottomRight())));
        }
}

void EgcFormulaItem::showLeftCursor(quint32 mathmlId, quint32 subindex)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, subindex);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setFormulaCursor(QLineF(mapToScene(renderPos.m_itemRect.topLeft()),
                                                       mapToScene(renderPos.m_itemRect.bottomLeft())));
        }
}

void EgcFormulaItem::showRightCursor(quint32 mathmlId, quint32 subindex)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, subindex);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setFormulaCursor(QLineF(mapToScene(renderPos.m_itemRect.topRight()),
                                                       mapToScene(renderPos.m_itemRect.bottomRight())));
        }
}

QRectF EgcFormulaItem::getElementRect(quint32 mathmlId, quint32 subindex)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, subindex);
        if (renderPos.m_nodeId) {
                return renderPos.m_itemRect;
        }

        return QRectF();
}

void EgcFormulaItem::showCursor(QLineF cursor)
{
        EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
        if (scn)
                scn->setFormulaCursor(QLineF(mapToScene(cursor.p1()), mapToScene(cursor.p2())));
}

void EgcFormulaItem::hideCursors(void)
{
        EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
        if (scn)
                scn->hideFormulaCursors();
}

void EgcFormulaItem::keyCursorKeyHandler(QKeyEvent *keyEvent)
{
        bool accepted = false;
        int key = keyEvent->key();
        EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
        if (!scn)
                return;

        switch (key) {
        case Qt::Key_Left:
                accepted = true;
                scn->itemYieldsFocus(EgcSceneSnapDirection::left, *this);
                break;
        case Qt::Key_Right:
                accepted = true;
                scn->itemYieldsFocus(EgcSceneSnapDirection::right, *this);
                break;
        case Qt::Key_Up:
                accepted = true;
                scn->itemYieldsFocus(EgcSceneSnapDirection::up, *this);
                break;
        case Qt::Key_Down:
                accepted = true;
                scn->itemYieldsFocus(EgcSceneSnapDirection::down, *this);
                break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
                accepted = true;
                scn->itemYieldsFocus(EgcSceneSnapDirection::down, *this);
                break;
        }

        if (accepted) {
                keyEvent->accept();
        } else {
                keyEvent->ignore();
                QGraphicsItem::keyPressEvent(keyEvent);
        }
}

void EgcFormulaItem::selectFormula(bool selected)
{
        if (selected) {
                setEditMode(true);
        } else {
                setEditMode(false);
        }
}

int EgcFormulaItem::type() const
{
        return static_cast<int>(EgcGraphicsItemType::EgcFormulaItemType);
}

EgcAbstractFormulaEntity* EgcFormulaItem::getEnity(void) const
{
        return m_entity;
}

QRectF EgcFormulaItem::bRect(void) const
{
        return boundingRect();
}

QPointF EgcFormulaItem::getPos()
{
        return pos();
}
