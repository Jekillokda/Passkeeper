#include "PreviousPasswordDialog.h"
#include "DataModel.h"
#include "DataAccess.h"
#include "PredefinedSettings.h"

PreviousPasswordDialog::PreviousPasswordDialog(DataModel *model, QString &databasePath, QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    databaseLocationLabel->setText(databasePath);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(checkData()));
    connect(openOtherButton, SIGNAL(clicked()), this, SLOT(acceptOtherDatabase()));
    this->databasePath = databasePath;
    this->model = model;
}

void PreviousPasswordDialog::checkData()
{
    errorCode res = model->openDatabase(databasePath, this->passwordEdit->text());
    if(res == INVALID_PASSWORD)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("Incorrect password - PassKeeper") );
        box.setText( tr("Incorrect password.") );
        box.setIcon(QMessageBox::Warning);
        box.exec();
        return;
    }
    else if(res == FILE_ERROR)
    {
        QMessageBox box(this);
        box.setWindowTitle( tr("PassKeeper") );
        box.setText( tr("Error opening database.") );
        box.setIcon(QMessageBox::Critical);
        box.exec();
        done(QDialog::Rejected);
    }
    else if(res == 0)
        done(QDialog::Accepted);
}

void PreviousPasswordDialog::acceptOtherDatabase()
{
    done(2);
}
