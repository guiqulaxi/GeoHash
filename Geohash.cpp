#include "Geohash.h"
#include <set>
#include <stack>
#include <QDebug>

int CGeoHash::IndexForChar(char c, const std::string & str) {
    int index = -1;
    for (int i = 0; i < str.size(); i++) {
        if (c == str[i]) {
            index = i;
            break;
        }
    }
    return index;
}

std::string CGeoHash::GetNeighbor(const std::string & hash, Direction direction) {
    int hash_length = hash.size();
    char last_char = hash[hash_length - 1];
    int is_odd = hash_length % 2;
    const char ** border = is_odd ? odd_borders : even_borders;
    const char ** neighbor = is_odd ? odd_neighbors : even_neighbors;
    std::string base = hash.substr(0, hash_length - 1);
    if (IndexForChar(last_char, border[direction]) != -1) {
        base = GetNeighbor(base, direction);
    }
    int neighbor_index = IndexForChar(last_char, neighbor[direction]);
    last_char = char_map[neighbor_index];
    base.push_back(last_char);
    return base;
}
void CGeoHash::GetNeighbors(const std::string & hash, std::vector<std::string>&rs)
{
    rs.clear();
    if (!hash.empty()) {
        // N, NE, E, SE, S, SW, W, NW
        rs.push_back(GetNeighbor(hash, NORTH));
        rs.push_back(GetNeighbor(rs.at(0), EAST));
        rs.push_back(GetNeighbor(hash, EAST));
        rs.push_back(GetNeighbor(rs.at(2), SOUTH));
        rs.push_back(GetNeighbor(hash, SOUTH));
        rs.push_back(GetNeighbor(rs.at(4), WEST));
        rs.push_back(GetNeighbor(hash, WEST));
        rs.push_back(GetNeighbor(rs.at(6), NORTH));
    }
}
CGeoHash::CGeoHash()
{
    //默认精度为20
    Init(20);
}

CGeoHash::CGeoHash(int nPrecision)
{
    Init(nPrecision);
}

CGeoHash::~CGeoHash()
{

}

void CGeoHash::Init(int nPrecision)
{



    for(int i=1;i<nPrecision;i++)
    {
        GeoCoord rect= GetGeoRect(0,0,i);
        GeoError error;
        error.latitudeError=rect.north-rect.latitude;
        error.longitudeError=rect.longitude-rect.west;
        error.distanceError=GetDistance(rect.latitude,
                                        rect.longitude,
                                        rect.north,
                                        rect.west);
        m_mapError[i]=error;
    }
}




double CGeoHash::Deg2Rad(double ang)
{
    return ang / D_R;
}

double CGeoHash::Rad2Deg(double ang)
{
    return ang * D_R;
}


std::string CGeoHash::Encode(double lat, double lng, int precision) {
  if (precision < 1 || precision > 12) precision = 6;
  std::string hash;
  if (lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {
    precision *= 5;
    Interval lat_interval = {MAX_LAT, MIN_LAT};
    Interval lng_interval = {MAX_LONG, MIN_LONG};
    Interval * interval;
    double coord = 0;
    double mid = 0;
    int is_even = 1;
    unsigned int hashChar = 0;
    for (int i = 1; i <= precision; i++) {
      if (is_even) {
        interval = &lng_interval;
        coord = lng;
      } else {
        interval = &lat_interval;
        coord = lat;
      }
      mid = (interval->low + interval->high) / 2.0;
      hashChar = hashChar << 1;
      if (coord > mid) {
        interval->low = mid;
        hashChar |= 0x01;
      } else {
        interval->high = mid;
      }
      if (!(i % 5)) {
        hash.push_back(char_map[hashChar]);
        hashChar = 0;
      }
      is_even = !is_even;
    }
  }
  return hash;
}

GeoCoord CGeoHash::GetGeoRect(double dLatitude, double dLongitude, int nPrecision)
{
    if(nPrecision<1)
    {
        GeoCoord  coord={0,0,MAX_LAT,MAX_LONG,MIN_LAT,MIN_LONG,0,0};
        return coord;
    }

    Encode(dLatitude,dLongitude,nPrecision);
    return  Decode(Encode(dLatitude,dLongitude,nPrecision));
}


double CGeoHash::GetDistance(double dLatitude, double dLongitude, double dsLatitude, double dsLongitude)
{
    double radlat1 = M_PI*dLatitude/180.0f;
    double radlat2 = M_PI*dsLatitude/180.0f;

    //now long.
    double radlong1 = M_PI*dLongitude/180.0f;
    double radlong2 = M_PI*dsLongitude/180.0f;

    if( radlat1 < 0 ) radlat1 = M_PI/2 + fabs(radlat1);// south
    if( radlat1 > 0 ) radlat1 = M_PI/2 - fabs(radlat1);// north
    if( radlong1 < 0 ) radlong1 = M_PI*2 - fabs(radlong1);//west
    if( radlat2 < 0 ) radlat2 = M_PI/2 + fabs(radlat2);// south
    if( radlat2 > 0 ) radlat2 = M_PI/2 - fabs(radlat2);// north
    if( radlong2 < 0 ) radlong2 = M_PI*2 - fabs(radlong2);// west

    double x1 = EARTH_RADIUS_IN_METERS * cos(radlong1) * sin(radlat1);
    double y1 = EARTH_RADIUS_IN_METERS * sin(radlong1) * sin(radlat1);
    double z1 = EARTH_RADIUS_IN_METERS * cos(radlat1);
    double x2 = EARTH_RADIUS_IN_METERS * cos(radlong2) * sin(radlat2);
    double y2 = EARTH_RADIUS_IN_METERS * sin(radlong2) * sin(radlat2);
    double z2 = EARTH_RADIUS_IN_METERS * cos(radlat2);
    double d = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

    double theta = acos((EARTH_RADIUS_IN_METERS*EARTH_RADIUS_IN_METERS+EARTH_RADIUS_IN_METERS*EARTH_RADIUS_IN_METERS-d*d) / (2* EARTH_RADIUS_IN_METERS * EARTH_RADIUS_IN_METERS));
    double dist  = theta * EARTH_RADIUS_IN_METERS;

    return dist;
}

GeoRect CGeoHash::GetBoundingBox(double dLatitude, double dLongitude, double dRadiusMeters)
{
    GeoRect objGeoRect;

    objGeoRect.west = dLongitude - Rad2Deg(dRadiusMeters/EARTH_RADIUS_IN_METERS/cos(Deg2Rad(dLatitude)));
    objGeoRect.east = dLongitude + Rad2Deg(dRadiusMeters/EARTH_RADIUS_IN_METERS/cos(Deg2Rad(dLatitude)));
    objGeoRect.north  = dLatitude - Rad2Deg(dRadiusMeters/EARTH_RADIUS_IN_METERS);
    objGeoRect.south  = dLatitude + Rad2Deg(dRadiusMeters/EARTH_RADIUS_IN_METERS);

    return objGeoRect;
}

GeoBoxDimension CGeoHash::GeohashDimensionsForPrecision(int precision) {
  GeoBoxDimension dimensions = {0.0, 0.0};
  if (precision > 0) {
    int lat_times_to_cut = precision * 5 / 2;
    int lng_times_to_cut = precision * 5 / 2 + (precision % 2 ? 1 : 0);
    double width = 360.0;
    double height = 180.0;
    for (int i = 0; i < lat_times_to_cut; i++) height /= 2.0;
    for (int i = 0; i < lng_times_to_cut; i++) width /= 2.0;
    dimensions.width = width;
    dimensions.height = height;
  }
  return dimensions;
}
GeoCoord CGeoHash::Decode(const std::string & hash) {
  GeoCoord coordinate = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  if (!hash.empty()) {
    if (!hash.empty()) {
      unsigned int char_mapIndex;
      struct Interval lat_interval = {MAX_LAT, MIN_LAT};
      struct Interval lng_interval = {MAX_LONG, MIN_LONG};
      struct Interval * interval;
      int is_even = 1;
      for (int i = 0; i < hash.size(); i++) {
        char_mapIndex = IndexForChar(hash[i], char_map);
        if (char_mapIndex < 0) break;
        // Interpret the last 5 bits of the integer
        for (int j = 0; j < 5; j++) {
          interval = is_even ? &lng_interval : &lat_interval;
          double delta = (interval->high - interval->low) / 2.0;
          if ((char_mapIndex << j) & 0x0010)
            interval->low += delta;
          else
            interval->high -= delta;
          is_even = !is_even;
        }
      }
      coordinate.latitude = lat_interval.high -
          ((lat_interval.high - lat_interval.low) / 2.0);
      coordinate.longitude = lng_interval.high -
          ((lng_interval.high - lng_interval.low) / 2.0);
      coordinate.north = lat_interval.high;
      coordinate.east = lng_interval.high;
      coordinate.south = lat_interval.low;
      coordinate.west = lng_interval.low;
    }
  }
  return coordinate;
}


void CGeoHash::GetRectGeoHashes(const GeoRect geoRect,std::vector<std::string>& hashes,int precision)
{
    double centerlng=(geoRect.west+geoRect.east)/2;
    double centerlat=(geoRect.south+geoRect.north)/2;

    GeoCoord coord=GetGeoRect(centerlat,centerlng,0);
    bool isContained=false;
    for(int i=1;i<=precision;i++)
    {
         coord=GetGeoRect(centerlat,centerlng,i);
         GeoRect rect;
         rect.north =coord.north;
         rect.east=coord.east;
         rect.south=coord.south;
         rect.west=coord.west;

         if(geoRect.Contains(rect))
         {
              hashes.push_back(Encode(coord.latitude,coord.longitude,i));
              GeoRect eastrect,southrect,westrect,northrect;

              westrect.north=geoRect.north;
              westrect.east=rect.west;
              westrect.south=rect.south;
              westrect.west=geoRect.west;
              GetRectGeoHashes(westrect,hashes,precision);
              qDebug()<<westrect.toString().c_str();
              southrect.north=rect.south;
              southrect.east=rect.east;
              southrect.south=geoRect.south;
              southrect.west=geoRect.west;
              GetRectGeoHashes(southrect,hashes,precision);
              qDebug()<<southrect.toString().c_str();
              eastrect.north=rect.north;
              eastrect.east=geoRect.east;
              eastrect.south=geoRect.south;
              eastrect.west=rect.east;
               GetRectGeoHashes(eastrect,hashes,precision);
              qDebug()<<eastrect.toString().c_str();
              northrect.north=geoRect.north;
              northrect.east=geoRect.east;
              northrect.south=rect.north;
              northrect.west=rect.west;
              GetRectGeoHashes(northrect,hashes,precision);
              qDebug()<<northrect.toString().c_str();
              isContained=true;
               break;
         }


    }
    if(!isContained)
    {
       //hashes.push_back(Encode(coord.latitude,coord.longitude,precision));
    }

}

void CGeoHash::GetRectGeoHashes1(const GeoRect geoRect,std::vector<std::string>& hashes,int precision)
{
    GeoCoord coord=GetGeoRect(geoRect.south,geoRect.west,precision);
    GeoRect rect;
    rect.north =coord.north;
    rect.east=coord.east;
    rect.south=coord.south;
    rect.west=coord.west;
    double steplng=rect.east- rect.west;
    double steplat=rect.north- rect.south;
    std::string geohash=Encode(coord.latitude,coord.longitude,precision);
    std::string prevWestSouthGeohash=geohash;
   for(rect.east=coord.east, rect.west=coord.west;rect.east<=geoRect.east;rect.east+=steplng,rect.west+=steplng)
   {
        std::string prevSouthGeohash=prevWestSouthGeohash;

        for(rect.north =coord.north, rect.south=coord.south;rect.north<=geoRect.north;rect.north+=steplat,rect.south+=steplat)
        {
            std::string hash=GetNeighbor(prevSouthGeohash,NORTH);
            hashes.push_back(hash);
            prevSouthGeohash=hash;
        }

        prevWestSouthGeohash=GetNeighbor(prevWestSouthGeohash,EAST);
    }



}
