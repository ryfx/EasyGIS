#ifndef MAPTILESPROVIDER_H
#define MAPTILESPROVIDER_H

#include <QtNetwork>
#include <QtGui>

class MapTilesProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ getServerAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(QString tileFileType READ getTileFileType WRITE setTileFileType NOTIFY tileFileTypeChanged)
    Q_PROPERTY(DataSource dataSource READ getDataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(QString mapType READ getMapType WRITE setMapType NOTIFY mapTypeChanged)

    Q_ENUMS(DataSource)
//    Q_ENUMS(MapType)

public:
    explicit MapTilesProvider(QObject *parent = 0);

    //bool loadTile(const QString& tileName);
    QImage* getTile(const QString& tileName, bool getDirectPath = false);

//    enum MapType {
//        NormalMap, ScenicRouteMap, SuggestedRouteMap
//    };

    enum DataSource {
        NetworkData, LocalData
    };

    QString getMapType() const;
    void setMapType(QString mapType);
    DataSource getDataSource() const;
    void setDataSource(DataSource dataSource);
    QString getServerAddress() const;
    void setServerAddress(const QString& newServerAddress);
    QString getTileFileType() const;
    void setTileFileType(const QString& newTileFileType);

private:
    QNetworkAccessManager nam;
    QCache<QString, QImage> imgCache;

    QString m_serverAddress;
    QString m_tileFileType;

    DataSource m_dataSource;
    QString m_mapType;

signals:
    void readyTile(QString path);
    void readyImage(QString path);

    void mapTypeChanged(QString newMapType);
    void serverAddressChanged(QString newServerAddress);
    void tileFileTypeChanged(QString newTileFileType);
    void dataSourceChanged(DataSource newDataSource);

private slots:
    void slotFinished(QNetworkReply* reply);

};

#endif // MAPTILESPROVIDER_H
