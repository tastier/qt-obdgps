
#include <QtGui>
#include "analogclock2.h"
#include "mainwindow.h"
#include "qtimer.h"
#include "data_control.h"
#include <QtCore>


 AnalogClock2::AnalogClock2(QWidget *parent)
     : QWidget(parent)
 {
      QTimer *atimer=new QTimer(this);
      connect(atimer,SIGNAL(timeout()),this,SLOT(setValue()));
      atimer->start(500);
      setWindowTitle(tr("Analog Clock"));     //设置标题
      setFixedSize(QSize(300,300));           //固定大小

 }
 void AnalogClock2::init()                    //初始化指针
 {
    startValue(0);
 }
 void AnalogClock2::paintEvent(QPaintEvent *)
 {
//设置指针的关键点
     static const QPoint hourHand[8] =
     {
         QPoint(1, -20),
         QPoint(4, -60),
         QPoint(1, -70),
         QPoint(1, -80),
         QPoint(-1, -80),
         QPoint(-1, -70),
         QPoint(-4, -60),
         QPoint(-4,-20)
     };
// 设置颜色
     QColor pointer(200,0,0);
     QColor hourColor(255, 255, 255);
     QColor minuteColor(255,122,29);
     QColor textColor(255, 255, 255);

     int side = qMin(width(), height());                //取最小值
     QPainter painter(this);
     painter.fillRect(rect(), QBrush(Qt::black));       //设置背景颜色
     painter.translate(width() / 2, height() / 2);       //将二分之一宽和高设为原点
     painter.scale(side / 300.0, side / 300.0);          //伸缩图片大小
//设置指针笔形与填充颜色
     painter.setPen(Qt::NoPen);
     painter.setBrush(pointer);
//设置时刻度
     painter.save();
     painter.setRenderHint(QPainter::Antialiasing,true);
     painter.setPen(hourColor);
     for (int i = 0; i < 9; ++i)
    {
         painter.drawLine(-53, 73, -62, 85);
         painter.rotate(36.0);
     }
    painter.restore();
//设置分刻度
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(minuteColor);
     for (int j = 0; j < 8; ++j)
    {
   painter.drawLine(-73, 53, -85, 62);
         painter.rotate(36.0);
     }
    painter.restore();
//设置刻度值
painter.setPen(textColor);
painter.drawText(-61, 57,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(0));//从（x,y）开始,高为30,宽为30
painter.drawText(-90, 15,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(0.5));
painter.drawText(-90, -35,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(1));
painter.drawText(-62, -71,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(1.5));
painter.drawText(-15, -90,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(2));
painter.drawText(32, -71,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(2.5));
painter.drawText(62, -31,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(3));
painter.drawText(60, 20,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(3.5));
painter.drawText(35, 55,30, 30,
         Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(4));
//内圆
      painter.save();
      painter.setRenderHint(QPainter::Antialiasing,true);
      painter.setPen(QPen(Qt::white,2,Qt::SolidLine));
      painter.setBrush(QBrush(Qt::NoBrush));
      painter.drawEllipse(-20,-20,40,40);
      painter.restore();
//画指针
      painter.save();
      painter.setPen(Qt::NoPen);
      painter.setRenderHint(QPainter::Antialiasing,true);
      painter.rotate(180+36+mValue*288/4000);
      painter.drawConvexPolygon(hourHand, 8);
      painter.restore();
//最外面圆弧
    painter.save();
    painter.setPen(QPen(Qt::red,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,-54*16,72*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::yellow,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,18*16,144*16);
    painter.restore();
    painter.save();
    painter.setPen(QPen(Qt::white,3,Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawArc(-105,-105,210,210,162*16,72*16);
    painter.restore();
//内圆内数字
    QFont font;
    font.setPointSize(12);
    setFont(font);
    painter.drawText(-15, -11,30, 30,
                     Qt::AlignHCenter | Qt::AlignTop,
                     QString::number(mValue/1000.0));
    painter.drawText(-23, 90,50, 30,
                     Qt::AlignHCenter | Qt::AlignTop,
                     QString("*1000"));

}

void AnalogClock2::startValue(int startvalue)
{
 mValue= startvalue;
 update();
}

void AnalogClock2::setValue()
{
 mValue = alldata.obdinfo.rpm; //
 update();
}


