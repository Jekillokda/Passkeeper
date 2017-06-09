#include <time.h>
#include <QString>

#include "DataAccess.h"
#include "Crypt.h"
DataAccess::DataAccess(const QString &path,const QString &password)
{
    this->path = path;
    this->password = password;
    file = new QFile(path);
}

DataAccess::~DataAccess()
{
    delete file;
}

errorCode DataAccess::read(QList< QVector< QString> > &list)
{
    if(!file->open(QIODevice::ReadOnly))
        return FILE_ERROR;
    QVector< QString > tempVector(4);
    file->seek(0);
    struct header head;
    if(!file->read((char*)&head, sizeof(header)))
        return FILE_ERROR;
    if(head.id[0] == 'P' && head.id[1] == 'A' && head.id[2] == 'S')
    {
      struct cryptedHeader chead;
      if(!file->read((char*)&chead, sizeof(cryptedHeader)))
        return FILE_ERROR;
      if(!(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S'))
        return INVALID_PASSWORD;

        for(int i = 0; i < head.entriescount; i++)
        {
            Crypt crypt;
            struct entryHeader e;
            if(!file->read((char*)&e, sizeof(entryHeader)))
                return FILE_ERROR;

            char *entry = (char*)malloc(e.entryLength);
            if(!file->read(entry, e.entryLength))
                return FILE_ERROR;

            char temp[500];
            int start = 0;
            int c1=e.nameLength;
            int c2=c1+e.urlLength;
            int c3=c2+e.loginLength;
            int c4=c3+e.passwordLength;

            QString detemp,strn;
            memcpy(temp, &entry[start], c4);
            temp[e.entryLength] = 0;
            strn=QString(temp);
            detemp=crypt.decryptString(strn);

            QStringRef name(&detemp, 0,c1);
            QStringRef url(&detemp, c1,c2-c1);
            QStringRef login(&detemp, c2,c3-c2);
            QStringRef password(&detemp, c3,c4-c3);
            tempVector[0] = name.toString();
            tempVector[1] = url.toString();
            tempVector[2] = login.toString();
            tempVector[3] = password.toString();

            list.append(tempVector);
            free(entry);
        }
    }
    file->close();
    return SUCCESS;
}

errorCode DataAccess::write(const QList< QVector< QString> > &data)
{
    if(!file->open(QIODevice::Truncate | QIODevice::WriteOnly))
        return FILE_ERROR;
    struct header head;

    head.id[0] = 'P';
    head.id[1] = 'A';
    head.id[2] = 'S';
    head.entriescount = data.count();

    srand(time(0));

    if( file->write((char*)&head, sizeof(header)) == -1 )
        return FILE_ERROR;

    struct cryptedHeader chead;
    chead.id[0] = 'P';
    chead.id[1] = 'A';
    chead.id[2] = 'S';
    if( file->write((char*)&chead, sizeof(cryptedHeader)) == -1 )
        return FILE_ERROR;

    for(int i = 0; i < data.count(); i++)
    {

        struct entryHeader e;
        e.nameLength = data[i][0].toUtf8().size();
        e.urlLength = data[i][1].toUtf8().size();
        e.loginLength = data[i][2].toUtf8().size();
        e.passwordLength = data[i][3].toUtf8().size();
        e.entryLength = e.nameLength + e.urlLength + e.loginLength + e.passwordLength;
        e.entryLength = ((e.entryLength/16)+1)*16;
        char *entry = (char*)malloc(e.entryLength);
        int start = 0;
        for(int j = 0; j < 4; j++)
        {
            memcpy(entry+start, data[i][j].toUtf8().constData(), data[i][j].toUtf8().size());
            start += data[i][j].toUtf8().size();
        }             
        Crypt crypt;
        int j=e.entryLength;
        QString centry=crypt.cryptString(QString::fromLatin1(entry,e.entryLength),j);
        QByteArray inBytes;
        const char *Str;
        inBytes = centry.toUtf8();
        Str = inBytes.constData();
        entry=strdup(Str);
        int length = e.entryLength;
        if( file->write((char*)&e, sizeof(entryHeader)) == -1 )
            return FILE_ERROR;
        if( file->write(entry, length) == -1 )
            return FILE_ERROR;
         free(entry);
    }
    file->close();
    return SUCCESS;
}

void DataAccess::setPassword(const QString &password)
{
    this->password = password;
}

QString DataAccess::getPassword()
{
    return this->password;
}
