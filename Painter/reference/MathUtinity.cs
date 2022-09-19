using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/* TEST
double a = UserMath.Distance(0, 0, 0, 1, 2, 3);//根号14
Console.WriteLine(string.Format("a：" +  a + "\r\n"));

double b = UserMath.GetDiatanceBetweenPoint(new Point(1, 1), new Point(2, 2));//根号2
Console.WriteLine(string.Format("b：" + b + "\r\n"));
*/

namespace WinFormMainApplication
{
    public class MathUtinity
    {
        /// <summary>
        /// 计算点(x,y,z)与点(a,b,c),两点之间的距离
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="z"></param>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="c"></param>
        /// <returns></returns>
        public static double Distance(double x, double y, double z, double a, double b, double c)
        {
            double x1 = x;
            double y1 = y;
            double z1 = z;

            double x2 = a;
            double y2 = b;
            double z2 = c;

            double result = Math.Sqrt(Math.Pow((x1 - x2), 2) + Math.Pow((y1 - y2), 2) + Math.Pow((z1 - z2), 2));
            return result;
        }

        /// <summary>
        /// 求两点之间的距离
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        /// <returns></returns>
        public static double GetDiatanceBetweenPoint(Point startPoint, Point endPoint)
        {
            double distance;
            distance = Math.Sqrt(Math.Abs(startPoint.X - endPoint.X) * Math.Abs(startPoint.X - endPoint.X) + Math.Abs(startPoint.Y - endPoint.Y) * Math.Abs(startPoint.Y - endPoint.Y));
            return distance;
        }

        /// <summary>
        /// 求两点之间的中点坐标
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        /// <returns></returns>
        public static Point GetMiddlePointBetweenPoint(Point startPoint, Point endPoint)
        {
            Point middlePoint = new Point();
            middlePoint.X = (startPoint.X + endPoint.X)/2;
            middlePoint.Y = (startPoint.Y + endPoint.Y)/2;
            return middlePoint;
        }


        /// <summary>
        /// 两点构成直线方程  y = bx + a ; ==> bx-y+a =0
        /// </summary>
        /// <param name="point_select"></param>
        /// <returns></returns>
        public static line_struct TwoPointToLine(Point startPoint, Point endPoint)
        {
            //斜率k=(y1-y2)/(x1-x2)
            float RCB = (float)(endPoint.Y - startPoint.Y) / (endPoint.X - startPoint.X);
            //回归系数a   y = bx + a => a = y - bx
            float RCA = startPoint.Y - RCB * startPoint.X;
            double angle = 180 * Math.Atan(RCB) / Math.PI;

            line_struct param = new line_struct();
            param.RCB = RCB;
            param.RCA = RCA;
            param.angle = angle;
            return param;
        }


        /// <summary>
        /// 获取参考点的坐标和方向,俩点中间的中点
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        /// <returns></returns>
        public static DirectionPoint GetReferentPoint(Point startPoint, Point endPoint)
        {
            DirectionPoint directionPoint = new DirectionPoint();
            line_struct a = TwoPointToLine(startPoint, endPoint);
            directionPoint.point = GetMiddlePointBetweenPoint(startPoint, endPoint);
            directionPoint.angle = a.angle;
            return directionPoint;
        }

    }
}
