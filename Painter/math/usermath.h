#ifndef USERMATH_H
#define USERMATH_H

#include <QPoint>
#include <math.h>
#include "mathStruct.h"
#include <QList>

#define PI 3.14159265
/* TEST
double a = UserMath.Distance(0, 0, 0, 1, 2, 3);//根号14
Console.WriteLine(string.Format("a：" +  a + "\r\n"));

double b = UserMath.GetDiatanceBetweenPoint(new Point(1, 1), new Point(2, 2));//根号2
Console.WriteLine(string.Format("b：" + b + "\r\n"));

double c = UserMath.getDistanceFormPointToLine(3, 2, 1,0,0);//3
Console.WriteLine(string.Format("c：" + c + "\r\n"));

Point d = UserMath.getFootOfPerpendicular(3, 2, 1, 0, 0);//(0.2)
Console.WriteLine(string.Format("d：" + d.X + " " +d.Y + "\r\n"));
*/



class UserMath
{
public:
    /// <summary>
    /// 计算点(x,y,z)与点(a,b,c),两点之间的距离
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <param name="c"></param>
    /// <reurns></returns>
    static double Distance(double x, double y, double z, double a, double b, double c)
    {
        double x1 = x;
        double y1 = y;
        double z1 = z;

        double x2 = a;
        double y2 = b;
        double z2 = c;

        double result = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2) + pow((z1 - z2), 2));
        return result;
    }

    /// <summary>
    /// 求两点之间的距离
    /// </summary>
    /// <param name="startPoint"></param>
    /// <param name="endPoint"></param>
    /// <returns></returns>
    static double GetDiatanceBetweenPoint(QPoint startPoint, QPoint endPoint)
    {
        double distance;

        distance = sqrt(abs(startPoint.x() - endPoint.x()) * abs(startPoint.x() - endPoint.x()) + abs(startPoint.y() - endPoint.y()) * abs(startPoint.y() - endPoint.y()));

        return distance;
    }

    /// <summary>
    /// 求点（x1,y1）与直线Ax+By+C = 0的距离
    /// </summary>
    /// <returns></returns>
    static double getDistanceFormPointToLine(int x1, int y1, double A, double B, double C)
    {
        double distance = 0;
        double numerator = A * x1 + B * y1 + C;
        double denominator = A * A + B * B;
        distance = (abs(numerator) / sqrt(denominator));
        return distance;
    }

    /// <summary>
    /// 求点（x1,y1）与直线Ax+By+C = 0的距离
    /// </summary>
    /// <returns></returns>
    static double getDistanceFormPointToLine(QPoint P, double A, double B, double C)
    {
        double distance = 0;
        double numerator = A * P.x() + B * P.y() + C;
        double denominator = A * A + B * B;
        distance = (abs(numerator) / sqrt(denominator));
        return distance;
    }

    /// <summary>
    /// 求点（x1,y1）与直线Ax+By+C = 0的交点坐标(垂足坐标)
    /// </summary>
    /// <returns></returns>
    static QPoint getFootOfPerpendicular(int x1, int y1, double A, double B, double C)
    {
        if (A * A + B * B < 1e-13)
        {
            return QPoint(0,0);
        }

        if (abs(A * x1 + B * y1 + C) < 1e-13)
        {
            return QPoint(x1, y1);
        }
        else
        {
            int newX = (int)((B * B * x1 - A * B * y1 - A * C) / (A * A + B * B));
            int newY = (int)((-A * B * x1 + A * A * y1 - B * C) / (A * A + B * B));
            return QPoint(newX, newY);
        }
    }

    /// <summary>
    /// 求点P（x1,y1）与直线Ax+By+C = 0的交点坐标(垂足坐标)  y = bx + a ; ==> bx-y+a =0
    /// </summary>
    /// <returns></returns>
    static QPoint getFootOfPerpendicular(QPoint P, double A, double B, double C)
    {
        if (A * A + B * B < 1e-13)
            return QPoint(0, 0);

        if (abs(A * P.x() + B * P.y() + C) < 1e-13)
        {
            return QPoint(P.x(), P.y());
        }
        else
        {
            int newX = (int)((B * B * P.x() - A * B * P.y() - A * C) / (A * A + B * B));
            int newY = (int)((-A * B * P.x() + A * A * P.y() - B * C) / (A * A + B * B));
            return QPoint(newX, newY);
        }
    }

    /// <summary>
    /// 将点拟合成直线
    /// </summary>
    /// <param name="point_select"></param>
    /// <returns></returns>
    static line_struct lineCalculate(QList<QPoint> &point_select)
    {
        QList<int> pointX;
        QList<int> pointY;
        float ArrPoint_X[400];
        float ArrPoint_Y[400];

        int ArrPoint_Count = 0;
        pointX.clear();
        pointY.clear();

        for (int i = 0; i < point_select.length(); i++)   //  边缘检测
        {
            pointX.append(point_select[i].x());
            pointY.append(point_select[i].y());
            ArrPoint_X[ArrPoint_Count] = point_select[i].x();
            ArrPoint_Y[ArrPoint_Count] = point_select[i].y();
            ArrPoint_Count++;
        }


        float averagex = 0, averagey = 0;
        for (int i=0;i<pointX.count();i++)
        {
            averagex += pointX[i];
        }
        for (int j=0;j<pointY.count();j++)
        {
            averagey += pointY[j];
        }
        averagex /= pointX.count();        //  取X坐标的平均数
        averagey /= pointY.count();        //  取Y坐标的平均数

        //经验回归系数的分子与分母
        float numerator = 0;
        float denominator = 0;
        for (int i = 0; i < pointX.count(); i++)
        {
            numerator += (ArrPoint_X[i] - averagex) * (ArrPoint_Y[i] - averagey);
            denominator += (ArrPoint_X[i] - averagex) * (ArrPoint_X[i] - averagex);
        }
        //回归系数b（Regression Coefficient）   y = bx + a ; ==> bx-y+a =0
        float RCB = numerator / denominator;
        //回归系数a
        float RCA = averagey - RCB * averagex;
        double angle = 180 * atan(RCB) / PI;

        line_struct param;
        param.RCB = RCB;
        param.RCA = RCA;
        param.angle = angle;
        return param;
    }

    /// <summary>
    /// 将点拟合成直线
    /// </summary>
    /// <param name="point_select"></param>
    /// <returns></returns>
    static line_struct lineCalculate(QList<QPoint> point_select)
    {
        QList<int> pointX;
        QList<int> pointY;
        float ArrPoint_X[400];
        float ArrPoint_Y[400];

        int ArrPoint_Count = 0;
        pointX.clear();
        pointY.clear();

        for (int i = 0; i < point_select.count(); i++)   //  边缘检测
        {
            pointX.append(point_select[i].x());
            pointY.append(point_select[i].y());
            ArrPoint_X[ArrPoint_Count] = point_select[i].x();
            ArrPoint_Y[ArrPoint_Count] = point_select[i].y();
            ArrPoint_Count++;
        }

        float averagex = 0, averagey = 0;
        for (int i=0;i<pointX.count();i++)
        {
            averagex += pointX[i];
        }
        for (int j=0;j<pointY.count();j++)
        {
            averagey += pointY[j];
        }
        averagex /= pointX.count();        //  取X坐标的平均数
        averagey /= pointY.count();        //  取Y坐标的平均数

        //经验回归系数的分子与分母
        float numerator = 0;
        float denominator = 0;
        for (int i = 0; i < pointX.count(); i++)
        {
            //numerator += (ArrPoint_X[i] - averagex) * (ArrPoint_Y[i] - averagey);
            //denominator += (ArrPoint_X[i] - averagex) * (ArrPoint_X[i] - averagex);
            numerator += (ArrPoint_X[i] * ArrPoint_Y[i]) - (averagex * averagey);
            denominator += (ArrPoint_X[i] * ArrPoint_X[i]) - (averagex * averagex);
        }
        //回归系数b（Regression Coefficient）   y = bx + a ; ==> bx-y+a =0
        float RCB = numerator / denominator;
        //回归系数a
        float RCA = averagey - RCB * averagex;
        double angle = 180 * atan(RCB) / PI;

        line_struct param;
        param.RCB = RCB;
        param.RCA = RCA;
        param.angle = angle;
        return param;
    }
        //拟合效果 相关系数：分子Cov(X,Y)协方差，分母deta标准差
        //r = Cov(X,Y)/(deta_X * deta_Y)
        //[0.75,1] 正相关很强
        //[-1,-0.75] 负相关很强
        //[-0.25,0.25]相关性较弱
        //相关的分子与分母
        //double relative_num = 0;
        //double relative_den = 0;
        //double r;
        //for (int i = 0; i<pointX.Count; i++)
        //{
        //    relative_num += (ArrPoint_X[i] - averagex) * (ArrPoint_Y[i] - averagey);
        //    relative_den += Math.Pow((ArrPoint_X[i] - averagex),2.0) * Math.Pow((ArrPoint_X[i] - averagex),2.0);
        //}
        //relative_den = Math.Sqrt(relative_den);
        //r = relative_num / relative_den;

    /// <summary>
    /// 两点构成直线方程  y = bx + a ; ==> bx-y+a =0
    /// </summary>
    /// <param name="point_select"></param>
    /// <returns></returns>
    static line_struct TwoPointToLine(QPoint startPoint, QPoint endPoint)
    {
        //斜率k=(y1-y2)/(x1-x2)
        float RCB = (float)(endPoint.y() - startPoint.y() ) / (endPoint.x() -startPoint.x());
        //回归系数a   y = bx + a => a = y - bx
        float RCA = startPoint.y() - RCB * startPoint.x();
        double angle = 180 * atan(RCB) / PI;

        line_struct param;
        param.RCB = RCB;
        param.RCA = RCA;
        param.angle = angle;
        return param;
    }

    /// <summary>
    /// 计算俩点之间的中点坐标
    /// </summary>
    /// <param name="startpoint"></param>
    /// <param name="endpoint"></param>
    /// <returns></returns>
    static QPoint GetMiddlePoint(QPoint startpoint,QPoint endpoint)
    {
        int tempX = (int)(startpoint.x() + endpoint.x()) / 2;
        int tempY = (int)(startpoint.y() + endpoint.y()) / 2;
        QPoint middlePoint(tempX, tempY);
        return middlePoint;
    }


    /// <summary>
    /// 获取参考点的坐标和方向
    /// </summary>
    /// <param name="startPoint"></param>
    /// <param name="endPoint"></param>
    /// <returns></returns>
    static DirectionPoint GetReferentPoint(QPoint startPoint, QPoint endPoint)
    {
        DirectionPoint directionPoint;
        line_struct a = TwoPointToLine(startPoint, endPoint);
        directionPoint.point = startPoint;
        directionPoint.angle = a.angle;
        return directionPoint;
    }

    /// <summary>
    /// 获取middle参考点的坐标和方向
    /// </summary>
    /// <param name="startPoint"></param>
    /// <param name="endPoint"></param>
    /// <returns></returns>
    static DirectionPoint GetMiddleReferentPoint(QPoint startPoint, QPoint endPoint)
    {
        DirectionPoint directionPoint;
        line_struct a = TwoPointToLine(startPoint, endPoint);
        directionPoint.point = GetMiddlePoint(startPoint,endPoint);
        directionPoint.angle = a.angle;
        return directionPoint;
    }
};


#endif // USERMATH_H
