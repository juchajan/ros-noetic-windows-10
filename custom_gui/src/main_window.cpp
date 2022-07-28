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
  // myviz_->setStyleSheet("background-color:rgb(236,236,236);");
  // setWindowFlags(Qt::WindowStaysOnTopHint);
  connect(ui.logoButton, SIGNAL(clicked()), this, SLOT(onClickCloseButton()));
  connect(ui.nameButton, SIGNAL(clicked()), this, SLOT(onClickCloseButton()));
  ui.stopButton->setEnabled(false);
  ui.saveMapButton->setVisible(false);
  ui.saveMapButton->setEnabled(false);
  connect(ui.runButton, SIGNAL(clicked()), this, SLOT(onClickRunButton()));
  connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(onClickStopButton()));
  connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(onClickResetButton()));
  connect(ui.saveMapButton, SIGNAL(clicked()), this, SLOT(onClickSaveMapButton()));
}

void MainWindow::onClickResetButton() {
  if (!ui.modeComboBox->isEnabled() && ui.modeComboBox->currentIndex() == 2)
  {
    QProcess* kill_carto_process = new QProcess(this);
    kill_carto_process->start("taskkill /t /f /pid " + QString::number(cartoProcess_->processId()));
    kill_carto_process->waitForFinished(-1);
    cartoProcess_->kill();
    kill_carto_process->kill();
    delete cartoProcess_;
    delete kill_carto_process;
    cartoProcess_ = new QProcess(this);
    cartoProcess_->start("roslaunch stella_slam stella_cartographer.launch");
    cartoProcess_->waitForStarted(-1);
  }
  myviz_->reset_();
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
    cartoProcess_ = new QProcess(this);
    if (idx == 1) {
      qProcess_->start("roslaunch custom_gui custom_control.launch");
    } else if (idx == 2) {
      qProcess_->start("roslaunch custom_gui custom_slam.launch");
      cartoProcess_->start("roslaunch stella_slam stella_cartographer.launch");
      ui.saveMapButton->setEnabled(true);
      ui.saveMapButton->setVisible(true);
      cartoProcess_->waitForStarted(-1);
    } else if (idx == 3) {
      qProcess_->start("roslaunch custom_gui custom_navigation.launch map_file:=c:/dev_ws/map/testtest.yaml");
    }
    qProcess_->waitForStarted(-1);
    ui.stopButton->setEnabled(true);
  } else {
    QMessageBox::critical(this, "경고", "모드를 선택하세요.", QMessageBox::Ok);
  }
}

void MainWindow::onClickStopButton() {
  if (ui.modeComboBox->currentIndex() == 2) {
    QProcess* kill_carto_process = new QProcess(this);
    kill_carto_process->start("taskkill /t /f /pid " + QString::number(cartoProcess_->processId()));
    kill_carto_process->waitForFinished(-1);
    cartoProcess_->kill();
    kill_carto_process->kill();
    delete cartoProcess_;
    delete kill_carto_process;
  }
  QProcess* kill_process = new QProcess(this);
  kill_process->start("taskkill /t /f /pid " + QString::number(qProcess_->processId()));
  kill_process->waitForFinished(-1);
  myviz_->reset_();
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
  ui.saveMapButton->setEnabled(false);
  mg_->start();
  mapSaveDialog_ = new MapSaveDialog(this, Qt::FramelessWindowHint);
  mapSaveDialog_->exec();
}

void MainWindow::onSaveMapFinished(){
  delete mg_;
  ui.saveMapButton->setVisible(false);
  mapSaveDialog_->hide();
  delete mapSaveDialog_;
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

//析构函数
MainWindow::~MainWindow() {
  delete myviz_;
  delete qProcess_;
}