#include "Geohash.h"
#include "LocationTest.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>
#include<QString>
#include <QVariant>
#include <QtDebug>
LocationTest::LocationTest()
{

}
void initRand()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime())+ qrand());
}

double rand(double min,double max)
{
    return min+qrand()/(double)(RAND_MAX/(max-min));
}

void  LocationTest::initDataBase()
{
         QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("location.db");
        if (!database.open())
        {
          return;
        }

        QSqlQuery query(database);
        query.exec("DROP TABLE data");    //删除名为data
        query.exec("CREATE TABLE data ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "name VARCHAR(40) NOT NULL, "
                           "lng DOUBLE NOT NULL, "
                           "lat DOUBLE NOT NULL, "
                           "geohash DOUBLE VARCHAR(12) NOT NULL) "
                    );
        initRand();
        database.transaction();
        CGeoHash g;


        for(int i=0;i<1000000;i++)
        {
            double lat=rand(-90,90);
            double lng=rand(-180,180);
            QString insertSql("INSERT INTO data (name,lng,lat,geohash)VALUES ('%1',%2,%3,'%4')");
           insertSql= insertSql.arg(QString::number(i)).arg(lng).arg(lat).arg(g.Encode(lat,lng,6).c_str());
            query.exec(insertSql);
            if(0==i%10000)
            {
              qDebug()<<i;

            }
        }
        database.commit();
        QString indexlnglat="CREATE INDEX lng_lat_index ON data (lng,lat)";
        query.exec(indexlnglat);
        QString indexgeohash="CREATE INDEX geohash_index ON data (geohash)";
        query.exec(indexgeohash);
        database.close();
//        QString selectSql = "SELECT name,lng,lat,geohash FROM data";
//        if(!query.exec(selectSql))
//        {
//            qDebug()<<query.lastError();
//        }
//        else
//        {
////            while(query.next())
////            {
////                QString name= query.value(0).toString();
////                double lng = query.value(1).toDouble();
////                double lat = query.value(2).toDouble();
////                QString geohash = query.value(3).toString();
////                qDebug()<<QString("%1,%2,%3,%4").arg(name).arg(lng).arg(lat).arg(geohash);
////            }
//        }
}

void  LocationTest::findRect(double north ,double west,double east,double south)
{
   QSqlDatabase database;
   database = QSqlDatabase::addDatabase("QSQLITE");
   database.setDatabaseName("location.db");
   if (!database.open())
   {
     return;
   }
     CGeoHash g;
     std::vector<std::string> hashes;
     GeoRect rect;
     rect.north=north;
     rect.west=west;
     rect.east=east;
     rect.south=south;
     g.GetRectGeoHashes(rect,hashes,4);

     QString querySql("SELECT * FROM data where ");
     for(int i=0;i<hashes.size();i++)
     {
         QString hash(hashes[i].c_str());
         hash.append("%");
         if(0==i)
         {
           QString cond=QString(" geohash LIKE '%1'").arg(hash);
           querySql.append(cond);
         }else
         {
           QString cond=QString(" or geohash LIKE '%1'").arg(hash);
           querySql.append(cond);
         }
     }
     //qDebug()<<querySql;
      QSqlQuery query(database);
     query.exec(querySql);
     database.close();
}
void  LocationTest::findRect1(double north, double west, double east, double south)
{
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("location.db");
    if (!database.open())
    {
      return;
    }
    QString querySql=QString("SELECT * FROM data WHERE ( lng BETWEEN %1 AND %2 ) "
                     "AND  ( lat BETWEEN %3 AND %4 )").arg(west).arg(east).arg(south).arg(north);
    QSqlQuery query(database);
    query.exec(querySql);
    database.close();
}
