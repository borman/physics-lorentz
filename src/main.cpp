#include <QApplication>
#include <QGraphicsView>

#include "physmodel.h"
#include "modelengine.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  PhysModel::Params param;
  param.gridStep = 50;
  param.gridWidth = 10;
  param.gridHeight = 10;
  param.ionBaseRadius = 5;
  param.ionDeltaRadius = 2;
  param.ionOscillSpeed = 10;
  param.ionPhaseDistribution = PhysModel::Uniform;
  param.electronCount = 1000;

  PhysModel phys(param);
  ModelEngine engine(&phys);

  QGraphicsScene scene;
  scene.addItem(engine.rootItem());

  QGraphicsView view(&scene);
  view.setRenderHint(QPainter::Antialiasing);
  view.setBackgroundBrush(Qt::black);
  view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  view.setOptimizationFlag(QGraphicsView::DontSavePainterState);
  view.show();

  return app.exec();
}
