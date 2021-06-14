#include <QCoreApplication>
#include "Geohash.h"
#include <QDebug>
#include <string>
#include <vector>
#include <QFile>
#include <QTextStream>
#include<QTime>
#include "LocationTest.h"
#include "TireTree.h"

void testGeohash()
{
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
    QTime time;
    time.start();
    g.GetRectGeoHashes(rect,hashes,4);
    int timeElapsed = time.elapsed();
    QFile f("/home/sun/location.csv");
    f.open(QIODevice::WriteOnly);
    QTextStream out(&f);
    for(const std::string &hash:hashes)
    {
        GeoCoord  coord=g.Decode(hash);
        out<<QString("%1,%2,%3,%4\n").arg(coord.north).arg(coord.west).arg(coord.south).arg(coord.east);
    }
    f.close();
}



void testTireTree()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+ qrand());
    CGeoHash g;
    TireTree<std::string> tt;
    std::vector<std::string> data;
    for(int i=0;i<100000;i++)
    {
        double lat=-90+qrand()/(double)(RAND_MAX/(180));
        double lng=-180+qrand()/(double)(RAND_MAX/(360));;
        std::string&& str=g.Encode(lat,lng,9);
        data.emplace_back(str);
    }

    std::string suf=data[0];
    {
        QTime time;
        time.start();
        for(size_t i=0;i<data.size();i++)
        {
            tt.add(data[i],data[i]);
        }
        for(int re=0;re<1000;re++)
        {
        std::vector<std::string> v;
        tt.get(suf,v,false);
        }
        qDebug()<<time.elapsed()/1000.0<<"s";
    }

    {
        QTime time;
        time.start();

        for(int re=0;re<1000;re++)
        {
            std::vector<std::string> v;
            for(size_t i=0;i<data.size();i++)
            {
                if(data[i].find(suf.c_str(),0,suf.size())!=std::string::npos)
                {
                    v.push_back(data[i]);
                }
            }
        }

        qDebug()<<time.elapsed()/1000.0<<"s";
    }


}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //LocationTest l;
    //l.findRect(26,119,123,22);

    //l.findRect1(26,119,123,22);
    testTireTree();

    return a.exec();
    //,
}
