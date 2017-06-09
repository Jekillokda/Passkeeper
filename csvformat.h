#ifndef CSVFORMAT_H
#define CSVFORMAT_H

#include <QString>
#include <QList>
#include <QVector>
#include <QFile>
#include <QObject>
#include <QStringList>

class CsvFormat
{
public:
 enum ErrorID {
 OpeningError = -1,
 IncorrectStructure = -2
              };
 CsvFormat(const QString &filename);
 QList< QVector< QString > > read();
 bool write(const QList< QVector< QString > > &data);
 int getErrorID();
private:
 int error;
 QFile file;
 QStringList split(QString &str);
};

#endif //CSVFORMAT_H
