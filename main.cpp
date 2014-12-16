#include "mainwindow.h"
#include "mainwin.h"
#include "obd.h"
#include "gps.h"
#include "data_control.h"

#include <QApplication>
#include <QUrl>

int main(int argc, char *argv[])
{
    pthread_t obd, contrl;

//    // obd thread
//    if((pthread_create(&obd, NULL, obd_thread, NULL)) == -1)
//    {
//        perror("Create OBD thread error !\n");
//        exit(-1);
//    }

//    //gps thread
    cthread_gps();

    // data_contrl thread
    if((pthread_create(&contrl, NULL, data_contrl, NULL)) == -1)
    {
        perror("data_contrl thread error !\n");
        exit(-1);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    pthread_join(obd, NULL);
//    pthread_join(pgps_t, NULL);
//    pthread_join(stored, NULL);

    return a.exec();
}
