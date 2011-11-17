#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->btnStep, SIGNAL(clicked()), ui->simViewer, SLOT(step()));
  connect(ui->btnRun, SIGNAL(toggled(bool)), ui->simViewer, SLOT(run(bool)));
  connect(ui->btnRun, SIGNAL(toggled(bool)), ui->btnStep, SLOT(setDisabled(bool)));
  connect(ui->btnRun, SIGNAL(toggled(bool)), ui->grpSimOptions, SLOT(setDisabled(bool)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setSimulation(Simulation *sim)
{
  ui->simViewer->setSimulation(sim);
  loadParams(sim->params());
}

void MainWindow::loadParams(const Simulation::Params &p)
{
  ui->spbDistance->setValue(p.gridStep);
  ui->spbColumnCount->setValue(p.gridWidth);
  ui->spbRowCount->setValue(p.gridHeight);
  ui->spbBaseRadius->setValue(p.ionBaseRadius);
  ui->spbDeltaRadius->setValue(p.ionDeltaRadius);
  ui->spbOscRate->setValue(p.ionOscillSpeed);
  ui->spbElectronBaseSpeed->setValue(p.electronBaseSpeed);
  ui->spbElectronCount->setValue(p.electronCount);
}
