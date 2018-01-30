#include "configxmlreader.h"

ConfigXmlReader::ConfigXmlReader(QString fileName, QObject *parent) : QObject(parent)
{
    this->fileName = fileName;
    fileSystemEntryList.clear();
}

QList<FileSystemEntry *> ConfigXmlReader::getFileSystemEntryList()
{
    fileSystemEntryList.clear();
    file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly))
    {
        return fileSystemEntryList;
    }

    reader = new QXmlStreamReader;
    reader->setDevice(file);

    while (!reader->atEnd() && !reader->hasError())
    {
        QXmlStreamReader::TokenType token = reader->readNext();
        if (token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        if (token == QXmlStreamReader::StartElement)
        {
            if (reader->name() == "stations")
            {
//                qDebug() << "Чтение конфигурации <stations> node";
            }

            if (reader->name() == "entry")
            {
  //              qDebug() << "Чтение конфигурации <entry> node";

                FileSystemEntry *fileSystemEntry = new FileSystemEntry(reader->attributes().value("name").toString());

                QString type = reader->attributes().value("type").toString();

                if (type == "local")
                {
                    fileSystemEntry->setType(FileSystemEntry::fslocal);
                }
                else if (type == "net")
                {
                    fileSystemEntry->setType(FileSystemEntry::fsremote);
                }

                fileSystemEntry->setHost(reader->attributes().value("host").toString());
                fileSystemEntry->setUser(reader->attributes().value("user").toString());
                fileSystemEntry->setRemote(reader->attributes().value("remote").toString());
                fileSystemEntry->setMount(reader->attributes().value("mount").toString());
                fileSystemEntry->setId_rsa(reader->attributes().value("id_rsa").toString());
                fileSystemEntry->setOptions(reader->attributes().value("options").toString());
                fileSystemEntry->setMode(reader->attributes().value("mode").toString());

                fileSystemEntryList.append(fileSystemEntry);

            }

        }
    }
    return fileSystemEntryList;
}
