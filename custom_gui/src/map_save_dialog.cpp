#include "map_save_dialog.h"

MapSaveDialog::MapSaveDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f), isAllowClose_(false)
{
    resize(300,300);
    pi = new QProgressIndicator(this);
    pi->startAnimation();
    pi->resize(300, 300);
    exec();
}

void MapSaveDialog::closeEvent(QCloseEvent * event)
{
  if (!isAllowClose_) 
  {
    event->ignore();
  }
}

void MapSaveDialog::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void MapSaveDialog::mapSaveFinished()
{
    isAllowClose_ = true;
    close();
}

MapSaveDialog::~MapSaveDialog()
{
    delete pi;
}