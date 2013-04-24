#include "graphics.h"


Graphics::Graphics(QString startName,QString endName)
{
    this->startName = startName;
    this->endName = endName;
    initeNode();
}
Graphics::Graphics(QPoint sp, QPoint ep)
{
    QSqlQuery query;
    QString str;
    QString str1;
    str.setNum(sp.x());
    str1.setNum(sp.y());

    if(!query.exec("SELECT name FROM interestNode WHERE CoordinateX="+str+" AND CoordinateY="+str1))
        qDebug() << "Error,Open failed!";;
    query.next();
    this->startName = query.value(0).toString();
    qDebug() << this->startName;
    str.setNum(ep.x());
    str1.setNum(ep.y());
    if(!query.exec("SELECT name FROM interestNode WHERE CoordinateX="+str+" AND CoordinateY="+str1))
        qDebug() << "Error,Open failed!";;
    query.next();
    this->endName = query.value(0).toString();
    qDebug() << this->endName;
    initeNode();
}

void Graphics::initeNode()
{
    int a;
    int b;
    int c;
    QSqlQuery query;
    query.exec("select count(id) from interestNode");
    while(query.next())
    {
       maxVertexNum = (query.value(0).toInt());
    }

    int echo;

    arcs = new int*[maxVertexNum];
    for(a = 0; a < maxVertexNum; a++)
    {
        arcs[a] = new int[maxVertexNum];
    }

    shortestPath = new int*[maxVertexNum];
    for(b = 0; b < maxVertexNum; b++)
    {
        shortestPath[b] = new int[maxVertexNum];
    }

    router = new int*[maxVertexNum];

    shortestRoute = new int[maxVertexNum];//只保存最短路径

    for(c = 0; c < maxVertexNum; c++)
    {
        router[c] = new int[maxVertexNum];
    }

    order = new int*[maxVertexNum];
    for(c = 0; c < maxVertexNum; c++)
    {
        order[c] = new int[2];
    }


    id = new int[maxVertexNum];

    this->vexnum = maxVertexNum;
    query.exec("select max(id) from Node");
    while(query.next())
    {
       echo = (query.value(0).toInt());
       this->arcnum = (echo/100) + 1;
    }

    for( a = 0; a < this->arcnum; a++ )
    {
       node.append(-1);
    }
    for( a=0; a < maxVertexNum ; a++)
    {
        for( b=0; b < maxVertexNum ; b++)
        {
            arcs[a][b] = 65535;
            shortestPath[a][b] = 65535;
            if(a == b)
            {
                arcs[a][b] = 0;
                shortestPath[a][b] = 0;
            }

        }

    }
    for(a = 0; a < maxVertexNum; a++ )
    {
        for( b = 0; b < maxVertexNum; b++)
        {
            router[a][b] = -1;
        }
        this->shortestRoute[a] = -1;//初始化指定最短路径数组
    }

    int idNumber = 0;
    query.exec("select startId from Node group by start_endId order by id");
    while(query.next())
    {
       node[idNumber].startId = (query.value(0).toInt());
       idNumber++;
    }

    idNumber = 0;
    query.exec("select endId from Node group by start_endId order by id");
    while(query.next())
    {
       node[idNumber].endId = (query.value(0).toInt());
       idNumber++;
    }

    query.exec("select id from interestNode where name='"+this->startName+"'");
    query.next();
    id[0] = query.value(0).toInt();
    query.exec("select id from interestNode where name <>'"+this->startName+"'");
    idNumber = 1;
    qDebug() << id[0]<< "&";
    while(query.next())
    {
       id[idNumber] = (query.value(0).toInt());
       qDebug() << id[idNumber]<< "&";
       idNumber++;
    }
    int i = 0;
    QString str;
    for( a = 0; a < this->arcnum; a++ )
    {
        str.setNum(i);
        query.exec("select max(id) from Node where (id/100) = 0" + str);
        while(query.next())
        {
             node[a].routerNumber = ((query.value(0).toInt())%100);
        }
        i++;
    }
    int *orderFirst = new int[this->arcnum];
    int *orderSecond = new int[this->arcnum];

    for( a = 0; a < this->arcnum; a++ )
    {
        orderFirst[a] = 0;
        orderSecond[a] = 0;
    }

    for( a = 0; a < this->arcnum; a++ )
    {
        for( b = 0; b < maxVertexNum ;b++)
        {
           if(node[a].startId == id[b])
           {
              orderFirst[a] = b;
              continue;
           }
        }
     }

    for( a = 0; a < this->arcnum; a++ )
    {
        for( b = 0; b < maxVertexNum ;b++)
        {
           if(node[a].endId == id[b])
           {
              orderSecond[a] = b;
              continue;
           }
        }
     }

    for( a = 0; a < this->arcnum; a++ )
    {
            arcs[orderFirst[a]][orderSecond[a]] = node[a].routerNumber;
            arcs[orderSecond[a]][orderFirst[a]] = node[a].routerNumber;
    }
}

void Graphics::findShortestPath()
{
    QSqlQuery query;
    query.exec("select count(id) from interestNode");
    while(query.next())
    {
       maxVertexNum = (query.value(0).toInt());
    }

    int *sPointsList = new int[maxVertexNum];
    for(int a = 0;a < maxVertexNum;a++)
    {
        sPointsList[a] = -1;//最短路径点集合
    }
    int pointsList = maxVertexNum;
    int pointLocation = 0;//表示每个点位置，0为第一个点,为关键点
    sPointsList[0] = 0;
    router[0][0] = 0;
    while( pointsList>0 )
    {
        int flag = 0;//flag就是邻接点的
        int minFlag = 0;
        int min = 65535;
        int tmp;
        while( flag<maxVertexNum )
        {
            if(sPointsList[flag] == -1)//等于-1就是说该位置的最短路径点还未被加入，可以考虑
            {
                if( arcs[pointLocation][flag]<min )
                {
                    min = arcs[pointLocation][flag];
                    minFlag = flag;
                }
                if( arcs[pointLocation][flag]!=65535 )
                {
                    tmp = 0;
                    while( router[flag][tmp]!=-1 )
                        tmp++;
                    if( router[flag][tmp-1]!=flag )
                    {
                        for( tmp=0; router[pointLocation][tmp]!=-1; tmp++)
                            //添加起始点,也就是将此关键点之前的最短路径复制到flag邻接点的最短路径，下面就会进行flag点的添加
                            router[flag][tmp] = router[pointLocation][tmp];
                        router[flag][tmp] = flag;//添加flag点，这两个点是相连的
                    }
                }
                if( shortestPath[0][flag]>(shortestPath[0][pointLocation]+arcs[pointLocation][flag]) )
                {
                    if( shortestPath[0][flag]!=65535 )
                    {
                        tmp = 0;
                        while( router[flag][tmp]!=-1 )
                            tmp++;
                        router[flag][tmp] = router[flag][tmp-1];
                        router[flag][tmp-1] = pointLocation;
                    }
                    shortestPath[0][flag] = shortestPath[0][pointLocation]+arcs[pointLocation][flag];
                    shortestPath[flag][0] = shortestPath[0][pointLocation]+arcs[pointLocation][flag];
                }
                if( shortestPath[0][pointLocation]>(shortestPath[0][flag]+arcs[flag][pointLocation]))
                {
                    if( shortestPath[0][flag]!=65535 )
                    {
                        tmp = 0;
                        while( router[pointLocation][tmp]!=-1 )
                            tmp++;
                        router[pointLocation][tmp] = router[pointLocation][tmp-1];
                        router[pointLocation][tmp-1] = flag;
                    }
                    shortestPath[0][pointLocation] = shortestPath[0][flag]+arcs[flag][pointLocation];
                    shortestPath[pointLocation][0] = shortestPath[0][flag]+arcs[flag][pointLocation];
                }
                shortestPath[pointLocation][flag] = arcs[pointLocation][flag];//先将最短距离值存入最短距离数组中
            }
            flag++;
        }
        pointLocation = minFlag;//转到最短的距离的点
        sPointsList[pointLocation] = pointLocation;//将最短路径点加入到最短路径点集合中
        pointsList--;
    }
    query.exec("select id from interestNode where name='"+this->endName+"'");
    query.next();
    for(int a=0; a<this->maxVertexNum; a++)
    {
        if(query.value(0).toInt()==id[a])
        {
            this->shortestRoute = this->router[a];
            break;
        }
    }
}


tagNode::tagNode(int i)
{
   startId = i;
}
