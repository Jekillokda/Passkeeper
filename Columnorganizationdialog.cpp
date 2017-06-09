#include "ColumnOrganizationDialog.h"

#include <QMessageBox>
ColumnOrganizationDialog::ColumnOrganizationDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

void ColumnOrganizationDialog::setColumnOrganization(const QVector<DataModel::Columns> &organization)
{
    if(organization.size() == 4)
	{
		c1Box->setCurrentIndex(organization[0]);
		c2Box->setCurrentIndex(organization[1]);
		c3Box->setCurrentIndex(organization[2]);
		c4Box->setCurrentIndex(organization[3]);
	}
}

QVector<DataModel::Columns> ColumnOrganizationDialog::getColumnOrganization() const
{
    QVector<DataModel::Columns> organization(4);
	organization[0] = (DataModel::Columns)c1Box->currentIndex();
	organization[1] = (DataModel::Columns)c2Box->currentIndex();
	organization[2] = (DataModel::Columns)c3Box->currentIndex();
	organization[3] = (DataModel::Columns)c4Box->currentIndex();

	return organization;
}

void ColumnOrganizationDialog::accept()
{
    int occurenceCount[4] = {0,0,0,0};
	occurenceCount[c1Box->currentIndex()]++;
	occurenceCount[c2Box->currentIndex()]++;
	occurenceCount[c3Box->currentIndex()]++;
	occurenceCount[c4Box->currentIndex()]++;

	bool ok = true;
    for(int i = 0; i < 3; i++)
	{
		if(occurenceCount[i] > 1)
		{
			ok = false;
			QMessageBox box(this);
			box.setText( tr("There are duplicate selections!") );
            box.setWindowTitle( tr("PassKeeper - warning") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
		}
	}
	if(ok)
		done(QDialog::Accepted);
}
