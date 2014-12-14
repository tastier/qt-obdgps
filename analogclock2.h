#ifndef ANALOGCLOCK2_H
#define ANALOGCLOCK2_H
#include <QWidget>

class AnalogClock2 : public QWidget
{
    Q_OBJECT

public:
    AnalogClock2(QWidget *parent = 0);
public slots:
    void startValue(int);
    void init(void);
    void setValue();

protected:
    void paintEvent(QPaintEvent *event);
int mValue;
};
#endif // ANALOGCLOCK2_H
