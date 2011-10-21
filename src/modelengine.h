#ifndef MODELENGINE_H
#define MODELENGINE_H

#include <vector>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include "physmodel.h"

class ModelEngine: public QObject
{
  Q_OBJECT
  public:
    // Becomes a parent of mdl
    ModelEngine(PhysModel *mdl, QObject *parent = 0);

    QGraphicsItem *rootItem() const { return m_rootItem; }
  private slots:
    void onTimer();
  private:
    PhysModel *m_model;
    QTimer *m_timer;
    QGraphicsItem *m_rootItem;
    std::vector<QGraphicsEllipseItem *> m_ions;
};

#endif // MODELENGINE_H
