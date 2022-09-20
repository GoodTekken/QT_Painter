#include "painter.h"
#include "ui_painter.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QColorDialog>
#include <QCursor>
#include <QFontDialog>
#include <QPalette>
#include "lidar/TIM561.h"
#include "math/usermath.h"
//#include <math.h>

#define PI 3.14159265

int painter_w;
int painter_h;
bool painter_resized = false;
TIM561 tim;
TIM561 tim2;
bool connect_flag = false;

Painter::Painter(QWidget *parent) : QMainWindow(parent), ui(new Ui::Painter)
{
    ui->setupUi(this);
    ui->color_pen->setTool(1);
    ui->color_back->setTool(2);
    New_File();
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open_File()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save_File()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(Save_As()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(New_File()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    connect(ui->actionRecover, SIGNAL(triggered()), this, SLOT(Undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(Redo()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(Copy()));
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(Cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(Paste()));
    connect(ui->paint_area, SIGNAL(setSize(int,int)), this, SLOT(setSize(int,int)));
    connect(ui->color_pen, SIGNAL(PenColor(QColor)), this, SLOT(PenColor(QColor)));
    connect(ui->color_back, SIGNAL(BrushColor(QColor)), this, SLOT(FillColor(QColor)));
    connect(ui->paint_area, SIGNAL(setSize(int,int)), this, SLOT(setSize(int,int)));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(AboutQt()));
    connect(ui->paint_area, SIGNAL(noUndo()), this, SLOT(noUndo()));
    connect(ui->paint_area, SIGNAL(noRedo()), this, SLOT(noRedo()));
    connect(ui->paint_area, SIGNAL(enableRedo()), this, SLOT(enableRedo()));
    connect(ui->paint_area, SIGNAL(enableUndo()), this, SLOT(enableUndo()));
    connect(ui->paint_area, SIGNAL(noCopy()), this, SLOT(noCopy()));
    connect(ui->paint_area, SIGNAL(canCopy()), this, SLOT(canCopy()));
    connect(ui->paint_area, SIGNAL(canPaste()), this, SLOT(canPaste()));
    connect(ui->actionResize, SIGNAL(triggered()), this, SLOT(showReisize()));

    connect(this,SIGNAL(tranferTimData(const QPointF*)),this,SLOT(showTimData(const QPointF*)));
    connect(this,SIGNAL(draw_leg_blue_point(const QPointF*)),this,SLOT(showTimBlueLeg(const QPointF*)));
    connect(this,SIGNAL(draw_leg_green_point(const QPointF*)),this,SLOT(showTimGreenLeg(const QPointF*)));

    ui->width_string->installEventFilter(this);
    ui->spray_string->installEventFilter(this);
    mytimer = new QTimer(this);
    connect(mytimer, SIGNAL(timeout()), this, SLOT(p_drawLine()));

    mytimer_draw = new QTimer(this);
    connect(mytimer_draw, SIGNAL(timeout()), this, SLOT(p_drawLine_Leg()));

    connect(this, SIGNAL(manualTrigger(const std::vector<std::pair<float, uint16_t>>*)), this, SLOT(p_manual_drawLine(const std::vector<std::pair<float, uint16_t>>*)));
    connect(this, SIGNAL(manualTriggerUp(const std::vector<std::pair<float, uint16_t>>*)), this, SLOT(p_manual_drawLineUp(const std::vector<std::pair<float, uint16_t>>*)));
}

Painter::~Painter()
{
    delete ui;
}

void Painter::on_setwidth_clicked()
{
    int w = ui->width_string->text().toInt();
    if (w != 0)
        ui->paint_area->setPenWidth(w);
}

void Painter::on_tool_pencil_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(1);
    QPixmap mouseCursor = QPixmap(":/img/cursor_pencil.gif");
    QCursor _Cur(mouseCursor, 5, 18);
    ui->paint_area->setCursor(_Cur);
}

void Painter::on_tool_eraser_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(2);
    QPixmap mouseCursor = QPixmap(":/img/tool_eraser.gif");
    QCursor _Cur(mouseCursor, 5, 18);
    ui->paint_area->setCursor(_Cur);
}

void Painter::Open_File()
{
    QString filename =  QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));
    ui->paint_area->openImage(filename);
    _filename = filename;
    _opened = true;
    Resize();
}

void Painter::Save_File()
{
    if (_opened == true || _saved == true)
        ui->paint_area->saveImage(_filename, "png");
    else
        Painter::Save_As();
}

void Painter::Save_As()
{
    QString filename =  QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Images (*.png)"));
    ui->paint_area->saveImage(filename, "png");
    _filename = filename;
    _saved = true;
}

void Painter::on_tool_line_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(3);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::on_tool_rectangle_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(4);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::About()
{
    QMessageBox::about(this, "About", "Hapyy Coding,Tekken");
}

void Painter::New_File()
{
    ui->paint_area->clearImage();
    _opened = false;
    _saved = false;
    _filename = "";
    _font = QFont("Consolas", 10, QFont::Normal);
    ui->paint_area->setPenColor(Qt::black);
    ui->paint_area->setBackColor(QColor(0, 0, 0, 0));
    ui->paint_area->setPenWidth(1);
    ui->paint_area->setFont(_font);
    ui->label_Font->setFont(_font);
    QPalette _pen = ui->color_pen->palette();
    QPalette _back = ui->color_back->palette();
    QPalette _fill_back = ui->color_back_frame->palette();
    QPalette _fill_pen = ui->color_pen_frame->palette();
    _fill_pen.setColor(QPalette::Background, Qt::black);
    _fill_back.setColor(QPalette::Background, Qt::white);
    _pen.setColor(QPalette::Background, QColor(0, 0, 0, 0));
    _back.setColor(QPalette::Background, QColor(0, 0, 0, 0));
    ui->color_back->autoFillBackground();
    ui->color_pen->autoFillBackground();
    ui->color_pen->setPalette(_pen);
    ui->color_back->setPalette(_back);
    ui->paint_area->clearHistory();
    ui->color_back_frame->autoFillBackground();
    ui->color_pen_frame->autoFillBackground();
    ui->color_pen_frame->setPalette(_fill_pen);
    ui->color_back_frame->setPalette(_fill_back);
    ui->paint_area->setGeometry(9, 9, 400, 300);
    ui->scrollAreaWidgetContents->setGeometry(0, 0, 418, 318);
    Resize();

    ui->detect_length->setText("1600");
    ui->detect_width->setText("1300");
    ui->rack_length->setText("1000");
    ui->rack_width->setText("1000");
    ui->pos1_X->setText("0");
    ui->pos1_Y->setText("0");
    ui->pos1_angle->setText("0");
    ui->pos2_X->setText("52");
    ui->pos2_Y->setText("-172");
    ui->pos2_angle->setText("0.6");

}

void Painter::on_tool_eclipse_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(5);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::on_tool_word_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(7);
    ui->paint_area->setCursor(Qt::IBeamCursor);
}

void Painter::on_tool_RoundRect_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(8);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::on_tool_Cursor_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(0);
    ui->paint_area->setCursor(Qt::ArrowCursor);
}

void Painter::on_set_Font_clicked()
{
    bool isOk;
    QFont font = QFontDialog::getFont(&isOk, QFont("Consolas", 18));
    if (isOk)
    {
        ui->paint_area->setPenFont(font);
        _font = font;
        ui->label_Font->setFont(font);
    }
}

void Painter::on_tool_Bucket_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(6);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::Undo()
{
    ui->paint_area->Undo();
}

void Painter::Redo()
{
    ui->paint_area->Redo();
}

void Painter::on_tool_Select_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(9);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::Cut()
{
    ui->paint_area->Cut();
}

void Painter::Copy()
{
    ui->paint_area->Copy();
}

void Painter::Paste()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(10);
    ui->paint_area->setCursor(Qt::CrossCursor);
}

void Painter::on_tool_spray_clicked()
{
    if (ui->paint_area->getZoom())
        ui->paint_area->zoomRecover();
    ui->paint_area->setTool(11);
    QPixmap mouseCursor = QPixmap(":/img/cursor_spray.png");
    QCursor _Cur(mouseCursor, 30, 0);
    ui->paint_area->setCursor(_Cur);
}

void Painter::on_setSpray_clicked()
{
    int w = ui->spray_string->text().toInt();
    if (w != 0)
        ui->paint_area->setSprayWidth(w);
}

void Painter::Resize()
{
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollAreaWidgetContents->setMinimumSize(ui->paint_area->getWidth() + 20, ui->paint_area->getHeight() + 20);
    ui->scrollAreaWidgetContents->setMaximumSize(ui->paint_area->getWidth() + 20, ui->paint_area->getHeight() + 20);
}

void Painter::on_tool_glass_clicked()
{
    ui->paint_area->setTool(12);
    QPixmap mouseCursor = QPixmap(":/img/cursor_glass.png");
    QCursor _Cur(mouseCursor, 30, 0);
    ui->paint_area->setCursor(_Cur);
}

void Painter::PenColor(QColor color)
{
    QPalette _pen = ui->color_pen->palette();
    _pen.setColor(QPalette::Background, color);
    ui->color_pen_frame->setPalette(_pen);
    ui->paint_area->setPenColor(color);
}

void Painter::FillColor(QColor color)
{
    QPalette _pen = ui->color_pen->palette();
    _pen.setColor(QPalette::Background, color);
    ui->color_back_frame->setPalette(_pen);
    ui->paint_area->setBackColor(color);
}

void Painter::setSize(int x, int y)
{
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollAreaWidgetContents->setMinimumSize(x, y);
    ui->scrollAreaWidgetContents->setMaximumSize(x, y);
}

void Painter::AboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void Painter::noRedo()
{
    ui->actionRedo->setEnabled(false);
}

void Painter::noUndo()
{
    ui->actionRecover->setEnabled(false);
}

void Painter::enableRedo()
{
    ui->actionRedo->setEnabled(true);
}

void Painter::canPaste()
{
    ui->actionPaste->setEnabled(true);
}

void Painter::enableUndo()
{
    ui->actionRecover->setEnabled(true);
}

void Painter::noCopy()
{
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
}

void Painter::canCopy()
{
    ui->actionCopy->setEnabled(true);
    ui->actionCut->setEnabled(true);
}

void Painter::showReisize()
{
    ResizeDialog a;
    a.exec();
    if (painter_resized)
    {
        ui->paint_area->ReSize(painter_w, painter_h);
        painter_resized = false;
    }
}

bool Painter::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->width_string && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            int w = ui->width_string->text().toInt();
            if (w != 0)
            {
                ui->paint_area->setPenWidth(w);
                return true;
            }
            else
                return QMainWindow::eventFilter(object, event);
        }
        else
        {
            return QMainWindow::eventFilter(object, event);
        }
    }
    else if (object == ui->spray_string && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            int w = ui->spray_string->text().toInt();
            if (w != 0)
            {
                ui->paint_area->setSprayWidth(w);
                return true;
            }
            else
                return QMainWindow::eventFilter(object, event);
        }
        else
        {
            return QMainWindow::eventFilter(object, event);
        }
    }
    else
    {
        return QMainWindow::eventFilter(object, event);
    }
}

//void *p_basler_Trigger(void *arg)
void *p_connect_Tim561(void *arg)
{
    connect_flag = true;
    std::cout << "Connection..." << std::endl;
    if( tim.connect("192.168.0.71",2112))
    {
        std::cout << "Connected" << std::endl;
        if( tim.start() )
        {
            std::cout << "Started" << std::endl;
            while(connect_flag)
            {
                usleep(70000); //70ms
//                tim.update();
                tim.updatemeandate();
                auto tmp = tim.getDataPoints();
                for( int i = 0 ; i< TIM561::NBR_DATA ; i+=1 )
                {
                    printf("[%g, %d] ", tmp->at(i).first, tmp->at(i).second);
                }
                printf("\n\n");
            }
        }
    }
}

void *p_disconnect_Tim561(void *arg)
{
    connect_flag = false;
    sleep(1);//1s;
    tim.close();
    tim2.close();
    printf("Tim561 disnnect.\n");
}

void Painter::on_connect_clicked()
{
    pthread_t tidp;
    int ret;
    ret = pthread_create(&tidp,NULL,p_connect_Tim561,NULL);
    if(ret)
    {
        printf("pthread_creat failed:%d\n",ret);
    }
    else
    {
        printf("pthread_creat success\n");
    }
}

void Painter::on_disconnect_clicked()
{
    pthread_t tidp;
    int ret;
    ret = pthread_create(&tidp,NULL,p_disconnect_Tim561,NULL);
    if(ret)
    {
        printf("disconnect Tim561 failed:%d\n",ret);
    }
    else
    {
        printf("disconnect Tim561 success\n");
    }
}


void Painter::showTimData(const QPointF* points)
{
//    ui->paint_area->earseImage();
    ui->paint_area->drawTimPoints(points,TIM561::NBR_DATA);
}

void Painter::showTimBlueLeg(const QPointF* points)
{
//    ui->paint_area->earseImage();
    ui->paint_area->drawTimLeg(points,2,Qt::blue);
}

void Painter::showTimGreenLeg(const QPointF* points)
{
//    ui->paint_area->earseImage();
    ui->paint_area->drawTimLeg(points,2,Qt::green);
}


QList<QPoint> Painter::cal_leg(int pointlengthX[],int pointlengthY[])
{
    QList<QPoint> actuallegpoint;
    actuallegpoint.clear();

    float theta = atan((ui->detect_width->text().toFloat()/2.0+200)/ui->detect_length->text().toFloat())*180/PI;
    int countIndex = theta*3;
    float detectlength = ui->detect_length->text().toFloat()+100;
    int middle_placement = (TIM561::NBR_DATA/2) ;
    int cal_startAngleIndex = middle_placement - countIndex;
    int cal_endAngleIndex = middle_placement + countIndex;


    //right point
    int i_count;
     bool flag = false; //判断第一个位置的数据是否进来
     for (i_count = middle_placement; i_count >= cal_startAngleIndex; i_count--)    //7CM的物体在110cm的距离将会扫到的数据：arctan(7/110)*delta = 21.8，即将会有21条光线打到此物体上
     {
         int j_count = 0;
         while (j_count <= 25)
         {
             if ((abs(pointlengthX[i_count] - pointlengthX[i_count - j_count]) > 80) || (abs(pointlengthY[i_count] - pointlengthY[i_count - j_count]) > 80))//获取Y轴方向上的切割位置
             {
                 break;
             }
             j_count++;
         }

         int sumX = 0;
         int sumY = 0;
         QPoint identifyPoint;
         int r = 8;
         if (j_count > 4)    //如果扫描的数量在10个以上，物体的宽度需要达到3cm才会认为检测到。
         {
             if (pointlengthY[i_count] < detectlength&&
                 pointlengthY[i_count] > (detectlength-500))
             {
                 for (int cnt = 0; cnt < j_count; cnt++)
                 {
                     sumX = sumX + (int)pointlengthX[i_count - cnt];
                     sumY = sumY + (int)pointlengthY[i_count - cnt];
                 }
                 identifyPoint.setX(sumX / (j_count));
                 identifyPoint.setY(sumY / (j_count));
                 if(flag == false)
                 {
                     actuallegpoint.append(identifyPoint);
                     flag = true;
//                         PaintVar.g.DrawEllipse(PaintVar.Green_pen, PaintVar.halfWidth + identifyPoint.X * addsd - r, PaintVar.halfHeight - identifyPoint.Y * addsd - r, r * 2, r * 2);
                 }
             }

         }
         i_count = i_count - j_count;
     }

     //left point
     flag = false;
     for (i_count = middle_placement; i_count <= cal_endAngleIndex; i_count++)    //7CM的物体在110cm的距离将会扫到的数据：arctan(7/110)*delta = 21.8，即将会有21条光线打到此物体上
     {
         int j_count = 0;
         while (j_count <= 25)
         {
             if ((abs(pointlengthX[i_count] -pointlengthX[i_count + j_count]) > 80) || (abs(pointlengthY[i_count] - pointlengthY[i_count + j_count]) > 80))//获取Y轴方向上的切割位置
             {
                 break;
             }
             j_count++;
         }

         int sumX = 0;
         int sumY = 0;
         QPoint identifyPoint;
         int r = 8;
         if (j_count > 4)    //如果扫描的数量在10个以上，物体的宽度需要达到3cm才会认为检测到。
         {
             if (pointlengthY[i_count] < detectlength&&
                 pointlengthY[i_count] > (detectlength-500))
             {
                 for (int cnt = 0; cnt < j_count; cnt++)
                 {
                     sumX = sumX + (int)pointlengthX[i_count + cnt];
                     sumY = sumY + (int)pointlengthY[i_count + cnt];
                 }
                 identifyPoint.setX(sumX / (j_count));
                 identifyPoint.setY(sumY / (j_count));
                 if (flag == false)
                 {
                     actuallegpoint.append(identifyPoint);
                     flag = true;
//                         PaintVar.g.DrawEllipse(PaintVar.Green_pen, PaintVar.halfWidth + identifyPoint.X * addsd - r, PaintVar.halfHeight - identifyPoint.Y * addsd - r, r * 2, r * 2);
                 }
             }

         }
         i_count = i_count + j_count;
     }
     return actuallegpoint;
}

void Painter::p_drawLine()
{
    if(connect_flag == true)
    {
        int pointlengthX[TIM561::NBR_DATA];
        int pointlengthY[TIM561::NBR_DATA];
        auto tmp = tim.getDataPoints();

        float pos_x1 = ui->pos1_X->text().toFloat();
        float pos_y1 = ui->pos1_Y->text().toFloat();
        float pos_angle1 = ui->pos1_angle->text().toFloat();

        int temx;
        int temy;
        QPointF pointfToPaint[TIM561::NBR_DATA];
        for( int i = 0 ; i< TIM561::NBR_DATA ; i+=1 )
        {
            temx = tmp->at(i).second * cos((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_x1;
            temy = tmp->at(i).second * sin((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_y1;
            pointlengthX[i]= (int)(temx);
            pointlengthY[i]= (int)(temy);
            pointfToPaint[i].setX(400 + temx/scale);
            pointfToPaint[i].setY(600 - temy/scale);
        }
        emit tranferTimData(pointfToPaint);


        QList<QPoint> actuallegpoint;
        actuallegpoint=cal_leg(pointlengthX,pointlengthY);


        QPoint orignal(0,0);
        QPointF legpointf[2];

        if(actuallegpoint.count() ==2)
        {
            if(UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[0])>0&&
               UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[1])>0&&
               UserMath::GetDiatanceBetweenPoint(actuallegpoint[0],actuallegpoint[1])>500)
            {
                //right point
                legpointf[0].setX(400 + actuallegpoint[0].x()/scale);
                legpointf[0].setY(600 - actuallegpoint[0].y()/scale);
                //left point
                legpointf[1].setX(400 + actuallegpoint[1].x()/scale);
                legpointf[1].setY(600 - actuallegpoint[1].y()/scale);
                //reference point
                DirectionPoint middleReference = UserMath::GetMiddleReferentPoint(actuallegpoint[0],actuallegpoint[1]);
                ui->ref1_x->setText(QString::number(middleReference.point.x()));
                ui->ref1_y->setText(QString::number(middleReference.point.y()));
                ui->ref1_angle->setText(QString::number(middleReference.angle,'f',2));
                //show
                ui->right1_X->setText(QString::number(actuallegpoint[0].x()));
                ui->right1_Y->setText(QString::number(actuallegpoint[0].y()));
                ui->left1_X->setText(QString::number(actuallegpoint[1].x()));
                ui->left1_Y->setText(QString::number(actuallegpoint[1].y()));
                //draw
                emit draw_leg_green_point(legpointf);
            }
        }
     }
}

void Painter::p_manual_drawLine(const std::vector<std::pair<float, uint16_t>>* tmp)
{
    if(connect_flag == true)
    {
        int pointlengthX[TIM561::NBR_DATA];
        int pointlengthY[TIM561::NBR_DATA];
//        auto tmp = tim.getDataPoints();

        float pos_x1 = ui->pos1_X->text().toFloat();
        float pos_y1 = ui->pos1_Y->text().toFloat();
        float pos_angle1 = ui->pos1_angle->text().toFloat();

        int temx;
        int temy;
        QPointF pointfToPaint[TIM561::NBR_DATA];
        for( int i = 0 ; i< TIM561::NBR_DATA ; i+=1 )
        {
            temx = tmp->at(i).second * cos((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_x1;
            temy = tmp->at(i).second * sin((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_y1;
            pointlengthX[i]= (int)(temx);
            pointlengthY[i]= (int)(temy);
            pointfToPaint[i].setX(400 + temx/scale);
            pointfToPaint[i].setY(600 - temy/scale);
        }
        emit tranferTimData(pointfToPaint);


        QList<QPoint> actuallegpoint;
        actuallegpoint=cal_leg(pointlengthX,pointlengthY);


        QPoint orignal(0,0);
        QPointF legpointf[2];

        if(actuallegpoint.count() ==2)
        {
            if(UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[0])>0&&
               UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[1])>0&&
               UserMath::GetDiatanceBetweenPoint(actuallegpoint[0],actuallegpoint[1])>500)
            {
                //right point
                legpointf[0].setX(400 + actuallegpoint[0].x()/scale);
                legpointf[0].setY(600 - actuallegpoint[0].y()/scale);
                //left point
                legpointf[1].setX(400 + actuallegpoint[1].x()/scale);
                legpointf[1].setY(600 - actuallegpoint[1].y()/scale);
                //reference point
                DirectionPoint middleReference = UserMath::GetMiddleReferentPoint(actuallegpoint[0],actuallegpoint[1]);
                ui->ref1_x->setText(QString::number(middleReference.point.x()));
                ui->ref1_y->setText(QString::number(middleReference.point.y()));
                ui->ref1_angle->setText(QString::number(middleReference.angle,'f',2));
                //show
                ui->right1_X->setText(QString::number(actuallegpoint[0].x()));
                ui->right1_Y->setText(QString::number(actuallegpoint[0].y()));
                ui->left1_X->setText(QString::number(actuallegpoint[1].x()));
                ui->left1_Y->setText(QString::number(actuallegpoint[1].y()));
                //draw
                emit draw_leg_blue_point(legpointf);
            }
        }
     }
}

void Painter::p_manual_drawLineUp(const std::vector<std::pair<float, uint16_t>>* tmp)
{
    if(connect_flag == true)
    {
        int pointlengthX[TIM561::NBR_DATA];
        int pointlengthY[TIM561::NBR_DATA];
//        auto tmp = tim.getDataPoints();

        float pos_x1 = ui->pos2_X->text().toFloat();
        float pos_y1 = ui->pos2_Y->text().toFloat();
        float pos_angle1 = ui->pos2_angle->text().toFloat();

        int temx;
        int temy;
        QPointF pointfToPaint[TIM561::NBR_DATA];
        for( int i = 0 ; i< TIM561::NBR_DATA ; i+=1 )
        {
            temx = tmp->at(i).second * cos((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_x1;
            temy = tmp->at(i).second * sin((tmp->at(i).first+pos_angle1)*PI/180.0)+pos_y1;
            pointlengthX[i]= (int)(temx);
            pointlengthY[i]= (int)(temy);
            pointfToPaint[i].setX(400 + temx/scale);
            pointfToPaint[i].setY(600 - temy/scale);
        }
        emit tranferTimData(pointfToPaint);


        QList<QPoint> actuallegpoint;
        actuallegpoint=cal_leg(pointlengthX,pointlengthY);


        QPoint orignal(0,0);
        QPointF legpointf[2];

        if(actuallegpoint.count() ==2)
        {
            if(UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[0])>0&&
               UserMath::GetDiatanceBetweenPoint(orignal,actuallegpoint[1])>0&&
               UserMath::GetDiatanceBetweenPoint(actuallegpoint[0],actuallegpoint[1])>500)
            {
                //right point
                legpointf[0].setX(400 + actuallegpoint[0].x()/scale);
                legpointf[0].setY(600 - actuallegpoint[0].y()/scale);
                //left point
                legpointf[1].setX(400 + actuallegpoint[1].x()/scale);
                legpointf[1].setY(600 - actuallegpoint[1].y()/scale);
                //reference point
                DirectionPoint middleReference = UserMath::GetMiddleReferentPoint(actuallegpoint[0],actuallegpoint[1]);
                ui->ref2_x->setText(QString::number(middleReference.point.x()));
                ui->ref2_y->setText(QString::number(middleReference.point.y()));
                ui->ref2_angle->setText(QString::number(middleReference.angle,'f',2));
                //show
                ui->right2_X->setText(QString::number(actuallegpoint[0].x()));
                ui->right2_Y->setText(QString::number(actuallegpoint[0].y()));
                ui->left2_X->setText(QString::number(actuallegpoint[1].x()));
                ui->left2_Y->setText(QString::number(actuallegpoint[1].y()));
                //draw
                emit draw_leg_green_point(legpointf);
            }
        }
     }
}
void Painter::on_drawLine_clicked()
{
    if(mytimer->isActive()==false)
    {
        mytimer->start(100);  //100ms
    }
    else
    {
        mytimer->stop();
    }
}

void *Painter::p_test_Tim561(void *arg)
{
    ;
}

void Painter::p_drawLine_Leg()
{
    clean_count++;
    if(clean_count>5)
    {
         ui->paint_area->clearImage();
        clean_count=0;
    }

    connect_flag = true;
    if( tim.connect("192.168.0.71",2112))
    {
        std::cout << "Connected" << std::endl;
        if( tim.start() )
        {
            std::cout << "Started" << std::endl;
            tim.update();
            auto tmp = tim.getDataPoints();
            emit this->manualTrigger(tmp); //blue
        }
    }
    tim.close();

    if( tim.connect("192.168.0.72",2112))
    {
        std::cout << "Connected" << std::endl;
        if( tim.start() )
        {
            std::cout << "Started" << std::endl;
            tim.update();
            auto tmp = tim.getDataPoints();
            emit this->manualTriggerUp(tmp);  //green
        }
    }
    tim.close();
    float temp1,temp2,temp3;
    temp1=ui->ref1_x->text().toFloat() - ui->ref2_x->text().toFloat();
    ui->delta_x->setText(QString::number(temp1));
    temp2=ui->ref1_y->text().toFloat() - ui->ref2_y->text().toFloat();
    ui->delta_y->setText(QString::number(temp2));
    temp3=ui->ref1_angle->text().toFloat() - ui->ref2_angle->text().toFloat();
    ui->delta_angle->setText(QString::number(temp3,'f',2));

    QPalette palette = ui->success_edit->palette();
    if(abs(temp1)<15&&abs(temp2)<15&&abs(temp3)<1)
    {
        palette.setColor(QPalette::Base,Qt::green);
        ui->success_edit->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::Base,Qt::red);
        ui->success_edit->setPalette(palette);
    }
}

void Painter::on_Test_Button_clicked()
{
    if(mytimer_draw->isActive()==false)
    {
        mytimer_draw->start(300);  //100ms
    }
    else
    {
        mytimer_draw->stop();
    }
}

//    connect_flag = true;
//    if( tim.connect("192.168.0.72",2112))
//    {
//        std::cout << "Connected" << std::endl;
//        if( tim.start() )
//        {
//            std::cout << "Started" << std::endl;
//            tim.update();
//            auto tmp = tim.getDataPoints();
//            emit this->manualTrigger(tmp);
//        }
//    }

//    pthread_t tidp;
//    int ret;
//    ret = pthread_create(&tidp,NULL,p_test_Tim561,this);
//    if(ret)
//    {
//        printf("disconnect Tim561 failed:%d\n",ret);
//    }
//    else
//    {
//        printf("disconnect Tim561 success\n");
//    }
