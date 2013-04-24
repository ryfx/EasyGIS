#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtGui>
#include <QtSql>

typedef struct tagNode
{
    int startId;
    int endId;
    int routerNumber;
    //int **order;
    tagNode(int i = 0);

}Node;

class Graphics
{
public:
    Graphics(QString startName,QString endName);
    Graphics(QPoint sp,QPoint ep);

    QList<Node> node;

    void initeNode();
    void findShortestPath();

    int **arcs;
    int **shortestPath;
    int **router;
    int *id;
    int **order;

    int maxVertexNum;
    int *shortestRoute;

private:
    int vexnum;
    int arcnum;
    QString startName;
    QString endName;

};

#endif // GRAPHICS_H
