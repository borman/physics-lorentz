#include <QApplication>
#include <QGraphicsView>
#include <QTime>
#include <QDebug>

#include "Simulation.h"
#include "SimulationViewer.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  Simulation::Params param;
  param.gridStep = 50;
  param.gridWidth = 10;
  param.gridHeight = 10;
  param.ionBaseRadius = 5;
  param.ionDeltaRadius = 2;
  param.ionOscillSpeed = 10;
  param.ionPhaseDistribution = Simulation::Uniform;
  param.electronCount = 10;

  Simulation sim(param);

  SimulationViewer sv(&sim);
  sv.resize(sv.sizeHint());
  sv.show();

  /*
  long long count = 0;
  QTime time;
  time.start();
  while (true)
  {
    sim.advanceTime(1.0/60);
    count++;
    if (count % 100000 == 0)
    {
      qDebug() << "simulation speed:" << (double(count)*1000.0/time.elapsed()) << "fps";
      count = 0;
      time.restart();
    }
  }
  */

  /*
  ModelEngine engine(&phys);

  QGraphicsScene scene;
  scene.addItem(engine.rootItem());

  QGraphicsView view(&scene);
  view.setRenderHint(QPainter::Antialiasing);
  view.setBackgroundBrush(Qt::black);
  view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  view.setOptimizationFlag(QGraphicsView::DontSavePainterState);
  view.show();
  */

  return app.exec();
}
