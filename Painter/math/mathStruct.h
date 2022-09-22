#ifndef MATHSTRUCT_H
#define MATHSTRUCT_H

#include <QPoint>


struct WALL_Struct
{
public:
    int id;
    int startPointX;
    int startPointY;
    int endPointX;
    int endPointY;

    int LS2000StartPointX;
    int LS2000StartPointY;
    int LS2000EndPointX;
    int LS2000EndPointY;

    float RCA;
    float RCB;
    double angle;
};

struct Object_Coordinate
{
public:
    double left_X;
    double left_Y;
    double left_Z;

    double right_X;
    double right_Y;
    double right_Z;

    double center_X;
    double center_Y;
    double center_Z;
};

//"车辆ID：{0},到达检测:{1},路径申请:{2},路径ID:{3},X坐标:{4},Y坐标:{5}，角度:{6}"
struct T_VehicleMessaageFromGP
{
public:
    int vehicle_ID;
    int vehicle_Arrive;
    int vehicle_Req;
    int vehicle_ReqSegID;
    int vehicle_X;
    int vehicle_Y;
    int vehicle_Angle;
};

struct line_struct
{
public:
    float RCA;
    float RCB;
    double angle;
};

struct TrailerPoint_Struct
{
public:
    double ls2000X;
    double ls2000Y;
    double ls2000Angle;
    double X;
    double Y;
    double angle;
};

struct DirectionPoint
{
public:
    QPoint point;
    double angle;
};

/// <summary>
/// point:由-90度逆时针旋转到90度，换算为平面直角坐标系
/// length:由扫描仪换取回来的原始数据
/// index:索引
/// angle:对应于LS2000的扫描角度
/// distanceToLastpoint:与前一个点的距离
/// isGoodPoint：判断是否为质量好的点
/// label:用于记录导航墙的ID；
/// </summary>
struct PointDetailStruct
{
public:
    int index;
    int length;
    double angle;
    QPoint point;
    double distanceToLastpoint;
    bool isGoodPoint;
    int label;
};

struct CoorStruct
{
public:
    int x;
    int y;
    float angle;
};



#endif // MATHSTRUCT_H
