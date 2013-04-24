/*============================================================
 * 地图瓦片提供模块
 * 功能 ： 根据地图显示模块的需求，从网络上获取瓦片数据，供其使用，
 *        并具备缓存机制，自动将获取到的数据保存在缓存中，下次取用时无需再次联网获取
 * 模块重要性： 重要
 *
 * 作者 ： 刘梓懿
 *
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 * 2013.3.14 从 MapTilesFrame 中独立出该模块
 * 2013.3.16 修正几个内存泄漏问题
 *
 ============================================================*/

#include "maptilesprovider.h"

/*============================================================
 * MapTilesProvider 构造函数
 * 功能说明 ： 初始化所需的变量，并连接下载完成的信号到我们自定义的槽函数
 *
 * 函数重要性： 重要
 *
 * 参数说明 ： 指向父对象的指针（Qt 默认参数）
 * 返回值  :  无
 * 作者    ： 刘梓懿
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 ============================================================*/

MapTilesProvider::MapTilesProvider(QObject *parent) :
    QObject(parent)
{
    setMapType("/NormalMap/");
    setDataSource(LocalData);
    setServerAddress(QCoreApplication::applicationDirPath() + "/离线地图包/");
    setTileFileType(".png");

    connect(&nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(slotFinished(QNetworkReply*)));

}


/*============================================================
 * 瓦片调取函数
 * 功能说明 ： 尝试从缓存中加载地图显示模块所需的瓦片，并返回其指针
 *           如果不存在
 * 函数重要性： 重要
 *
 * 参数说明 ： 新的视口坐标
 *           （视口坐标为视口的左上角点坐标，以服务器坐标系为基准）
 * 返回值  :  无
 * 作者    ： 刘梓懿
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 ============================================================*/
QImage *MapTilesProvider::getTile(const QString &tileName, bool getDirectPath)
{
    //Note: QImage 使用了Implicit Data Sharing，拷贝一份QImage对象速度很快

    QString path;

    if(getDirectPath)
    {
        path = getServerAddress() + tileName;
    }
    else
    {
        path = getServerAddress() + getMapType() + tileName + getTileFileType();
    }

    qDebug() << "path: " << path;

    if(imgCache[path] != NULL)
    {
        qDebug() << "tile is CACHED" << path;
        return new QImage(*imgCache[path]);
    }
    else
    {
        if(getDataSource() == NetworkData)
        {
            QNetworkRequest request;
            request.setUrl(QUrl(path));
            request.setAttribute(QNetworkRequest::User, path);
            request.setAttribute(QNetworkRequest::UserMax, getDirectPath);

            qDebug() << "tile is NOT CACHED, sending request: " << request.url().toString();

            nam.get(request);

            return NULL;
        }
        else
        {
            qDebug() << "reading local tile file " << path;
            QImage *img = new QImage(path);
            if(img->isNull())
            {
                qDebug() << "file not exist or corrupted";
                return NULL;
            }
            else
            {
                if(!imgCache.insert(path, img))
                {
                    qDebug() << "ERROR!!! failed to insert local file " << path << " into cache";
                    qDebug() << "Cache size at this time" << imgCache.size();
                }
                return new QImage(*img);
            }
        }

    }
}

//QImage *MapTilesProvider::getImage(const QString &imagePath)
//{
//    //Note: QImage 使用了Implicit Data Sharing，拷贝一份QImage对象速度很快

//    QString path(getServerAddress() + imagePath);
//    qDebug() << "path: " << path;

//    if(imgCache[path] != NULL)
//    {
//        qDebug() << "tile is CACHED" << path;
//        return new QImage(*imgCache[path]);
//    }
//    else
//    {
//        if(getDataSource() == NetworkData)
//        {
//            QNetworkRequest request;
//            request.setUrl(QUrl(path));
//            request.setAttribute(QNetworkRequest::User, path);

//            qDebug() << "tile is NOT CACHED, sending request: " << request.url().toString();

//            nam.get(request);

//            return NULL;
//        }
//        else
//        {
//            qDebug() << "reading local tile file " << path;
//            QImage *img = new QImage(path);
//            if(img->isNull())
//            {
//                qDebug() << "file not exist or corrupted";
//                return NULL;
//            }
//            else
//            {
//                if(!imgCache.insert(path, img))
//                {
//                    qDebug() << "ERROR!!! failed to insert local file " << path << " into cache";
//                    qDebug() << "Cache size at this time" << imgCache.size();
//                }
//                return new QImage(*img);
//            }
//        }

//    }
//}

QString MapTilesProvider::getMapType() const
{
    return m_mapType;
}

void MapTilesProvider::setMapType(QString mapType)
{
    m_mapType = mapType;
    emit mapTypeChanged(mapType);
}

MapTilesProvider::DataSource MapTilesProvider::getDataSource() const
{
    return m_dataSource;
}

void MapTilesProvider::setDataSource(MapTilesProvider::DataSource dataSource)
{
    m_dataSource = dataSource;
    emit dataSourceChanged(dataSource);
}

QString MapTilesProvider::getServerAddress() const
{
    return QString(m_serverAddress);
}

void MapTilesProvider::setServerAddress(const QString &newServerAddress)
{
    if(!QUrl(newServerAddress).isValid())
    {
        //qDebug() << "NOT A VALID newServerAddress!";
        return;
    }
    else
    {
        m_serverAddress = newServerAddress;
        emit serverAddressChanged(m_serverAddress);
    }
}

QString MapTilesProvider::getTileFileType() const
{
    return QString(m_tileFileType);
}

void MapTilesProvider::setTileFileType(const QString &newTileFileType)
{
    m_tileFileType = newTileFileType;
    emit tileFileTypeChanged(m_tileFileType);
}

void MapTilesProvider::slotFinished(QNetworkReply *reply)
{
    QString path = reply->request().attribute(QNetworkRequest::User).toString();
    bool getDirectPath = reply->request().attribute(QNetworkRequest::UserMax).toBool();

    qDebug() << "slotFinished called for tile: " << path;

    if(reply->error())
    {
        qDebug() << "network reply ERROR for tile: " << path;
        return;
    }

    QImage *replyImg = new QImage(QImage::fromData(reply->readAll()));

    //判断网络传回的图片是否损坏
    if(replyImg->isNull())
    {
        qDebug() << "reply data is NOT A VALID IMAGE for tile: " << path;
        return;
    }
    else
    {
        if(!imgCache.insert(path, replyImg))
        {
            qDebug() << "ERROR!!! failed to insert " << path << " into cache";
            qDebug() << "Cache size at this time" << imgCache.size();
            return;
        }
    }

    //QImage *tileImg = new QImage(*replyImg);

    if(getDirectPath)
    {
        emit readyImage(path);
    }
    else
    {
        emit readyTile(path);
    }
}
