#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QtNetwork>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

signals:

public slots:
    static void downloadImageFileToPath(QString remotePath,QString localPath);
private:
};

#endif // DOWNLOADER_H
