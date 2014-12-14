#ifndef GPS_H
#define GPS_H

#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define BUF_SIZE 1024
#define GPS_INTERVAL 10000//Interval of reading(us)

// 11 members
struct GPS
{
    unsigned int		date;
    int					time_hh;
	int					time_min;
	int					time_sec;
    double  			latitude;
    double				longitude;
    double				speed;
    double  			direction;

    char				lat_N_S;
    char				lng_E_W;
    char				status;

};

/*
 * "-------------------GPS-------------------------\n   \
     This is current GPS information:\n                 \
     Time     : %f\n                                    \
     Status   : %c (A stands for Available)\n           \
     Latitude : %f\n                                    \
     N or S   : %c\n                                    \
     Longitude: %f\n                                    \
     E or W   : %c\n                                    \
     Speed    : %f\n                                    \
     Direction: %f (degree, compared with North)\n      \
     Date     : %u\n                                    \
    -----------------------------------------------\n"
*Paras:
* Cgps.time, Cgps.status, Cgps.latitude, Cgps.lat_N_S, Cgps.longitude, Cgps.lng_E_W, Cgps.speed, Cgps.direction, Cgps.date
*/
extern pthread_t pgps_t;
extern int fd_gps;//File descriptor
extern struct GPS gps_info;//Global GPS information
extern const char *dev_path;//GPS device file path

void cthread_gps();//Create thread to read GPS continously
void display_gps();
struct GPS test();//A set of test GPS data(The same format as reading from device)
void modify();//Modify the GPS data in debug stage
struct GPS reading_gps();//Reading GPS from GPS device
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
void *thread_reading(void *arg);


int set_para();//Set GPS device parameters


#endif // GPS_H
