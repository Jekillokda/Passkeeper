#include "PasswordChangeDialog.h"

#include <QMessageBox>

PasswordChangeDialog::PasswordChangeDialog(const QString &currentPassword, QWidget *parent) : QDialog(parent)
{
 setupUi(this);
 this->currentPassword = currentPassword;
}

QString PasswordChangeDialog::getNewPassword()
{
 return newPasswordEdit1->text();
}

void PasswordChangeDialog::accept()
{
 if(newPasswordEdit1->text() != newPasswordEdit2->text())
  {
   QMessageBox box(this);
   box.setWindowTitle( tr("PassKeeper") );
   box.setText( tr("Passwords does not match") );
   box.setIcon( QMessageBox::Warning );
   box.exec();
   return;
  }
  if(currentPasswordEdit->text() != currentPassword)
   {
    QMessageBox box(this);
    box.setWindowTitle( tr("PassKeeper") );
    box.setText( tr("Incorrect current password.") );
    box.setIcon(QMessageBox::Warning);
    box.exec();
    return;
   }
	if(newPasswordEdit1->text().isEmpty())
     {
      QMessageBox box(this);
      box.setWindowTitle( tr("PassKeeper") );
      box.setText( tr("New password can't be empty.") );
      box.setIcon(QMessageBox::Warning);
      box.exec();
      return;
     }
	done(QDialog::Accepted);
}
