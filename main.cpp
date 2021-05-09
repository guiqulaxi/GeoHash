#include <QCoreApplication>
#include "Geohash.h"
#include <QDebug>
#include <string>
#include <vector>
#include <QFile>
#include <QTextStream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CGeoHash g;
    std::string  s=g.Encode(30,120,5);
    GeoCoord coord= g.Decode(s);
    std::vector<std::string> neighbors;
    g.GetNeighbors(s,neighbors);
    for(int i=0;i<50;i+=5)
    {
        GeoCoord  rect=g.GetGeoRect(30,120,5);
         qDebug()<<(rect.latitude-rect.north);
    }
    GeoRect rect;
    rect.north=26;
    rect.west=119;
    rect.east=123;
    rect.south=22;
    std::vector<std::string> hashes;
    g.GetRectGeoHashes(rect,hashes,4);
    QFile f("/home/sun/location.csv");
    f.open(QIODevice::WriteOnly);
    QTextStream out(&f);
    for(const std::string &hash:hashes)
    {
        GeoCoord  coord=g.Decode(hash);
        out<<QString("%1,%2,%3,%4\n").arg(coord.north).arg(coord.west).arg(coord.south).arg(coord.east);
    }
    f.close();

    return a.exec();
    //,
}
