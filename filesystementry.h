#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

#include <QtCore>

class FileSystemEntry
{

public:
    enum FsType{fslocal = 0, fsremote, fsnone};

    FileSystemEntry(QString name = 0);
    ~FileSystemEntry() {}

    QString getName() const;

    FsType getType() const;
    void setType(const FsType &value);

    QString getHost() const;
    void setHost(const QString &value);

    QString getUser() const;
    void setUser(const QString &value);

    QString getRemote() const;
    void setRemote(const QString &value);

    QString getMount() const;
    void setMount(const QString &value);

    QString getId_rsa() const;
    void setId_rsa(const QString &value);

    QString getOptions() const;
    void setOptions(const QString &value);

    bool getIsReadOnly() const;
    void setIsReadOnly(bool value);

    bool getShowHidden() const;
    void setShowHidden(bool value);

    bool getShowSymlink() const;
    void setShowSymlink(bool value);

    void setMode(const QString &value);

    QDir::Filters getFilterRoot() const;
    QDir::Filters getFilterBase() const;

private:
    QString name;
    FsType type;
    QString host;
    QString user;
    QString remote;
    QString mount;
    QString id_rsa;
    QString options;
    bool isReadOnly;
    bool showHidden;
    bool showSymlink;

    QDir::Filters filterRoot;
    QDir::Filters filterBase;
};

#endif // FILESYSTEMENTRY_H
