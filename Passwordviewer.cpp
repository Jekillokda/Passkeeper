#include "PasswordViewer.h"

#include <QTableWidgetItem>
PasswordViewer::PasswordViewer(QWidget *parent, QString password) : QDialog(parent)
{
	setupUi(this);

	table->clear();
	table->setColumnCount(password.size());
	table->setMaximumWidth(password.size()*25);
	table->setMinimumWidth(password.size()*25);

	for(int i = 0; i < password.size(); i++)
	{
		QTableWidgetItem *headerItem = new QTableWidgetItem();
		headerItem->setText(QString::number(i+1));
		table->setHorizontalHeaderItem(i, headerItem);

		QTableWidgetItem *item = new QTableWidgetItem();
		item->setText(password.at(i));
		table->setItem(0, i, item);
	}
}
