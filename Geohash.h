#include <stdio.h>
#include <string>
#include <math.h>
#include <map>
#include <vector>

#define D_R (M_PI / 180.0)
const double EARTH_RADIUS_IN_METERS = 6372797.560856;

//实现grohash算法的过程
//add by freeeyes

// geohash精度的设定参考 http://en.wikipedia.org/wiki/Geohash
// geohash	length	lat bits	lng bits	lat error	lng error	km error
// 1		2			3					±23				±23			±2500
// 2		5			5					± 2.8			± 5.6		±630
// 3		7			8					± 0.70		    ± 0.7		±78
// 4		10			10				    ± 0.087		    ± 0.18		±20
// 5		12			13				    ± 0.022		    ± 0.022		±2.4
// 6		15			15				    ± 0.0027	    ± 0.0055	±0.61
// 7		17			18				    ±0.00068	    ±0.00068	±0.076
// 8		20			20				   ±0.000085	    ±0.00017	±0.019



#define MAX_LAT             90.0
#define MIN_LAT             -90.0

#define MAX_LONG            180.0
#define MIN_LONG            -180.0

// Normal 32 characer map used for geohashing
static char char_map[] = "0123456789bcdefghjkmnpqrstuvwxyz";

// The follow character maps were created by Dave Troy and used
// in his Javascript Geohashing library.
// http://github.com/davetroy/geohash-js
static const char * even_neighbors[] = {
    "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
    "bc01fg45238967deuvhjyznpkmstqrwx",
    "14365h7k9dcfesgujnmqp0r2twvyx8zb",
    "238967debc01fg45kmstqrwxuvhjyznp"
};
static const char * odd_neighbors[] = {
    "bc01fg45238967deuvhjyznpkmstqrwx",
    "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
    "238967debc01fg45kmstqrwxuvhjyznp",
    "14365h7k9dcfesgujnmqp0r2twvyx8zb"
};
static const char * even_borders[] = {"prxz", "bcfguvyz", "028b", "0145hjnp"};
static const char * odd_borders[] = {"bcfguvyz", "prxz", "0145hjnp", "028b"};


struct GeoRect
{
    double north;
    double east;
    double south;
    double west;

    GeoRect()
    {
         north=0.0;
         east=0.0;
         south=0.0;
         west=0.0;
    }
    bool Contains(const  GeoRect& other) const
    {
        if(north>=other.north&&
                east>=other.east&&
                south<=other.south  &&
                west<=other.west )
        {
            return  true;
        }
        else
        {
            return false;
        }
    }
    std::string toString()
    {
         std::string str;
         str.append(std::to_string(north));
         str.append(",");
         str.append(std::to_string(east));
         str.append(",");
         str.append(std::to_string(south));
         str.append(",");
         str.append(std::to_string(west));
         return str;
    }
};



struct GeoError{
    double latitudeError;
    double longitudeError;
    double distanceError;
    GeoError()
    {
        latitudeError=0.0;
        longitudeError=0.0;
        distanceError=0.0;
    }
};


struct Interval {

    double high;
    double low;

} ;

struct GeoBoxDimension {

    double height;
    double width;

} ;

struct GeoCoord {

    double latitude;
    double longitude;

    double north;
    double east;
    double south;
    double west;

    GeoBoxDimension dimension;

} ;

class CGeoHash
{
public:

    enum Direction
    {
        NORTH=  0,
        EAST =  1,
        SOUTH  =  2,
        WEST   =  3

    };
    CGeoHash();
    CGeoHash(int nPrecision);
    ~CGeoHash();

    //得到GeoHash计算字符串

    std::string Encode(double dLatitude, double dLongitude, int nPrecision);
   GeoCoord Decode(const std::string & hash);
    //得到当前的矩形
    GeoCoord GetGeoRect(double dLatitude, double dLongitude, int nPrecision);

    //得到当前geohash的相邻矩形
    void GetNeighbors(const std::string & hash,std::vector<std::string>&rs);

    std::string GetNeighbor(const std::string & hash, Direction direction);

    //计算任意两点间的距离
    double GetDistance(double dLatitude, double dLongitude, double dsLatitude, double dsLongitude);

    //得到指定半径区域的矩形区域
    GeoRect GetBoundingBox(double dLatitude, double dLongitude, double dRadiusMeters);

    //获得指定经度下经纬度距离误差
    GeoError GetLatitudeError(int nPrecision){return m_mapError[nPrecision];}
    //获得指定精度下geohash盒子长宽 单位 度经度纬度
    GeoBoxDimension GeohashDimensionsForPrecision(int precision);
     //获得指定矩形中的Geohash precision最大精度
    void GetRectGeoHashes(const GeoRect geoRect,std::vector<std::string>& hashes,int precision);
    void GetRectGeoHashes1(const GeoRect geoRect,std::vector<std::string>& hashes,int precision);

private:
    void Init(int nPrecision);
    //获得Geohash的经度或者纬度二进制pData，返回经度或者纬度的范围
    double Deg2Rad(double ang);
    double Rad2Deg(double ang);
    int IndexForChar(char c, const std::string & str);

private:
    std::map<int,GeoError> m_mapError;



};
