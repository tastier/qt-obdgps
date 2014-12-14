#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H
#include <QWidget>

 class AnalogClock : public QWidget
 {
     Q_OBJECT

 public:
     AnalogClock(QWidget *parent = 0);
 public slots:
     void startValue(int);
     void init(void);
     void setValue();


 protected:
     void paintEvent(QPaintEvent *event);
 int mValue;
 };
#endif // ANALOGCLOCK_H
