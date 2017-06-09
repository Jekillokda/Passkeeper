#ifndef OTHERDATABASEDIALOG_H
#define OTHERDATABASEDIALOG_H

#include <QDialog>

#include "DataModel.h"
#include "ui_OtherDatabaseDialog.h"

class OtherDatabaseDialog : public QDialog, private Ui::OtherDatabaseDialog
{
 Q_OBJECT
public:
 OtherDatabaseDialog(DataModel *model, QWidget *parent = 0);
 QString getPath();
 void setPath(const QString path);
 bool isSetAsDefault();
 void setAsDefault(bool isDefault);
 void setMode(int mode);
 enum Modes
  {
   OpenExisting,
   CreateNew
  };
private:
 QString newpath;
 DataModel *model;
private slots:
 void next();
 void previous();
 void browse();
 void accept();
};

#endif
