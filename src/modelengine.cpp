#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

#include "modelengine.h"

using namespace Geom;

ModelEngine::ModelEngine(PhysModel *mdl, QObject *parent)
  : QObject(parent), m_model(mdl)
{
  QGraphicsRectItem *rootRect = new QGraphicsRectItem(
      0, 0, mdl->width(), mdl->height());
  rootRect->setBrush(Qt::black);
  m_rootItem = rootRect;

  QColor ionColor(240, 200, 40);
  m_ions.reserve(mdl->ionCount());
  for (int i=0; i<mdl->ionCount(); i++)
  {
    QPointF center = mdl->ionPosition(i);
    qreal radius = mdl->ionRadius(i); 
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(
        -radius, -radius, 2*radius, 2*radius, rootRect);
    item->setPen(ionColor);
    item->setBrush(ionColor);
    item->setPos(center);
    m_ions.push_back(item);
  }

  m_timer = new QTimer(this);
  m_timer->setInterval(1000/60);
  m_timer->start();
  connect(m_timer, SIGNAL(timeout()), SLOT(onTimer()));
}

void ModelEngine::onTimer()
{
  m_model->advanceTime(1.0/60);
  for (int i=0; i<m_model->ionCount(); i++)
  {
    QPointF center = m_model->ionPosition(i);
    qreal radius = m_model->ionRadius(i);
    m_ions[i]->setPos(center);
    m_ions[i]->setRect(-radius, -radius, radius*2, radius*2);
  }
}

