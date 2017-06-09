#ifndef PREVIOUSPASSWORDDIALOG_H
#define PREVIOUSPASSWORDDIALOG_H

#include <QDialog>

#include "DataModel.h"
#include "ui_PreviousPasswordDialog.h"

class PreviousPasswordDialog : public QDialog, private Ui::PreviousPasswordDialog
{
Q_OBJECT
public:
 PreviousPasswordDialog(DataModel *model, QString &databasePath, QWidget *parent = 0);
private:
 QString databasePath;
 DataModel *model;
 private slots:
 void checkData();
 void acceptOtherDatabase();
};
#endif //PREVIOUSPASSWORDDIALOG_H
