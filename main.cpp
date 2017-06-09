#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QSharedMemory>
#include <QObject>
#include <QTranslator>
#include <QFile>
#include <QSettings>

#include "App.h"
#include "MainWindow.h"
#include "PredefinedSettings.h"
#include "PreviousPasswordDialog.h"
#include "NewDatabaseDialog.h"
#include "OtherDatabaseDialog.h"

QString getDatabasePath()
{
 QSettings settings;
 QString userPath = settings.value("databasePath", PredefinedSettings::databasePath()).toString();
 if(QFile::exists( userPath ))
  return settings.value("databasePath", PredefinedSettings::databasePath()).toString();
 else
  return PredefinedSettings::databasePath();
}

void setDatabasePath(QString path)
{
 QSettings settings;
 settings.setValue("databasePath", path);
}

int main(int argc, char *argv[])
{
 App app(argc, argv);
 app.setWindowIcon(QIcon(":/icons/mainIcon"));
 QCoreApplication::setApplicationName("PassKeeper");
 QCoreApplication::setOrganizationName("PassKeeper");
 QString path = getDatabasePath();
 DataModel model;
 if(QFile::exists( path ))
  {
   PreviousPasswordDialog dialog(&model, path);
   int res = dialog.exec();
   if(res == 2)
    {
     OtherDatabaseDialog otherDatabaseDialog(&model);
     int ret = otherDatabaseDialog.exec();
     if(ret != QDialog::Accepted)
     return 0;
   else
    {
     if(otherDatabaseDialog.isSetAsDefault())
      setDatabasePath(otherDatabaseDialog.getPath());
    }
    }
   else if(res != QDialog::Accepted)
    return 0;
 }
 else
  {
   NewDatabaseDialog dialog;
   if(dialog.exec() != QDialog::Accepted)
    return 0;
   OtherDatabaseDialog other(&model);
   other.setAsDefault(true);
   other.setMode(OtherDatabaseDialog::CreateNew);
   other.setPath(path);
   if(other.exec() != QDialog::Accepted)
    return 0;
   if(other.isSetAsDefault())
    setDatabasePath(other.getPath());
  }
  MainWindow mainWindow(&model);
  app.setMainWindow(&mainWindow);
  return app.exec();
}
