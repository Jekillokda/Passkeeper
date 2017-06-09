#ifndef DATABASEEXPORTDIALOG_H
#define DATABASEEXPORTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_DatabaseExportDialog.h"
#include "ColumnOrganizationDialog.h"

class DatabaseExportDialog : public QDialog, private Ui::DatabaseExportDialog
{
 Q_OBJECT
 QVector<DataModel::Columns> organization;
public:
 DatabaseExportDialog(QWidget *parent);
 QString getPassword();
 QString getPath();
 int getFormat();
 QVector<DataModel::Columns> getColumnOrganization() const;
private slots:
 void showColumnOrganizationDialog();
 void browse();
 void accept();
};

#endif //DATABASEEXPORTDIALOG_H
