#ifndef DATA_CONTROL_H
#define DATA_CONTROL_H

#include "gps.h"
#include "obd.h"
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QString>

struct ALLDATA
{
    struct GPS gpsinfo;
    struct OBD obdinfo;
};

extern bool status;
extern struct ALLDATA alldata;
extern QString start_d, end_d, start_t, end_t, playback_ret;

void *data_contrl(void *m);

#endif // DATA_CONTROL_H
