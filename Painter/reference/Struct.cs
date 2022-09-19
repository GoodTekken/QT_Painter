using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormMainApplication
{
    public struct WALL_Struct
    {
        public int id;
        public int startPointX;
        public int startPointY;
        public int endPointX;
        public int endPointY;

        public int LS2000StartPointX;
        public int LS2000StartPointY;
        public int LS2000EndPointX;
        public int LS2000EndPointY;

        public float RCA;
        public float RCB;
        public double angle;
    }

    struct Object_Coordinate
    {
        public double left_X;
        public double left_Y;
        public double left_Z;

        public double right_X;
        public double right_Y;
        public double right_Z;

        public double center_X;
        public double center_Y;
        public double center_Z;
    }

    //"车辆ID：{0},到达检测:{1},路径申请:{2},路径ID:{3},X坐标:{4},Y坐标:{5}，角度:{6}"
    struct T_VehicleMessaageFromGP
    {
        public int vehicle_ID;
        public int vehicle_Arrive;
        public int vehicle_Req;
        public int vehicle_ReqSegID;
        public int vehicle_X;
        public int vehicle_Y;
        public int vehicle_Angle;
    }

    public struct line_struct
    {
        public float RCA;
        public float RCB;
        public double angle;
    }

    public struct TrailerPoint_Struct
    {
        public double ls2000X;
        public double ls2000Y;
        public double ls2000Angle;
        public double X;
        public double Y;
        public double angle;
    }

    public struct DirectionPoint
    {
        public Point point;
        public double angle;
    }

    /// <summary>
    /// point:由-90度逆时针旋转到90度，换算为平面直角坐标系
    /// length:由扫描仪换取回来的原始数据
    /// index:索引
    /// angle:对应于LS2000的扫描角度
    /// distanceToLastpoint:与前一个点的距离
    /// isGoodPoint：判断是否为质量好的点
    /// label:用于记录导航墙的ID；
    /// </summary>
    public struct PointDetailStruct
    {
        public int index;
        public int length;
        public double angle;
        public Point point;
        public double distanceToLastpoint;
        public bool isGoodPoint;
        public int label;
    }
}
