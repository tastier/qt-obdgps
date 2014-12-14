#include <QHBoxLayout>
#include <QTimer>
#include "mainwin.h"
#include "mainwindow.h"
 MainWin::MainWin(QWidget *parent)
     : QWidget(parent), slider(Qt::Vertical)
 {



     clock.init();
 QVBoxLayout* layout = new QVBoxLayout(this);
 layout->addWidget(&clock);

 clock2.init();
layout->addWidget(&clock2);


 }


void MainWin::doValueChanged()
{
 //clock.setValue(slider.value());
    ;
}
