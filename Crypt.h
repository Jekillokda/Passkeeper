#ifndef CRYPT_H
#define CRYPT_H

#include <QObject>
#include <QDebug>

class Crypt
{
public:
 Crypt(QObject *parent = 0);
 QString cryptString(QString toCrypt,int length);
 QString decryptString(QString toDecrypt);
private:
 int keyIndex(int index);
};

#endif // CRYPT_H
