#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QString>

#include "DataAccess.h"
#include "CsvFormat.h"

class DataModel : public QAbstractTableModel
{
 Q_OBJECT
public:
 enum FileFormat
 {
  Native,
  Csv
 };
  enum Columns {Name, URL, UserName, Password};
  DataModel(QObject *parent = 0);
  ~DataModel();
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
  void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
  errorCode exportDatabase(const QString &path, const QString &password, int format = Native, QVector<Columns> organization = QVector<Columns>());
  int importDatabase(const QString &path,const QString &password, bool replaceExisting = false, int format = Native, QVector<Columns> organization = QVector<Columns>());
  QString getPassword();
  errorCode changePassword(const QString &newPassword);
  void swapEntries(int firstIndex, int secondIndex);
  errorCode saveDatabase();
  errorCode openDatabase(const QString &path,const QString &password, bool openExisting = true);
private:
  QList< QVector< QString > > dataList;
  DataAccess *database;
};

#endif //DATAMODEL_H
