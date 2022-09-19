using Nanoscan3_Library;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
/***************************************************************************
************
*         *
*        *
*       *
* θ   *    
*---- *   
*    *                        以中心轴为其中的直角边，支架长度的一半+20CM为另一直角边边，计算此时的夹角:
*   *                         double rackStartAngle = 90 - rackTheta;
*  *                          double rackEndAngle   = 90 + rackTheta;
* *
**
*
*
*   B1                         B2
*    ___________________________
*    |                         |
*    |                         |
*    |                         |
*    |                         |
*    |                         |
*    |_________________________|
*  A1             ^            A2  
*               参考点 
*                               
*                               A1-A2:货架的长度
*                               A1-B1:货架的宽度
*                               根据A1和A2的坐标计算出A1和A2之间的参考点坐标以及方向
*
*
*****************************************************************************///rackStartAngle的计算方式

namespace WinFormMainApplication
{
    public partial class Form1 : Form
    {
        public void Paint_Paint(int[] measurementdatatemp, bool boolShowStyletemp,float startAngletemp,float endAngletemp)
        {
            Paint_Initializatian(measurementdatatemp, boolShowStyletemp, startAngletemp, endAngletemp);
            ExchangeTheMeasurementDate();
            ShowActualTotalOutLine();


            //PaintVar.legPoint = IdentifyRackLeg();//二选一：方法1：逆时针旋转，符合范围内的所有点。
            PaintVar.legPoint = IdentifyRackLegFromMiddleScan();  //二选一：方法2，从扫描仪中间开始寻找，找到符合条件的俩个点

            ShowActualRackOutLine(PaintVar.legPoint);
            ShowInTextBox();
        }

        /// <summary>
        /// 参数初始化
        /// </summary>
        public void Paint_Initializatian(int[] measurementdatatemp, bool boolShowStyletemp, float startAngletemp, float endAngletemp)
        {
            PaintVar.measurementdata = (int[])measurementdatatemp.Clone();  //数组的复制
            PaintVar.boolShowStyle = boolShowStyletemp;
            PaintVar.startAngle = startAngletemp;
            PaintVar.endAngle = endAngletemp;

            PaintVar.halfWidth = this.Width / 2;
            PaintVar.halfHeight = this.Height / 2;
            PaintVar.Blue_pen_arrow.CustomEndCap = PaintVar.arrow;
            PaintVar.penGoldRack.DashStyle = DashStyle.Custom;
            PaintVar.penGoldRack.DashPattern = new float[] { 1f, 1f };

            PaintVar.g = CreateGraphics();
            PaintVar.distance = new int[PaintVar.nanoscanDataLength];   //measurementdataLength = 1651
            PaintVar.lengthX = new float[PaintVar.nanoscanDataLength];
            PaintVar.lengthY = new float[PaintVar.nanoscanDataLength];
        }


        /// <summary>
        /// 转化扫描仪的长度数据为角度数据
        /// </summary>
        public void ExchangeTheMeasurementDate()
        {
            for (int i = 0; i < PaintVar.nanoscanDataLength; i++)
            {
                PaintVar.distance[i] = PaintVar.measurementdata[i];  //将长度单位mm转换成cm显示出来
            }

            for (int i = 0; i < PaintVar.nanoscanDataLength; i++)      //1651
            {
                PaintVar.length = PaintVar.distance[i];
                PaintVar.delta = PaintVar.nanoscanDataLength / (PaintVar.endAngle - PaintVar.startAngle);   //每一度所包含的数据量
                PaintVar.theta = (float)((i / PaintVar.delta) + PaintVar.startAngle);  //从-47.5开始计数
                if (PaintVar.length == 0)
                {
                    PaintVar.length = 7000;
                }
                PaintVar.lengthX[i] = PaintVar.length * (float)(Math.Cos(Math.PI * PaintVar.theta / 180));
                PaintVar.lengthY[i] = PaintVar.length * (float)(Math.Sin(Math.PI * PaintVar.theta / 180));
            }
        }

        /// <summary>
        /// 显示扫描仪所有的扫描坐标
        /// </summary>
        public void ShowActualTotalOutLine()
        {
            PaintVar.g.Clear(this.BackColor);   //清除画面
            if (PaintVar.boolShowStyle)
            {
                Point[] point = new Point[PaintVar.nanoscanDataLength];
                for (int i = 0; i < PaintVar.nanoscanDataLength; i++)      //显示方式：画点
                {
                    point[i].X = (int)(PaintVar.halfWidth + PaintVar.lengthX[i] * addsd);
                    point[i].Y = (int)(PaintVar.halfHeight - PaintVar.lengthY[i] * addsd);
                }
                PaintVar.g.DrawLines(PaintVar.laser, point);
            }
            else
            {
                for (int i = 0; i < PaintVar.nanoscanDataLength; i++)      //显示方式：画线
                {
                    PaintVar.g.DrawLine(PaintVar.laser, PaintVar.halfWidth, PaintVar.halfHeight, PaintVar.halfWidth + PaintVar.lengthX[i] * addsd, PaintVar.halfHeight - PaintVar.lengthY[i] * addsd);
                }
            }
        }

        /// <summary>
        /// 支腿识别 方法1，逆时针扫描
        /// </summary>
        /// <returns></returns>
        public List<Point> IdentifyRackLeg()
        {
            //Math.Atan()返回弧度，如Math.Atan(1)时，返回的时pi/4;
            double rackTheta = 180 / Math.PI * Math.Atan((PaintVar.pallet_length / 2 + 200) * 1.0 / PaintVar.identify_distace * 1.0); //以中心轴为其中的直角边，支架长度的一半+20CM为另一直角边边，计算此时的夹角。
            double rackStartAngle = 90 - rackTheta;
            double rackEndAngle = 90 + rackTheta;
            List<Point> legPoint = new List<Point>();

            //显示扫描区域
            PaintVar.g.DrawLine(PaintVar.penGoldRack,
                    PaintVar.halfWidth,
                    PaintVar.halfHeight,
                    PaintVar.halfWidth  + 2000 * UserCos(rackStartAngle) * addsd,
                    PaintVar.halfHeight - 2000 * UserSin(rackStartAngle) * addsd);
            PaintVar.g.DrawLine(PaintVar.penGoldRack,
                    PaintVar.halfWidth,
                    PaintVar.halfHeight,
                    PaintVar.halfWidth  + 2000 * UserCos(rackEndAngle) * addsd,
                    PaintVar.halfHeight - 2000 * UserSin(rackEndAngle) * addsd);
            //********************//

            PaintVar.delta = PaintVar.nanoscanDataLength / (PaintVar.endAngle - PaintVar.startAngle);   //每一度所包含的数据量
            int cal_startAngleIndex = (int)((rackStartAngle - PaintVar.startAngle) * PaintVar.delta);
            int cal_endAngleIndex = (int)((rackEndAngle - PaintVar.startAngle) * PaintVar.delta);

            int i_count;
            for (i_count = cal_startAngleIndex; i_count <= cal_endAngleIndex; i_count++)    //7CM的物体在110cm的距离将会扫到的数据：arctan(7/110)*delta = 21.8，即将会有21条光线打到此物体上
            {
                int j_count = 0;
                while (j_count <= 25)
                {
                    if ((Math.Abs(PaintVar.lengthX[i_count] - PaintVar.lengthX[i_count + j_count]) > 80)||(Math.Abs(PaintVar.lengthY[i_count] - PaintVar.lengthY[i_count + j_count]) > 80))//获取Y轴方向上的切割位置
                    {
                        break;
                    }
                    j_count++;
                }

                int sumX = 0;
                int sumY = 0;
                Point identifyPoint = new Point();
                int r = 8;
                if (j_count > 4)    //如果扫描的数量在10个以上，物体的宽度需要达到3cm才会认为检测到。
                {
                    if (PaintVar.lengthY[i_count] < 2000.0)
                    {
                        for (int cnt = 0; cnt < j_count; cnt++)
                        {
                            sumX = sumX + (int)PaintVar.lengthX[i_count + cnt];
                            sumY = sumY + (int)PaintVar.lengthY[i_count + cnt];
                        }
                        identifyPoint.X = sumX / (j_count);
                        identifyPoint.Y = sumY / (j_count);
                        legPoint.Add(identifyPoint);
                        PaintVar.g.DrawEllipse(PaintVar.Green_pen, PaintVar.halfWidth + identifyPoint.X * addsd - r, PaintVar.halfHeight - identifyPoint.Y * addsd - r, r * 2, r * 2);
                    }

                }
                i_count = i_count + j_count;
            }
            return legPoint;
        }


        /// <summary>
        /// 支腿识别 方法2，从中间向两边扫描，各自选择符合要求的第一个点，1651/2=825。
        /// </summary>
        /// <returns></returns>
        public List<Point> IdentifyRackLegFromMiddleScan()
        {
            //Math.Atan()返回弧度，如Math.Atan(1)时，返回的时pi/4;
            double rackTheta = 180 / Math.PI * Math.Atan((PaintVar.pallet_length / 2 + 200) * 1.0 / PaintVar.identify_distace * 1.0); //以中心轴为其中的直角边，支架长度的一半+20CM为另一直角边边，计算此时的夹角。
            double rackStartAngle = 90 - rackTheta;
            double rackEndAngle = 90 + rackTheta;
            List<Point> legPoint = new List<Point>();

            //显示扫描区域
            PaintVar.g.DrawLine(PaintVar.penGoldRack,
                    PaintVar.halfWidth,
                    PaintVar.halfHeight,
                    PaintVar.halfWidth + 2000 * UserCos(rackStartAngle) * addsd,
                    PaintVar.halfHeight - 2000 * UserSin(rackStartAngle) * addsd);
            PaintVar.g.DrawLine(PaintVar.penGoldRack,
                    PaintVar.halfWidth,
                    PaintVar.halfHeight,
                    PaintVar.halfWidth + 2000 * UserCos(rackEndAngle) * addsd,
                    PaintVar.halfHeight - 2000 * UserSin(rackEndAngle) * addsd);
            //********************//

            PaintVar.delta = PaintVar.nanoscanDataLength / (PaintVar.endAngle - PaintVar.startAngle);   //每一度所包含的数据量
            int cal_startAngleIndex = (int)((rackStartAngle - PaintVar.startAngle) * PaintVar.delta);
            int cal_endAngleIndex = (int)((rackEndAngle - PaintVar.startAngle) * PaintVar.delta);

            int i_count;
            bool flag = false; //判断第一个位置的数据是否进来
            for (i_count = 825; i_count >= cal_startAngleIndex; i_count--)    //7CM的物体在110cm的距离将会扫到的数据：arctan(7/110)*delta = 21.8，即将会有21条光线打到此物体上
            {
                int j_count = 0;
                while (j_count <= 25)
                {
                    if ((Math.Abs(PaintVar.lengthX[i_count] - PaintVar.lengthX[i_count - j_count]) > 80) || (Math.Abs(PaintVar.lengthY[i_count] - PaintVar.lengthY[i_count - j_count]) > 80))//获取Y轴方向上的切割位置
                    {
                        break;
                    }
                    j_count++;
                }

                int sumX = 0;
                int sumY = 0;
                Point identifyPoint = new Point();
                int r = 8;
                if (j_count > 4)    //如果扫描的数量在10个以上，物体的宽度需要达到3cm才会认为检测到。
                {
                    if (PaintVar.lengthY[i_count] < 2000.0)
                    {
                        for (int cnt = 0; cnt < j_count; cnt++)
                        {
                            sumX = sumX + (int)PaintVar.lengthX[i_count - cnt];
                            sumY = sumY + (int)PaintVar.lengthY[i_count - cnt];
                        }
                        identifyPoint.X = sumX / (j_count);
                        identifyPoint.Y = sumY / (j_count);
                        if(flag == false)
                        {
                            legPoint.Add(identifyPoint);
                            flag = true;
                            PaintVar.g.DrawEllipse(PaintVar.Green_pen, PaintVar.halfWidth + identifyPoint.X * addsd - r, PaintVar.halfHeight - identifyPoint.Y * addsd - r, r * 2, r * 2);
                        }
                    }

                }
                i_count = i_count - j_count;
            }

            flag = false;
            for (i_count = 825; i_count <= cal_endAngleIndex; i_count++)    //7CM的物体在110cm的距离将会扫到的数据：arctan(7/110)*delta = 21.8，即将会有21条光线打到此物体上
            {
                int j_count = 0;
                while (j_count <= 25)
                {
                    if ((Math.Abs(PaintVar.lengthX[i_count] - PaintVar.lengthX[i_count + j_count]) > 80) || (Math.Abs(PaintVar.lengthY[i_count] - PaintVar.lengthY[i_count + j_count]) > 80))//获取Y轴方向上的切割位置
                    {
                        break;
                    }
                    j_count++;
                }

                int sumX = 0;
                int sumY = 0;
                Point identifyPoint = new Point();
                int r = 8;
                if (j_count > 4)    //如果扫描的数量在10个以上，物体的宽度需要达到3cm才会认为检测到。
                {
                    if (PaintVar.lengthY[i_count] < 2000.0)
                    {
                        for (int cnt = 0; cnt < j_count; cnt++)
                        {
                            sumX = sumX + (int)PaintVar.lengthX[i_count + cnt];
                            sumY = sumY + (int)PaintVar.lengthY[i_count + cnt];
                        }
                        identifyPoint.X = sumX / (j_count);
                        identifyPoint.Y = sumY / (j_count);
                        if (flag == false)
                        {
                            legPoint.Add(identifyPoint);
                            flag = true;
                            PaintVar.g.DrawEllipse(PaintVar.Green_pen, PaintVar.halfWidth + identifyPoint.X * addsd - r, PaintVar.halfHeight - identifyPoint.Y * addsd - r, r * 2, r * 2);
                        }
                    }

                }
                i_count = i_count + j_count;
            }
            return legPoint;
        }

        public void ShowActualRackOutLine(List<Point> legPoint)
        {
            if(legPoint.Count==2)
            {
                int i = 0;
                Point[] legPointArray = new Point[2];

                CleanTextBox();

                foreach (Point tempPoint in legPoint)
                {
                    legPointArray[i] = tempPoint;
                    i++;
                }

                double distance = MathUtinity.GetDiatanceBetweenPoint(legPointArray[0], legPointArray[1]);
                if (distance > (PaintVar.pallet_length-200))  //大于货架的长度-200mm
                {
                    InvokeShowInTextBox($"支腿长度：{distance}");
                    //绘制实际图形位置
                    AllowEnterTheArea(legPointArray);
                }
                else
                {
                    InvokeShowInTextBox($"error:支腿长度少于设定长度(-200mm)：{distance}");
                    CannotEnterTheArea();
                }
            }

            else
            {
                InvokeShowInTextBox($"error:目标点数量不正确：{ legPoint.Count}");
                CannotEnterTheArea();
            }
        }

        public void AllowEnterTheArea(Point[] pointArray)
        {
            Point[] A1A2Point = pointArray;
            DirectionPoint ReferentPoint = MathUtinity.GetReferentPoint(A1A2Point[0], A1A2Point[1]);
            PaintVar.g.DrawLine(PaintVar.Blue_pen_arrow, 
                                PaintVar.halfWidth, 
                                PaintVar.halfHeight, 
                                PaintVar.halfWidth + ReferentPoint.point.X * addsd, 
                                PaintVar.halfHeight - ReferentPoint.point.Y * addsd);//指定中点方向

            if(Math.Abs(ReferentPoint.angle)<10)  //摆正10度以内才能进入
            {
                Point centerPoint = new Point();
                float axisAngle = (float)ReferentPoint.angle + 90;  //对称轴角度
                centerPoint.X = ReferentPoint.point.X + (int)((PaintVar.pallet_width / 2)*UserCos(axisAngle));
                centerPoint.Y = ReferentPoint.point.Y + (int)((PaintVar.pallet_width / 2)*UserSin(axisAngle));
                Point A2 = A1A2Point[0];
                Point A1 = A1A2Point[1];
                Point B1 = new Point(A1.X  + (int)((PaintVar.pallet_width) * UserCos(axisAngle)), 
                                     A1.Y  + (int)((PaintVar.pallet_width) * UserSin(axisAngle)));
                Point B2 = new Point(A2.X  + (int)((PaintVar.pallet_width) * UserCos(axisAngle)), 
                                     A2.Y  + (int)((PaintVar.pallet_width) * UserSin(axisAngle)));

                //蓝色表示摆放端正,连线A1-A2，A2-B2，B1-B2，A1-B1
                PaintVar.g.DrawLine(PaintVar.Blue_pen,
                    PaintVar.halfWidth  + A1.X * addsd,
                    PaintVar.halfHeight - A1.Y * addsd,
                    PaintVar.halfWidth  + A2.X * addsd,
                    PaintVar.halfHeight - A2.Y * addsd);
                PaintVar.g.DrawLine(PaintVar.Blue_pen,
                    PaintVar.halfWidth  + A2.X * addsd,
                    PaintVar.halfHeight - A2.Y * addsd,
                    PaintVar.halfWidth  + B2.X * addsd,
                    PaintVar.halfHeight - B2.Y * addsd);
                PaintVar.g.DrawLine(PaintVar.Blue_pen,
                    PaintVar.halfWidth  + B1.X * addsd,
                    PaintVar.halfHeight - B1.Y * addsd,
                    PaintVar.halfWidth  + B2.X * addsd,
                    PaintVar.halfHeight - B2.Y * addsd);
                PaintVar.g.DrawLine(PaintVar.Blue_pen,
                    PaintVar.halfWidth  + A1.X * addsd,
                    PaintVar.halfHeight - A1.Y * addsd,
                    PaintVar.halfWidth  + B1.X * addsd,
                    PaintVar.halfHeight - B1.Y * addsd);

                PaintVar.g.DrawLine(PaintVar.Blue_pen_arrow,
                    PaintVar.halfWidth  + ReferentPoint.point.X * addsd,
                    PaintVar.halfHeight - ReferentPoint.point.Y * addsd,
                    PaintVar.halfWidth  + centerPoint.X * addsd,
                    PaintVar.halfHeight - centerPoint.Y * addsd);//指定中点方向
            }
            else
            {
                InvokeShowInTextBox($"error:角度超过范围：{ ReferentPoint.angle}");
            }
        }

        //求Cos(角度)的值
        public float UserCos(double angle)  
        {
            return ((float)(Math.Cos(Math.PI * angle / 180)));
        }

        //求Sin(角度)的值
        public float UserSin(double angle)
        {
            return ((float)(Math.Sin(Math.PI * angle / 180)));
        }

        public void CannotEnterTheArea()
        {
            //绘制红框表示不能进入
            Graphics g1 = this.CreateGraphics();//获取到graphics对象
            int a1 = PaintVar.halfWidth - (int)(PaintVar.pallet_length / 2 * addsd);
            int a2 = PaintVar.halfHeight - (int)((PaintVar.identify_distace + PaintVar.pallet_width) * addsd);
            int a3 = (int)(PaintVar.pallet_length * addsd);
            int a4 = (int)(PaintVar.pallet_width * addsd);
            Rectangle rect = new Rectangle(a1, a2, a3, a4);//定义矩形,参数为起点横纵坐标以及其长和宽                                            //Rectangle rect = new Rectangle(10, 10, 30, 30);
            g1.DrawRectangle(PaintVar.Red_pen, rect);
        }

        public void ShowInTextBox() //跨线程调用
        {
            foreach (Point tempPoint in PaintVar.legPoint)
            {
                InvokeShowInTextBox($"Point_X:{tempPoint.X.ToString()}, Point_Y:{tempPoint.Y.ToString()}");
            }
        }

        public void InvokeShowInTextBox(string str)
        {
            BeginInvoke(new MethodInvoker(delegate ()
            {
                textBox1.Text += str+"\r\n";
            }));
        }

        public void CleanTextBox()
        {
            BeginInvoke(new MethodInvoker(delegate ()
            {
                textBox1.Text = "";
            }));
        }
    }
}



//g.DrawLine(laser, halfWidth, halfHeight - 20,  0, 0);                   //画左上角
//g.DrawLine(laser, halfWidth, halfHeight - 20, this.Width, 0);           //画右上角
//g.DrawLine(laser, halfWidth, halfHeight - 20, 0, this.Height);          //画左下角
//g.DrawLine(laser, halfWidth, halfHeight - 20, this.Width, this.Height); //画右下角


////*******************************************//货架轮廓
//Graphics g1 = this.CreateGraphics();//获取到graphics对象
//int a1 = halfWidth - (pallet_length / 2);
//int a2 = halfHeight - 20 - identify_distace - pallet_width;
//int a3 = pallet_length;
//int a4 = pallet_width;
//Rectangle rect = new Rectangle(a1, a2, a3, a4);//定义矩形,参数为起点横纵坐标以及其长和宽
//                                               //Rectangle rect = new Rectangle(10, 10, 30, 30);
//g1.DrawRectangle(Pens.Blue, rect);
////*******************************************//
///
//int cal_length = identify_distace + pallet_width;
//int cal_width = pallet_length;
//double cal_theta;
//int cal_angle;
//int cal_angle_90;
//int a;
//int b;
//float min_left;
//float min_right;

//cal_theta = Math.Atan((double)(cal_width / 2 - 10) / cal_length);            //50 280 11度
//cal_angle = (int)(Math.Ceiling(cal_theta * 180 / Math.PI));      //计算栈板的内角

//cal_angle_90 = 270 - cal_angle * 3;
//a = 135 + cal_angle_90;
//b = 675 - cal_angle_90;
//min_left = lengthX[135];
//min_right = lengthX[b];


//for (int i = 135; i <= a; i++)
//{
//    if (lengthX[i] <= cal_width / 2 - 10)
//    {
//        Result[0] = 0;
//        Result[1] = 0;
//        return Result;
//    }
//    if (lengthX[i] < min_left)
//    {
//        min_left = lengthX[i];
//    }
//}
//for (int i = a; i <= b; i++)
//{
//    if (lengthY[i] <= cal_length)
//    {
//        Result[0] = 0;
//        Result[1] = 0;
//        return Result;
//    }
//}
//for (int i = b; i <= 675; i++)  //右边为负数
//{
//    if (Math.Abs(lengthX[i]) <= (cal_width / 2 - 10))
//    {
//        Result[0] = 0;
//        Result[1] = 0;
//        return Result;
//    }
//    if (lengthX[i] > min_right)   //为负数比较
//    {
//        min_right = lengthX[i];
//    }
//}

//if ((min_left - min_right) < cal_width)  //少于栈板的宽度表示放不进货物
//{
//    Result[0] = 0;
//    Result[1] = 0;
//    return Result;
//}

//Result[0] = (int)(min_left - cal_width / 2);
//Result[1] = (int)(-min_right - cal_width / 2);
//return Result;