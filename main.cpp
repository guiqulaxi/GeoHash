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
        //qDebug()<<(rect.latitude-rect.north);
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
void fun(TrieTree<std::string>& tree,const std::string& key,int num)
{
   if(num==0)
   {
       tree.insert(key,key);
       return;
   }
   for(char i='a';i<='z';i++)
   {
      fun(tree,key+i,num-1);
   }
}
void testTireTree()
{
   TrieTree<std::string> tree;
   fun(tree,"",4);
   //tree.remove("aba");
   std::unordered_map<std::string,std::string> values;
   std::string v;
   tree.prefix("a",values);
   int a=0;
}


int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
//    LocationTest l;
//    {
//        QTime time;
//        time.start();
//        l.findRect(26,119,123,22);
//        qDebug()<<time.elapsed()/1000.0<<"s";
//    }
//    {
//        QTime time;
//        time.start();
//        l.findRect1(26,119,123,22);
//        qDebug()<<time.elapsed()/1000.0<<"s";
//    }
     testTireTree();

   // return a.exec();
    //,
}
