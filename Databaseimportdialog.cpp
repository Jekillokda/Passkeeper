#include "DatabaseImportDialog.h"
#include "DataModel.h"

#include <QFileDialog>
#include <QMessageBox>

DatabaseImportDialog::DatabaseImportDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	changeColumnOrganizationButton->hide();
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(changeColumnOrganizationButton, SIGNAL(clicked()), this, SLOT(showColumnOrganizationDialog()));
}

QString DatabaseImportDialog::getPassword()
{
	return passwordEdit->text();
}

QString DatabaseImportDialog::getPath()
{
	return pathEdit->text();
}

int DatabaseImportDialog::getMode()
{
	if(appendButton->isChecked())
		return DatabaseImportDialog::Append;
	else
		return DatabaseImportDialog::Replace;
}

int DatabaseImportDialog::getFormat()
{
	if(qpaFormat->isChecked())
		return DataModel::Native;
	else
		return DataModel::Csv;
}

QVector<DataModel::Columns> DatabaseImportDialog::getColumnOrganization() const
{
	return organization;
}

void DatabaseImportDialog::browse()
{
	QFileDialog dialog(this);
	if(qpaFormat->isChecked())
        dialog.setNameFilter( tr("PassKeeper database files (*.PKdb);;All Files (*)") );
	else if(csvFormat->isChecked())
        dialog.setNameFilter( tr("CSV files (*.csv);;All Files (*)") );
	dialog.setFileMode( QFileDialog::ExistingFile );
	if(dialog.exec() == QDialog::Accepted)
		pathEdit->setText( dialog.selectedFiles().at(0));
}

void DatabaseImportDialog::accept()
{
	if(pathEdit->text().isEmpty())
	{
		QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
		box.setText( tr("File path can not be empty") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	if(qpaFormat->isChecked())
	{
		DataModel model;
		errorCode err = model.openDatabase(getPath(), getPassword());
		if(err == INVALID_PASSWORD)
		{
			QMessageBox box(this);
            box.setWindowTitle( tr("PassKeeper") );
			box.setText( tr("Incorrect password.") );
			box.setIcon(QMessageBox::Warning);
			box.exec();
			return;
		}
		else if(err == FILE_ERROR)
		{
			QMessageBox box(this);
            box.setWindowTitle( tr("PassKeeper") );
			box.setText( tr("Selected database is corrupted or it is not a qpass database.") );
			box.setIcon(QMessageBox::Warning);
			box.exec();
			return;
		}
	}
	done(QDialog::Accepted);
}

void DatabaseImportDialog::showColumnOrganizationDialog()
{
	ColumnOrganizationDialog dialog(this);
	dialog.setColumnOrganization(organization);
	if(dialog.exec() == QDialog::Accepted)
	{
		organization = dialog.getColumnOrganization();
	}
}
