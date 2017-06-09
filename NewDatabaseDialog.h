#ifndef NEWDATABASEDIALOG_H
#define NEWDATABASEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>

#include "ui_NewDatabaseDialog.h"

class NewDatabaseDialog : public QDialog, private Ui::NewDatabaseDialog
{
 public:
 NewDatabaseDialog(QWidget *parent = 0);
};

#endif //NEWDATABASEDIALOG_H
