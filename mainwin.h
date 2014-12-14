#ifndef MAINWIN_H
#define MAINWIN_H
#include <QSlider>
#include <QWidget>
#include "analogclock.h"
#include "analogclock2.h"

class MainWin: public QWidget
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = 0);

protected slots:
    void doValueChanged();
private:
QSlider slider;
AnalogClock clock;
AnalogClock2 clock2;
};


#endif
