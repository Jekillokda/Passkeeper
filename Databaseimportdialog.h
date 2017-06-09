#ifndef DATABASEIMPORTDIALOG_H
#define DATABASEIMPORTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_databaseimportdialog.h"
#include "datamodel.h"
#include "ColumnOrganizationDialog.h"

class DatabaseImportDialog : public QDialog, private Ui::DatabaseImportDialog
{
 Q_OBJECT
 QVector<DataModel::Columns> organization;
public:
 DatabaseImportDialog(QWidget *parent);
 QString getPassword();
 QString getPath();
 int getMode();
 int getFormat();
 QVector<DataModel::Columns> getColumnOrganization() const;
 enum mode
 {
  Append,
  Replace
 };
private slots:
 void showColumnOrganizationDialog();
 void browse();
 void accept();
};

#endif //DATABASEIMPORTDIALOG_H
