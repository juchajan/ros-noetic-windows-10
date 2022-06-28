/**
 * @file /include/main_window.hpp
 *
 * @brief Qt based gui
 *
 * @date
 **/
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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

#include "myviz.h"
#include "ui_main_window.h"

#include "mymap.h"
#include "map_save_dialog.h"

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
  
  void closeEvent(QCloseEvent * event);

 public slots:
  void onClickRunButton();
  void onClickStopButton();
  void onClickCloseButton();
  void onClickResetButton();
  void onClickSaveMapButton();
  void onSaveMapFinished();

 private:
  Ui::MainWindowDesign ui;
  MyViz * myviz_;
  QProcess* qProcess_;
  bool isAllowClose_ = false;
  MapGenerator * mg_;
  MapSaveDialog * mapSaveDialog_;
};

#endif  // MAIN_WINDOW_H
