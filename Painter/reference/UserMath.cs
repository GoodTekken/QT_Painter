using System;
using System.Collections;
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

double c = UserMath.getDistanceFormPointToLine(3, 2, 1,0,0);//3
Console.WriteLine(string.Format("c：" + c + "\r\n"));

Point d = UserMath.getFootOfPerpendicular(3, 2, 1, 0, 0);//(0.2)
Console.WriteLine(string.Format("d：" + d.X + " " +d.Y + "\r\n"));
*/


namespace WinFormMainApplication
{
    public static class UserMath
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
        /// 求点（x1,y1）与直线Ax+By+C = 0的距离
        /// </summary>
        /// <returns></returns>
        public static double getDistanceFormPointToLine(int x1, int y1, double A, double B, double C)
        {
            double distance = 0;
            double numerator = A * x1 + B * y1 + C;
            double denominator = A * A + B * B;
            distance = (Math.Abs(numerator) / Math.Sqrt(denominator));
            return distance;
        }

        /// <summary>
        /// 求点（x1,y1）与直线Ax+By+C = 0的距离
        /// </summary>
        /// <returns></returns>
        public static double getDistanceFormPointToLine(Point P, double A, double B, double C)
        {
            double distance = 0;
            double numerator = A * P.X + B * P.Y + C;
            double denominator = A * A + B * B;
            distance = (Math.Abs(numerator) / Math.Sqrt(denominator));
            return distance;
        }

        /// <summary>
        /// 求点（x1,y1）与直线Ax+By+C = 0的交点坐标(垂足坐标)
        /// </summary>
        /// <returns></returns>
        public static Point getFootOfPerpendicular(int x1, int y1, double A, double B, double C)
        {
            if (A * A + B * B < 1e-13)
                return new Point(0, 0);

            if (Math.Abs(A * x1 + B * y1 + C) < 1e-13)
            {
                return new Point(x1, y1);
            }
            else
            {
                int newX = (int)((B * B * x1 - A * B * y1 - A * C) / (A * A + B * B));
                int newY = (int)((-A * B * x1 + A * A * y1 - B * C) / (A * A + B * B));
                return new Point(newX, newY);
            }
        }

        /// <summary>
        /// 求点P（x1,y1）与直线Ax+By+C = 0的交点坐标(垂足坐标)  y = bx + a ; ==> bx-y+a =0
        /// </summary>
        /// <returns></returns>
        public static Point getFootOfPerpendicular(Point P, double A, double B, double C)
        {
            if (A * A + B * B < 1e-13)
                return new Point(0, 0);

            if (Math.Abs(A * P.X + B * P.Y + C) < 1e-13)
            {
                return new Point(P.X, P.Y);
            }
            else
            {
                int newX = (int)((B * B * P.X - A * B * P.Y - A * C) / (A * A + B * B));
                int newY = (int)((-A * B * P.X + A * A * P.Y - B * C) / (A * A + B * B));
                return new Point(newX, newY);
            }
        }

        /// <summary>
        /// 将点拟合成直线
        /// </summary>
        /// <param name="point_select"></param>
        /// <returns></returns>
        public static line_struct lineCalculate(Point[] point_select)
        {
            ArrayList pointX = new ArrayList();
            ArrayList pointY = new ArrayList();
            float[] ArrPoint_X = new float[400];
            float[] ArrPoint_Y = new float[400];

            int ArrPoint_Count = 0;
            pointX.Clear();
            pointY.Clear();
            try
            {
                for (int i = 0; i < point_select.Length; i++)   //  边缘检测
                {
                    pointX.Add(point_select[i].X);
                    pointY.Add(point_select[i].Y);
                    ArrPoint_X[ArrPoint_Count] = point_select[i].X;
                    ArrPoint_Y[ArrPoint_Count] = point_select[i].Y;
                    ArrPoint_Count++;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            float averagex = 0, averagey = 0;
            foreach (int i in pointX)
            {
                averagex += i;
            }
            foreach (int j in pointY)
            {
                averagey += j;
            }
            averagex /= pointX.Count;        //  取X坐标的平均数
            averagey /= pointY.Count;        //  取Y坐标的平均数

            //经验回归系数的分子与分母
            float numerator = 0;
            float denominator = 0;
            for (int i = 0; i < pointX.Count; i++)
            {
                numerator += (ArrPoint_X[i] - averagex) * (ArrPoint_Y[i] - averagey);
                denominator += (ArrPoint_X[i] - averagex) * (ArrPoint_X[i] - averagex);
            }
            //回归系数b（Regression Coefficient）   y = bx + a ; ==> bx-y+a =0
            float RCB = numerator / denominator;
            //回归系数a
            float RCA = averagey - RCB * averagex;
            double angle = 180 * Math.Atan(RCB) / Math.PI;

            line_struct param = new line_struct();
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
        public static line_struct lineCalculate(List<Point> point_select)
        {
            ArrayList pointX = new ArrayList();
            ArrayList pointY = new ArrayList();
            float[] ArrPoint_X = new float[400];
            float[] ArrPoint_Y = new float[400];

            int ArrPoint_Count = 0;
            pointX.Clear();
            pointY.Clear();
            try
            {
                for (int i = 0; i < point_select.Count; i++)   //  边缘检测
                {
                    pointX.Add(point_select[i].X);
                    pointY.Add(point_select[i].Y);
                    ArrPoint_X[ArrPoint_Count] = point_select[i].X;
                    ArrPoint_Y[ArrPoint_Count] = point_select[i].Y;
                    ArrPoint_Count++;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            float averagex = 0, averagey = 0;
            foreach (int i in pointX)
            {
                averagex += i;
            }
            foreach (int j in pointY)
            {
                averagey += j;
            }
            averagex /= pointX.Count;        //  取X坐标的平均数
            averagey /= pointY.Count;        //  取Y坐标的平均数

            //经验回归系数的分子与分母
            float numerator = 0;
            float denominator = 0;
            for (int i = 0; i < pointX.Count; i++)
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
            double angle = 180 * Math.Atan(RCB) / Math.PI;

            line_struct param = new line_struct();
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
        public static line_struct TwoPointToLine(Point startPoint, Point endPoint)
        {
            //斜率k=(y1-y2)/(x1-x2)
            float RCB = (float)(endPoint.Y - startPoint.Y ) / (endPoint.X -startPoint.X);
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
        /// 计算俩点之间的中点坐标
        /// </summary>
        /// <param name="startpoint"></param>
        /// <param name="endpoint"></param>
        /// <returns></returns>
        public static Point GetMiddlePoint(Point startpoint,Point endpoint)
        {
            int tempX = (int)(startpoint.X + endpoint.X) / 2;
            int tempY = (int)(startpoint.Y + endpoint.Y) / 2;
            Point middlePoint = new Point(tempX, tempY);
            return middlePoint;
        }


        /// <summary>
        /// 获取参考点的坐标和方向
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        /// <returns></returns>
        public static DirectionPoint GetReferentPoint(Point startPoint, Point endPoint)
        {
            DirectionPoint directionPoint = new DirectionPoint();
            line_struct a = TwoPointToLine(startPoint, endPoint);
            directionPoint.point = startPoint;
            directionPoint.angle = a.angle;
            return directionPoint;
        }
    }
}
