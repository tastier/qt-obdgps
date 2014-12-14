#include "gps.h"

pthread_t pgps_t;
int fd_gps;//File descriptor
struct GPS gps_info;//Global GPS information
const char *dev_path = "/dev/ttyACM0";//GPS device file path

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;
    if(tcgetattr(fd,&oldtio)!=0)
    {
        perror("SetupSerial 1\n");
        return -1;
    }
    memset(&newtio,0,sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch(nEvent)
    {
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch(nSpeed)
    {
        case 4800:
            cfsetispeed(&newtio,B4800);
            cfsetospeed(&newtio,B4800);
            break;
        case 9600:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
        default:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
    }

    if(1==nStop)
    newtio.c_cflag &= ~CSTOPB;
    else if (2==nStop)
    newtio.c_cflag |= CSTOPB;

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 100;

    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("Com set error\n");
        return -1;
    }
  //  printf("set done!\n");
    return 0;
} //set_opt

int set_para()
{
    int set_serial;
    fd_gps  = open(dev_path,O_RDWR|O_NONBLOCK);
    if(-1 == fd_gps )
    {
        perror("Error opening gps device\n");
        return -1;
    }
    set_serial = set_opt(fd_gps,9600,8,'N',1); //set serial parameters
    if(-1 == set_serial)
    {
        perror("Error setting serial port\n");
        return -1;
    }
    return 0;
}

struct GPS reading_gps()
{

   char* commaPos[13] = {0};
   char *gprmc = NULL;
   char buff_gps[BUF_SIZE];
   int Gtime;
   int Gtime_hh;

   memset(buff_gps,0,sizeof(buff_gps));
   read(fd_gps,buff_gps,BUF_SIZE);
   if((gprmc = strstr(buff_gps,"$GPRMC")) != NULL)
   {
       int nthComma = 0;
       char *p;
       for(p=gprmc;(*p!='*');p++)
       {
           if ((*p!=','))
                continue;
           nthComma++;
           commaPos[nthComma] = p;
       }

       Gtime = (int)atof(commaPos[1]+1);
	   if ((floor(Gtime / 10000) + 8) > 24) {
		   Gtime_hh = (int) (floor(Gtime / 10000) + 8 - 24);
	   }else{
		   Gtime_hh = (int) (floor(Gtime / 10000) + 8);
	   }

	   gps_info.time_hh    = Gtime_hh;
	   gps_info.time_min   = (int) (floor(Gtime % 10000) / 100);
	   gps_info.time_sec   = (int) floor(Gtime % 100);
       gps_info.status     = *(commaPos[2]+1);
       gps_info.latitude   = atof(commaPos[3]+1)/100;
       gps_info.lat_N_S    = *(commaPos[4]+1);
       gps_info.longitude  = atof(commaPos[5]+1)/100;
       gps_info.lng_E_W    = *(commaPos[6]+1);
       gps_info.speed      = atof(commaPos[7]+1)*1.852;
       gps_info.direction  = atof(commaPos[8]+1);
       gps_info.date       = atoi(commaPos[9]+1);

       if((gps_info.latitude-floor(gps_info.latitude))*100<60.0)
           gps_info.latitude = floor(gps_info.latitude) + ((gps_info.latitude-floor(gps_info.latitude)) * 100)/60.0;
       else
           gps_info.latitude = floor(gps_info.latitude) + ((gps_info.latitude-floor(gps_info.latitude)) * 10)/60.0;

       if((gps_info.longitude-floor(gps_info.longitude))*100<60.0)
           gps_info.longitude = floor(gps_info.longitude) + ((gps_info.longitude-floor(gps_info.longitude)) * 100)/60.0;
       else
           gps_info.longitude = floor(gps_info.longitude) + ((gps_info.longitude-floor(gps_info.longitude)) * 10)/60.0;

   }
   return gps_info;
}
//void display_gps()
//{
//    printf("-------------------GPS-------------------------\n");
//    printf("This is current GPS information:\n");
//    printf("Time     : %f\n", gps_info.time);
//    printf("Status   : %c (A stands for Available)\n", gps_info.status);
//    printf("Latitude : %f\n", gps_info.latitude);
//    printf("N or S   : %c\n", gps_info.lat_N_S);
//    printf("Longitude: %f\n", gps_info.longitude);
//    printf("E or W   : %c\n", gps_info.lng_E_W);
//    printf("Speed    : %f\n", gps_info.speed);
//    printf("Direction: %f (degree, compared with North)\n", gps_info.direction);
//    printf("Date     : %u\n", gps_info.date);
//    printf("-----------------------------------------------\n");
//
//}

void * thread_reading(void *ctmp)
{
	set_para(); ////

    //Class_GPS *p = (Class_GPS *)ctmp;//Need to get this pointer to use the non-static member
    while (1)
    {
        reading_gps();

//		test();

        //usleep(p->interval);
        //printf("Thread running\n");
        usleep(GPS_INTERVAL);
//		usleep(400000);
    }
    return ((void *)0);
}

void cthread_gps()
{
    if((pthread_create(&pgps_t, NULL, thread_reading, NULL)) == -1)
    {
        perror("Create GPS thread error in FUNC cthread_gps in class Class_GPS!\n");
        exit(-1);
    }
}

//struct GPS test()
//{
//    gps_info.time = 164620.00;
//    gps_info.status = 'A';
//    gps_info.latitude = 39.57;
//    gps_info.lat_N_S = 'N';
//    gps_info.longitude = 116.21;
//    gps_info.lng_E_W = 'E';
//    gps_info.speed = 0.058;
//    gps_info.direction = 0;
//    gps_info.date = 071014;
//    return gps_info;
//}

void modify()
{
    gps_info.speed = 1;
}
