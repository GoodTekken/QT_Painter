#ifndef PAINTER_H
#define PAINTER_H

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include "resizedialog.h"
#include <QTimer>
#include <math/mathStruct.h>

namespace Ui {
class Painter;
}

class Painter : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Painter(QWidget *parent = 0);
    void *p_test_Tim561(void *arg);
    ~Painter();

signals:
    void tranferTimData(const QPointF* points);
    void draw_leg_blue_point(const QPointF* points);
    void draw_leg_green_point(const QPointF* points);
    void manualTrigger(const std::vector<std::pair<float, uint16_t>>*);
    void manualTriggerUp(const std::vector<std::pair<float, uint16_t>>*);

    
private slots:
    void on_setwidth_clicked();

    void on_tool_pencil_clicked();

    void on_tool_eraser_clicked();

    void Open_File();

    void Save_File();

    void Save_As();

    void on_tool_line_clicked();

    void on_tool_rectangle_clicked();

    void About();

    void New_File();

    void on_tool_eclipse_clicked();

    void on_tool_word_clicked();

    void on_tool_RoundRect_clicked();

    void on_tool_Cursor_clicked();

    void on_set_Font_clicked();

    void on_tool_Bucket_clicked();

    void Undo();

    void Redo();

    void on_tool_Select_clicked();

    void Cut();

    void Copy();

    void Paste();

    void on_tool_spray_clicked();

    void on_setSpray_clicked();

    void on_tool_glass_clicked();

    void Resize();

    void PenColor(QColor);

    void FillColor(QColor);

    void setSize(int, int);

    void AboutQt();

    void noRedo();

    void noUndo();

    void enableRedo();

    void enableUndo();

    void canPaste();

    void canCopy();

    void noCopy();

    void showReisize();

    void on_connect_clicked();

    void on_disconnect_clicked();

    void on_drawLine_clicked();

    void showTimData(const QPointF* points);

    void showTimBlueLeg(const QPointF* points);
    void showTimGreenLeg(const QPointF* points);

    QList<QPoint> cal_leg(int pointlengthX[],int pointlengthY[]);

    void p_drawLine();
    void p_drawLine_Leg();

    void p_manual_drawLine(const std::vector<std::pair<float, uint16_t>>*);
    void p_manual_drawLineUp(const std::vector<std::pair<float, uint16_t>>*);

    void on_Test_Button_clicked();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::Painter *ui;
    bool _saved;
    bool _opened;
    QString _filename;
    QColor Color;
    QColor BGColor;
    QFont _font;

    QTimer *mytimer;
    QTimer *mytimer_draw;
    float scale = 4.0;
    int clean_count =0;
    int mean_count=0;
    QList<CoorStruct> result_points;
};

#endif // PAINTER_H
