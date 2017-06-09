#include "Crypt.h"
static const QString kEncryptorString = QString("encryption key encryption key");

Crypt::Crypt(QObject *parent)
{
}

QString Crypt::cryptString(QString toCrypt,int length)
{
   toCrypt.resize(length);
   QString resultString = "";
   for ( int i = 0; i < toCrypt.length(); i++)
    {
      resultString.append(QString(QChar(toCrypt[i]).unicode()^QChar(kEncryptorString[keyIndex(i)]).unicode()));
     }
     return resultString;
    }

QString Crypt:: decryptString(QString toDecrypt)
    {
        QString resultString = "";
        for ( int i = 0; i < toDecrypt.length(); i++)
        {
            resultString.append(QString(QChar(toDecrypt[i]).unicode()^QChar(kEncryptorString[keyIndex(i)]).unicode()));
        }
        return resultString;
    }

int Crypt:: keyIndex(int index)
    {
        int len = kEncryptorString.length();
        int multiple = index / len;
        if ( index >=  len ) {
            return index - (len * multiple);
        }
        return index;
    }

