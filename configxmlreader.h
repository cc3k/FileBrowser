#ifndef CONFIGXMLREADER_H
#define CONFIGXMLREADER_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include <QDebug>

#include "filesystementry.h"

class ConfigXmlReader : public QObject
{
    Q_OBJECT
public:
    explicit ConfigXmlReader(QString fileName, QObject *parent = 0);
    ~ConfigXmlReader() {}

    QList<FileSystemEntry *> getFileSystemEntryList();

private:
    QString fileName;
    QFile *file;
    QXmlStreamReader *reader;

    QList<FileSystemEntry *> fileSystemEntryList;


signals:

public slots:
};

#endif // CONFIGXMLREADER_H
