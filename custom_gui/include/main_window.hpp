/**
 * @file /include/ros_qt5_gui_app/main_window.hpp
 *
 * @brief Qt based gui for ros_qt5_gui_app.
 *
 * @date November 2010
 **/
#ifndef ros_qt5_gui_app_MAIN_WINDOW_H
#define ros_qt5_gui_app_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>

#include <QComboBox>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QVariant>
#include <map>
#include <QTimer>

#include <QProcess>
#include <QDialog>

#include "myviz.h"
#include "ui_main_window.h"

#include "mymap.h"


/*****************************************************************************
** Namespace
*****************************************************************************/

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(int argc, char **argv, QWidget *parent = 0);
  ~MainWindow();
  
  void resetRviz();

  void closeEvent(QCloseEvent * event);

 public slots:
  void onClickRunButton();
  void onClickStopButton();
  void onClickCloseButton();
  void onClickSaveMapButton();
  void onSaveMapFinished();

 private:
  Ui::MainWindowDesign ui;
  MyViz * myviz_;
  QProcess* qProcess_;
  bool isAllowClose_ = false;
  MapGenerator * mg_;
  QDialog * mapSaveDialog_;
};

#endif  // ros_qt5_gui_app_MAIN_WINDOW_H
