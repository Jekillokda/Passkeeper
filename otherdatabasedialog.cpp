#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "OtherDatabaseDialog.h"

OtherDatabaseDialog::OtherDatabaseDialog(DataModel *model, QWidget *parent) : QDialog(parent)
{
 setupUi(this);
 passwordEdit2->setVisible(false);
 retypePasswordLabel->setVisible(false);
 this->model = model;
 connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
 connect(nextButton1, SIGNAL(clicked()), this, SLOT(next()));
 connect(nextButton2, SIGNAL(clicked()), this, SLOT(next()));
 connect(backButton1, SIGNAL(clicked()), this, SLOT(previous()));
 connect(backButton2, SIGNAL(clicked()), this, SLOT(previous()));
}

QString OtherDatabaseDialog::getPath()
{
 return pathEdit->text();
}

void OtherDatabaseDialog::setPath(const QString path)
{
 pathEdit->setText(path);
}

bool OtherDatabaseDialog::isSetAsDefault()
{
 return setAsDefaultBox->isChecked();
}

void OtherDatabaseDialog::setAsDefault(bool isDefault)
{
 setAsDefaultBox->setChecked(isDefault);
}

void OtherDatabaseDialog::setMode(int mode)
{
 if(mode == OpenExisting)
  openExistingButton->setChecked(true);
 else
  createNewButton->setChecked(true);
}

void OtherDatabaseDialog::browse()
{
 QString path;

 if(openExistingButton->isChecked())
  {
   path = QFileDialog::getOpenFileName(this, tr("Select database"), NULL, tr("PassKeeper database files (*.PKdb);;All Files (*)"));
  }
  else
  {
   path = QFileDialog::getSaveFileName(this, tr("Select database"), NULL, tr("PassKeeper database files (*.PKdb);;All Files (*)"));
  }
   pathEdit->setText(path);
}

void OtherDatabaseDialog::accept()
{
 if(openExistingButton->isChecked())
  {
   errorCode ret = model->openDatabase(pathEdit->text(), passwordEdit1->text());
   if(ret == INVALID_PASSWORD)
    {
      QMessageBox box(this);
      box.setWindowTitle("PassKeeper");
      box.setText( tr("Password is incorrect") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      return;
    }
     else if(ret == FILE_ERROR)
      {
       QMessageBox box(this);
       box.setWindowTitle("PassKeeper");
       box.setText( tr("Error reading database or database doesn't exist") );
       box.setIcon( QMessageBox::Critical );
       box.exec();
       return;
      }
  }
   else
   {
    if(passwordEdit1->text() != passwordEdit2->text())
     {
      QMessageBox box(this);
      box.setWindowTitle("PassKeeper");
      box.setText( tr("Passwords doesn't match") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      return;
     }
      errorCode ret = model->openDatabase(pathEdit->text(), passwordEdit1->text(), false);
      if(ret == FILE_ERROR)
       {
        QMessageBox box(this);
        box.setWindowTitle("PassKeeper");
        box.setText( tr("Selected file is not writable") );
        box.setIcon( QMessageBox::Critical );
        box.exec();
        return;
       }
  }
   done(QDialog::Accepted);
}

void OtherDatabaseDialog::next()
{
  if(stackedWidget->currentIndex() == 0)
   {
    if(openExistingButton->isChecked())
     {
      passwordEdit2->setVisible(false);
      retypePasswordLabel->setVisible(false);
      nextButton2->setText(tr("OK"));
     }
      else
       {
        passwordEdit2->setVisible(true);
        retypePasswordLabel->setVisible(true);
        nextButton2->setText(tr("Next"));
       }
      stackedWidget->setCurrentIndex(stackedWidget->currentIndex()+1);
     }
     else if(stackedWidget->currentIndex() == 1 && openExistingButton->isChecked())
      {
       emit accept();
      }
        else
        stackedWidget->setCurrentIndex(stackedWidget->currentIndex()+1);
}

void OtherDatabaseDialog::previous()
{
 stackedWidget->setCurrentIndex(stackedWidget->currentIndex()-1);
}
