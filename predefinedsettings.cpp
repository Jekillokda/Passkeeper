#include <QCoreApplication>
#include <QDir>

#include "PredefinedSettings.h"

QString PredefinedSettings::dataPath()
{
    QString path;
    if(path.isEmpty())
    {
     path = QCoreApplication::applicationDirPath()+"/../share/Passkeeper";
    }
    QDir p(path);
    return p.absolutePath();
}

QString PredefinedSettings::databasePath()
{
  QDir dir;
  return dir.homePath()+"/.PKdb";
}
