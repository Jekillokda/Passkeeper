#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QList>
#include <QVector>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QObject>

struct header
{
 char id[3];
 int entriescount;
};

struct cryptedHeader
{
 char id[3];
 char space[13];
};

struct entryHeader
{
 short int entryLength;
 short int nameLength;
 short int urlLength;
 short int loginLength;
 short int passwordLength;
 char space[4];
};

enum errorCode
{
 SUCCESS = 0,
 INVALID_PASSWORD = -1,
 FILE_ERROR = -2
};

class DataAccess : public QObject
{
 public:
  DataAccess(const QString &path,const QString &password);
  ~DataAccess();
  errorCode read(QList< QVector< QString> > &list);
  errorCode write(const QList< QVector< QString> > &data);
  void setPassword(const QString &password);
  QString getPassword();
 private:
  QString path;
  QString password;
  QFile *file;
};

#endif //DATAACCESS_H
