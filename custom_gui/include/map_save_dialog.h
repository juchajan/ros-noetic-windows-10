#ifndef MAP_SAVE_DIALOG_H
#define MAP_SAVE_DIALOG_H

#include "QProgressIndicator.h"

#include <QCloseEvent>
#include <QDialog>
  
class MapSaveDialog : public QDialog
{
    Q_OBJECT
    public:
        MapSaveDialog(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
        ~MapSaveDialog();
        void keyPressEvent(QKeyEvent *event);
        void closeEvent(QCloseEvent * event);
        void mapSaveFinished();
    private:
        QProgressIndicator* pi;
        bool isAllowClose_;
};

#endif // MAP_SAVE_DIALOG_H