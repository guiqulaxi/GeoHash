#include "Util.h"

/**
 * @brief 转化为弧度(rad)
 * @param d
 * @return
 */
double rad(double d)
{
    return d*M_PI/180.0;
}
/**
* @brief 求两经纬度距离
 *
* @param lon1
*            第一点的经度
* @param lat1
*            第一点的纬度
* @param lon2
*            第二点的经度
* @param lat2
*           第二点的纬度
* @return 两点距离，单位m
* */
double distance1(double lon1, double lat1, double lon2,double lat2) {
    double r1 = rad(lat1);
    double r2 = rad(lon1);
    double a = rad(lat2);
    double b = rad(lon2);
    double s = acos(cos(r1) * cos(a) * cos(r2 - b)
                    + sin(r1) * sin(a))
            * EARTH_RADIUS;
    return s;
}
double distance2(double lon1, double lat1, double lon2,
                 double lat2) {
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lon1) - rad(lon2);
    double s = 2 * asin(sqrt(pow(sin(a / 2), 2)
                             + cos(radLat1) * cos(radLat2)
                             * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS;
    return s;
}

/**
       * 求两经纬度距离
       *
       * @param lon1
       *            第一点的经度
       * @param lat1
       *            第一点的纬度
       * @param lon2
       *            第二点的经度
       * @param lat2
       *            第二点的纬度
       * @return 两点距离，单位m
       * */
 double distance3(double lon1, double lat1,
                        double lon2, double lat2) {
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double radLon1 = rad(lon1);
    double radLon2 = rad(lon2);
    if (radLat1 < 0)
        radLat1 = M_PI / 2 + abs(radLat1);// south
    if (radLat1 > 0)
        radLat1 = M_PI / 2 - abs(radLat1);// north
    if (radLon1 < 0)
        radLon1 = M_PI * 2 - abs(radLon1);// west
    if (radLat2 < 0)
        radLat2 = M_PI / 2 + abs(radLat2);// south
    if (radLat2 > 0)
        radLat2 = M_PI / 2 - abs(radLat2);// north
    if (radLon2 < 0)
        radLon2 = M_PI * 2 - abs(radLon2);// west
    double x1 = cos(radLon1) * sin(radLat1);
    double y1 = sin(radLon1) * sin(radLat1);
    double z1 = cos(radLat1);

    double x2 = cos(radLon2) * sin(radLat2);
    double y2 = sin(radLon2) * sin(radLat2);
    double z2 = cos(radLat2);

    double d = pow((x1 - x2), 2) + pow((y1 - y2), 2)
            + pow((z1 - z2), 2);
    // // 余弦定理求夹角
    // double theta = acos((2 - d) / 2);

    d = pow(EARTH_RADIUS, 2) * d;
    // //余弦定理求夹角
    double theta = acos((2 * pow(EARTH_RADIUS, 2) - d)
                        / (2 * pow(EARTH_RADIUS, 2)));

    double dist = theta * EARTH_RADIUS;
    return dist;
}

/**
       * 求两经纬度方向角
       *
       * @param lon1
       *            第一点的经度
       * @param lat1
       *            第一点的纬度
       * @param lon2
       *            第二点的经度
       * @param lat2
       *            第二点的纬度
       * @return 方位角，角度（单位：°）
       * */
double azimuth(double lon1, double lat1, double lon2,double lat2) {
    lat1 = rad(lat1);
    lat2 = rad(lat2);
    lon1 = rad(lon1);
    lon2 = rad(lon2);
    double azimuth = sin(lat1) * sin(lat2) + cos(lat1)
            * cos(lat2) * cos(lon2 - lon1);
    azimuth = sqrt(1 - azimuth * azimuth);
    azimuth = cos(lat2) * sin(lon2 - lon1) / azimuth;
    azimuth = asin(azimuth) * 180 / M_PI;
    if (isnan(azimuth)) {
        if (lon1 < lon2) {
            azimuth = 90.0;
        } else {
            azimuth = 270.0;
        }
    }
    return azimuth;
}



/**
       * 已知一点经纬度A，和与另一点B的距离和方位角，求B的经纬度
       *
       * @param lon1
       *            A的经度
       * @param lat1
       *            A的纬度
       * @param distance
       *            AB距离（单位：米）
       * @param azimuth
       *            AB方位角
       * @return B的经纬度
       * */
void polar(double lon1, double lat1,
           double distance, double azimuth,double &lon,double& lat) {
    azimuth = rad(azimuth);
    // 将距离转换成经度的计算公式
    lon = lon1 + (distance * sin(azimuth))
            / (EARTH_ARC * cos(rad(lat1)));
    // 将距离转换成纬度的计算公式
    lat = lat1 + (distance * cos(azimuth)) / EARTH_ARC;
}
