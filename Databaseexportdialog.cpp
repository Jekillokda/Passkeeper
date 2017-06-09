#include "DatabaseExportDialog.h"
#include "DataModel.h"

#include <QFileDialog>
#include <QMessageBox>

DatabaseExportDialog::DatabaseExportDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	changeColumnOrganizationButton->hide();
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(changeColumnOrganizationButton, SIGNAL(clicked()), this, SLOT(showColumnOrganizationDialog()));
}

QString DatabaseExportDialog::getPassword()
{
	return passwordEdit1->text();
}

QString DatabaseExportDialog::getPath()
{
	return pathEdit->text();
}

int DatabaseExportDialog::getFormat()
{
    if(PKdbFormat->isChecked())
		return DataModel::Native;
	else
		return DataModel::Csv;
}

QVector<DataModel::Columns> DatabaseExportDialog::getColumnOrganization() const
{
	return organization;
}

void DatabaseExportDialog::browse()
{
	QFileDialog dialog(this);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	
    if(PKdbFormat->isChecked())
	{
        dialog.setNameFilter("PassKeeper database files (*.PKdb);;All Files (*)");
        dialog.setDefaultSuffix("PKdb");
	}
	else if(csvFormat->isChecked())
	{
        dialog.setNameFilter("CSV files (*.csv);;All Files (*)");
		dialog.setDefaultSuffix("csv");
	}
	if(dialog.exec() == QDialog::Accepted)
	{
		pathEdit->setText(dialog.selectedFiles()[0]);
	}
}

void DatabaseExportDialog::accept()
{
	if(passwordEdit1->text() != passwordEdit2->text())
	{
		QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
		box.setText( tr("Passwords does not match") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	if(pathEdit->text().isEmpty())
	{
		QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
		box.setText( tr("File path can not be empty") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	done(QDialog::Accepted);
}

void DatabaseExportDialog::showColumnOrganizationDialog()
{
	ColumnOrganizationDialog dialog(this);
	dialog.setColumnOrganization(organization);
	if(dialog.exec() == QDialog::Accepted)
	{
		organization = dialog.getColumnOrganization();
	}
}
