#include "data_control.h"

bool status = false; // false means real-time and true means history
QString playback_ret;
QString start_d, end_d, start_t, end_t;
struct ALLDATA alldata;

static QString display_histry(QTextStream & , const QString & , const QString & , const QString & , const QString & , struct ALLDATA *);

void *data_contrl(void *m)
{
    QFile file("data.txt");
    if(!file.open(QFile::ReadWrite | QFile::Append | QFile::Text))
    {
        std::cout<<"open file error"<<std::endl;
    }

    QTextStream stream(&file);

    //// mutex

    while (1) {

        if(status == false){

            // display real-time data
            alldata.gpsinfo = gps_info;
            alldata.obdinfo = obd_info;

            // storing the real-time data, 19 kinds of data
            stream << "date:" << gps_info.date
                   << "time:" << gps_info.time_hh << ":" << gps_info.time_min << ":" << gps_info.time_sec << "   " // 8
                   << "dir:" << gps_info.direction << "      " //7
                   << "lat:" << gps_info.latitude << "      "  //7
                   << "lon:" << gps_info.longitude << "      " //7
                   << "spe:" << gps_info.speed << "   " //4
                   << "ect:" << obd_info.ect << "   " //3
                   << "rpm:" << obd_info.rpm << "     " //5
                   << "vss:" << obd_info.vss << "   "  //3
                   << "imap:" << obd_info.imap << "   " //3
                   << "iat:" << obd_info.iat << "  "
                   << "DTC:" << obd_info.num_DTC << " " //1
                   << "fss:" << obd_info.fss << "  " //2
                   << "fcp:" << obd_info.fcphk << "    " //4
                   << "ful:" << obd_info.ful_cspt << "      " //6
                   << "km:" << obd_info.kmeters << "     " //5
                   << "acc:" << obd_info.accelt << "     " << endl; //5
            stream.flush();
            sleep(1);
        }
        else if(status == true){
            playback_ret = "";
            sleep(1);

            // display the history datas, 14 kinds of data
            playback_ret = display_histry(stream, start_d, end_d, start_t, end_t, &alldata);
            stream.seek(file.size()); // move file pointer to the end of file
            status = false; // display real-time data next
        }

    }

    file.close();
    return (void *)0;
}

static QString display_histry(QTextStream & data_operate, const QString & start_date, const QString & end_date, const QString & start_time, const QString & end_time, struct ALLDATA *p)
{
    QString time, dir, lat, lon, spe, ect, rpm, vss, imap, DTC, fss, fcp, ful, km, acc; // 14 kinds
    int start_positon, tmp_position, end_position;
    bool ok;

    data_operate.seek(0);
    QString history = QString(data_operate.readAll());

    start_positon = history.indexOf("date:" + start_date + "time:" + start_time); // position points to date:
    if(start_positon == -1){
        return "can't find the start time!";
    }

    end_position = history.indexOf("date:" + end_date + "time:" + end_time);
    if(end_position == -1){
        return "can't find the end time!";
    }

    tmp_position = start_positon;
    while(status == true){

        tmp_position = history.indexOf("time:", tmp_position);
        time = history.mid(tmp_position + 5, 8); // 13=5+8
        time.remove(QChar(' '));
        // judge the end of time
        if(QString::compare(time, end_time, Qt::CaseSensitive) == 0)
            break;

        tmp_position = history.indexOf("dir:", tmp_position); // tmp_position points to dir:
        dir = history.mid(tmp_position + 4, 7);
        dir.remove(QChar(' '));
        p->gpsinfo.direction = dir.toDouble();

        tmp_position = history.indexOf("lat:", tmp_position); // tmp_position points to lat:
        lat = history.mid(tmp_position + 4, 7);
        lat.remove(QChar(' '));
        p->gpsinfo.latitude = lat.toDouble();

        tmp_position = history.indexOf("lon:", tmp_position); // tmp_position points to lon:
        lon = history.mid(tmp_position + 4, 7);
        lon.remove(QChar(' '));
        p->gpsinfo.longitude = lon.toDouble();

        tmp_position = history.indexOf("spe:", tmp_position); // tmp_position points to spe:
        spe = history.mid(tmp_position + 4, 4);
        spe.remove(QChar(' '));
        p->gpsinfo.speed = spe.toDouble();

        tmp_position = history.indexOf("ect:", tmp_position);
        ect = history.mid(tmp_position + 4, 3);
        ect.remove(QChar(' '));
        p->obdinfo.ect = ect.toInt(&ok, 10);

        tmp_position = history.indexOf("rpm:", tmp_position);
        rpm = history.mid(tmp_position + 4, 5);
        rpm.remove(QChar(' '));
        p->obdinfo.rpm = rpm.toInt(&ok, 10);

        tmp_position = history.indexOf("vss:", tmp_position);
        vss = history.mid(tmp_position + 4, 3);
        vss.remove(QChar(' '));
        p->obdinfo.vss = vss.toInt(&ok, 10);

        tmp_position = history.indexOf("imap:", tmp_position);
        imap = history.mid(tmp_position + 5, 3);
        imap.remove(QChar(' '));
        p->obdinfo.imap = imap.toInt(&ok, 10);

        tmp_position = history.indexOf("DTC:", tmp_position);
        DTC = history.mid(tmp_position + 4, 1);
        DTC.remove(QChar(' '));
        p->obdinfo.num_DTC = DTC.toInt(&ok, 10);

        tmp_position = history.indexOf("fss:", tmp_position);
        fss = history.mid(tmp_position + 4, 2);
        fss.remove(QChar(' '));
        p->obdinfo.fss = fss.toInt(&ok, 10);

        tmp_position = history.indexOf("fcp:", tmp_position);
        fcp = history.mid(tmp_position + 4, 4);
        fcp.remove(QChar(' '));
        p->obdinfo.fcphk = fcp.toFloat();

        tmp_position = history.indexOf("ful:", tmp_position);
        ful = history.mid(tmp_position + 4, 6);
        ful.remove(QChar(' '));
        p->obdinfo.ful_cspt = ful.toFloat();

        tmp_position = history.indexOf("km:", tmp_position);
        km = history.mid(tmp_position + 3, 5);
        km.remove(QChar(' '));
        p->obdinfo.kmeters = km.toFloat();

        tmp_position = history.indexOf("acc:", tmp_position);
        acc = history.mid(tmp_position + 4, 5);
        acc.remove(QChar(' '));
        p->obdinfo.accelt = acc.toFloat();

        // history time +1s every loop
        sleep(1);
    }

    return "playback successful!";
}
