#include "filedownloader.h"
#include "dbmanager.h"


FileDownloader::FileDownloader(QUrl imageUrl, int id, QObject *parent) :
 QObject(parent)
{
    m_id = id;
    m_path = QString::number(id)+".jpg";
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this,SLOT(fileDownloaded(QNetworkReply*)));
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    DbManager db("data.db");
    m_DownloadedData = pReply->readAll();
    bool ok = db.setGameImage(m_id,m_DownloadedData);
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
