#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Simulation.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSimulation(Simulation *sim);
    void loadParams(const Simulation::Params &p);
  public slots:
    void applyParams();

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
