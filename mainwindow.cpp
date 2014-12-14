#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwin.h"
#include "data_control.h"
#include <QtCore>
#include<sstream>

//int newvalue;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_12->setText("real-time");
    ui->dateTimeEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit_2->setDate(QDate::currentDate());

    QTimer *timer = new QTimer(this);   //新建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(UpDate()));//关联定时器计满信号和相应的槽函数
    timer->start(200);//定时器开始计时，其中1表示1ms
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpDate()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss dddd");//设置系统时间显示格式
    ui->label_8->setText(strTime);//在标签上显示时间
    char str[128];

    sprintf(str, "%d", alldata.obdinfo.vss); //将speed转为字符串
    ui->textBrowser->setText(str);
    sprintf(str, "%.2f", alldata.gpsinfo.speed); //将speed转为字符串
    ui->textBrowser_8->setText(str);
    sprintf(str, "%f", alldata.gpsinfo.latitude);
    ui->textBrowser_2->setText(str);    //latitude
    sprintf(str, "%f", alldata.gpsinfo.longitude);
    ui->textBrowser_3->setText(str);    //longitude

    sprintf(str, "%f", alldata.gpsinfo.direction);
    ui->textBrowser_4->setText(str);    //direction


    sprintf(str, "%5.2f", alldata.obdinfo.fcphk);
    ui->textBrowser_5->setText(str);

    sprintf(str, "%d", alldata.obdinfo.ect);
    ui->textBrowser_6->setText(str);    //temperature

    sprintf(str, "%d", alldata.obdinfo.num_DTC);
    ui->textBrowser_7->setText(str);    //num_DTC

    sprintf(str, "%d", alldata.obdinfo.imap);
    ui->textBrowser_10->setText(str);    //in_manifold_pre

    sprintf(str, "%4.1f", alldata.obdinfo.accelt);
    ui->textBrowser_11->setText(str);    //in_air_temp

    sprintf(str, "%6.4f", alldata.obdinfo.ful_cspt);
    ui->textBrowser_12->setText(str);    //throttle

    sprintf(str, "%6.3f", alldata.obdinfo.kmeters);
    ui->textBrowser_13->setText(str);

    //alldata.obdinfo.fss
    switch(alldata.obdinfo.fss){
    case 1: strcpy(str,"Open loop due to insufficient engine temperature");break;
    case 2: strcpy(str,"Closed loop, using oxygen sensor feedback to determine fuel mix");break;
    case 4: strcpy(str,"Open loop due to engine load OR fuel cut due to deceleration");break;
    case 8: strcpy(str, "Open loop due to system failure");break;
    case 16: strcpy(str,"Closed loop, using at least one oxygen sensor but there is a fault in the feedback system");break;
    default:break;
    }
    ui->textBrowser_14->setText(str);    //fuel_system1_status


    if(status == false){
        ui->label_12->setText("real-time data");
    }
    else if(status == true){
        ui->label_12->setText("history data");

        QDate start_date = ui->dateTimeEdit->date();
        QTime start_time = ui->dateTimeEdit->time();
        start_d = start_date.toString("dMyy");
        start_t = start_time.toString("H:m:s");

        QDate end_date = ui->dateTimeEdit_2->date();
        QTime end_time = ui->dateTimeEdit_2->time();
        end_d = end_date.toString("dMyy");
        end_t = end_time.toString("H:m:s");
    }

    ui->label_16->setText(playback_ret);

}
//start
void MainWindow::on_pushButton_clicked()
{
    status = true;
}
//end
void MainWindow::on_pushButton_2_clicked()
{
    status = false;
}
