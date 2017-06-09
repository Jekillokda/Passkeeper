#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_AboutDialog.h"
class AboutDialog : public QDialog, private Ui::AboutDialog
{
    Q_OBJECT
    public:
    AboutDialog(QWidget *parent=0);
};

#endif //ABOUTDIALOG_H
