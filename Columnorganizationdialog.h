#ifndef COLUMNORGANIZATIONDIALOG_H
#define COLUMNORGANIZATIONDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_ColumnOrganizationDialog.h"
#include "DataModel.h"

class ColumnOrganizationDialog : public QDialog, private Ui::ColumnOrganizationDialog
{
 Q_OBJECT
public:
 ColumnOrganizationDialog(QWidget *parent = 0);
 void setColumnOrganization(const QVector<DataModel::Columns> &organization);
 QVector<DataModel::Columns> getColumnOrganization() const;
private slots:
 void accept();
};

#endif
