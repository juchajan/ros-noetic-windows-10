/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "main_window.hpp"

#include <QString>
#include <QtGui>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QDialog>

/*****************************************************************************
** Namespaces
*****************************************************************************/

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char **argv, QWidget *parent)
    : QMainWindow(parent) {
  ui.setupUi(this);
  myviz_ = new MyViz(ui.mainHorizontalLayout);
  // setWindowFlags(Qt::WindowStaysOnTopHint);
  menuBar()->setStyleSheet("background-color:rgb(240, 240, 240);");
  menuBar()->addAction("           ", this, SLOT(onClickCloseButton()));
  menuBar()->setLayoutDirection( Qt::RightToLeft);
  menuBar()->setMouseTracking(false);


  ui.stopButton->setEnabled(false);
  ui.saveMapButton->setVisible(false);
  ui.saveMapButton->setEnabled(false);
  connect(ui.runButton, SIGNAL(clicked()), this, SLOT(onClickRunButton()));
  connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(onClickStopButton()));
  connect(ui.saveMapButton, SIGNAL(clicked()), this, SLOT(onClickSaveMapButton()));
}

void MainWindow::onClickCloseButton() {
  isAllowClose_ = !isAllowClose_;
}

void MainWindow::onClickRunButton() {
  int idx = ui.modeComboBox->currentIndex();
  if (idx) {
    ui.modeComboBox->setEnabled(false);
    ui.runButton->setEnabled(false);
    qProcess_ = new QProcess(this);
    if (idx == 1) {
      qProcess_->start("roslaunch custom_gui custom_slam.launch");
      ui.saveMapButton->setEnabled(true);
      ui.saveMapButton->setVisible(true);
    } else if (idx == 2) {
      qProcess_->start("roslaunch custom_gui custom_navigation.launch map_file:=c:/dev_ws/map/map.yaml");
    }
    qProcess_->waitForStarted(-1);
    ui.stopButton->setEnabled(true);
  } else {
    QMessageBox::critical(this, "경고", "모드를 선택하세요.", QMessageBox::Ok);
  }
}

void MainWindow::onClickStopButton() {
  QProcess* kill_process = new QProcess(this);
  kill_process->start("taskkill /t /f /pid " + QString::number(qProcess_->processId()));
  kill_process->waitForFinished(-1);
  resetRviz();
  qProcess_->kill();
  kill_process->kill();
  delete qProcess_;
  delete kill_process;
  ui.modeComboBox->setEnabled(true);
  ui.runButton->setEnabled(true);
  ui.stopButton->setEnabled(false);
  ui.saveMapButton->setVisible(false);
}

void MainWindow::onClickSaveMapButton() {
  mg_ = new MapGenerator("testtest");
  connect(mg_, SIGNAL(saveMap()), this, SLOT(onSaveMapFinished()));
  mg_->start();
  ui.saveMapButton->setEnabled(false);
  mapSaveDialog_ = new QDialog(this, Qt::FramelessWindowHint);
  QMovie *movie = new QMovie("c:/dev_ws/src/custom_gui/resources/images/wait2.gif");
  qDebug() << movie->lastErrorString();
  qDebug() << movie->fileName();
  QLabel *processLabel = new QLabel(mapSaveDialog_);
  processLabel->setMovie(movie);
  mapSaveDialog_->show();
  movie->start();
  movie->scaledSize();
}

void MainWindow::onSaveMapFinished(){
  qDebug() << "ggg";
  delete mg_;
  ui.saveMapButton->setVisible(false);
  mapSaveDialog_->close();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
  if (isAllowClose_) 
  {
    QProcess* kill_process = new QProcess(this);
    kill_process->start("taskkill /t /f /im cmd.exe");
    kill_process->waitForFinished(-1);
    qProcess_->kill();
  }
  else 
  {
    event->ignore();
  }
}


void MainWindow::resetRviz() {
    myviz_->reset_();
}

//析构函数
MainWindow::~MainWindow() {
  delete myviz_;
  delete qProcess_;
}