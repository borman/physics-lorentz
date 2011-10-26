#ifndef MODELENGINE_H
#define MODELENGINE_H

#include <vector>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTimer>

#include "Simulation.h"

class ModelEngine: public QObject
{
  Q_OBJECT
  public:
    // Becomes a parent of mdl
    ModelEngine(Simulation *mdl, QObject *parent = 0);

    QGraphicsItem *rootItem() const { return m_rootItem; }
  private slots:
    void onTimer();
  private:
    Simulation *m_model;
    QTimer *m_timer;
    QGraphicsItem *m_rootItem;
    std::vector<QGraphicsEllipseItem *> m_ions;
    std::vector<QGraphicsRectItem *> m_electrons;
};

#endif // MODELENGINE_H
