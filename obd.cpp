#include "obd.h"

struct OBD obd_info;
//float ltft = 0;
//int rpm=0, ect=0, vss=0, num_DTC=0, iat=0, fuel_system1_status=0, imap=0;
//float fcphk = 0; // Instantaneous fuel consumption per hundred kilometers
//float ful_cspt = 0; // fuel consumption
//float kmeters = 0;
//float accelt = 0; // acceleration

/*
 *open the uart file
 */
static int open_uart( char *dev)
{
    int fd;
    fd = open( dev, O_RDWR );
    if (fd == -1)
    {
        printf("Can't Open OBD Serial Port!\n");
        exit(1);
    }
    else{
        printf("Open obd serial port successfully!\n");
        return fd;
    }
}
/*
 * uart init  8.N.1
 */
static void init_uart( int fd )
{
    struct  termios options;
    tcgetattr(fd, &options);

    /*  baud rate 9600Bps */
    cfsetispeed(&options,B38400);
    cfsetospeed(&options,B38400);

    /*  8 data bit  */
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    /*no flow control--no parity--1 stop bit*/
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;

    /* raw mode */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    tcflush(fd,TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
       printf("Setting obd serial port in error!\n");
       exit(1);
    }
    else{
        printf("Initialize obd serial successfully!\n");
    }
}
/*
 * function will clear buff before reading
 */
static void read_uart( int fd, char *buff, int bufflen)
{
    int nread = 0;
    int tmpLen = 0;
    memset( buff, 0, bufflen);

    while ((nread = read(fd, buff + tmpLen, 512)) > 0) //block
    {
        tmpLen += nread;
        if (buff[tmpLen - 1] == '>') ////determined by your project, detect it!!!
        {
            break;
        }
    }
    if (nread<=0)
    {
      printf("Read obd data in error!\n");
    }
}
/*
 * init obd
 */
static int init_obd( int fd)
{
    int flag = 0;
    char buff[64] = "";
    char buf_atz[] = "ATZ\r";
    char buf_ate0[] = "ATE0\r";
    char buf_atsp0[] = "ATSP0\r";
    char buf_connect[] = "0100\r";//test obd connection

    while ( flag != 4)
    {
        switch (flag)
        {
        case 0:{
                    if( write(fd, buf_atz, sizeof(buf_atz)) == sizeof(buf_atz)){
                        read_uart( fd, buff, 64);
                        if (( strncmp(buff, "ATZ\n", 4) == 0) || ( strncmp(buff, "\n\nELM", 5) == 0)){
                            flag = 1;
                            printf("Write ATZ successful!\n");
                        }
                    }
                    else{
                        printf("write ATZ in error!\n");
                    }
                    break;
                }
        case 1:	{
                    if( write(fd, buf_ate0, sizeof(buf_ate0)) == sizeof(buf_ate0)){
                        read_uart( fd, buff, 64);
                        if ( strncmp(buff, "ATE0\n", 5) == 0){
                            flag = 2;
                            printf("Write ATE0 successful!\n");
                        }
                    }
                    else
                        printf("write ATE0 in error\n");
                    break;
                }
        case 2:{
                    if( write(fd, buf_atsp0, sizeof(buf_atsp0)) == sizeof(buf_atsp0)){
                        read_uart( fd, buff, 64);
                        if ( strncmp(buff, "OK", 2) == 0){
                            flag = 3; // obd init over
                            printf("Write ATSP0 successful!\n");
                        }
                    }
                    else
                        printf("write ATSP0 in error\n");
                    break;
                }
        case 3:{
                    if( write(fd, buf_connect, sizeof(buf_connect)) == sizeof(buf_connect)){
                        read_uart( fd, buff, 64);
                        if (( strncmp(buff, "41 00", 5) == 0) || (strncmp(buff, "SEARCH", 6) == 0)){
                            flag = 4;
                            printf("Write 4100 successful!\n");
                        }
                    }
                    else
                        printf("write 4100 in error\n");
                    break;
                }
        default: break;
        }
    }
    printf("Initialize obd successfully!\n");
    return 0;
}
/*
*convert one byte from hex to decimal
*parameter: c0 is the high four bits of the byte and c1 is low
*/
static int hex2dec( char c0, char c1)
{
    char c[2] = {c0, c1};
    int num[2];
    int result, i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] >= '0' && c[i] <= '9') {
            num[i] = c[i] - 48;
        }
        else if(c[i] >= 'A' && c[i] <= 'F'){
            num[i] = c[i] - 55;
        }
    }
    result = num[0]*16 + num[1];
    return result;
}
static int Engine_rpm_formula(int fd, char *commmand)
{
    int rpm = 0;
    char buff[64] = "";
    int first_byte = 0, second_byte = 0;

    if ( write(fd, commmand, 5) == 5) {
        read_uart( fd, buff, 32);
        if ( strncmp(buff, "41 0C", 5) == 0) {	//you wen ti   || (strncmp(buff, "SEARCH", 6) == 0)
            first_byte = hex2dec( *(buff+6), *(buff+7));
            second_byte = hex2dec( *(buff+9), *(buff+10));
            rpm = ((first_byte*256) + second_byte) / 4;
            return rpm;
        }
//		else if (strncmp(buff, "SEARCH", 6) == 0) {
//
//		}
    }
    else
        return OBD_ERROR;
}
static int Vehicle_speed_formula(int fd, char *command)
{
    int speed = 0;
    char buff[64] = "";

    if ( write(fd, command, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 0D", 5) == 0) {
            speed = hex2dec( *(buff+6), *(buff+7));
            return speed;
        }
    }
    else
        return OBD_ERROR;
}

static int Engine_coolant_temperature_formula(int fd, char *command)
{
    int temp = 0;
    char buff[64] = "";
    int first_byte = 0;

    if ( write(fd, command, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 05", 5) == 0) {
            first_byte = hex2dec( *(buff+6), *(buff+7));
            temp = first_byte - 40;
            return temp;
        }
    }
    else
        return OBD_ERROR;
}
static int number_of_DTC_formula(int fd, char *commmand)
{
    int num_DTC = 0;
    char buff[64] = "";
    int first_byte = 0;

    if ( write(fd, commmand, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 01", 5) == 0) {
            first_byte = hex2dec( *(buff+6), *(buff+7));
            if (first_byte >= 128) {
                num_DTC = first_byte - 128;
                return num_DTC;
            }
            else{
                return 0;
            }
        }
    }
    else
        return OBD_ERROR;
}
//static float engine_load_value_formula(int fd, char *command)
//{
//	float load_value = 0;
//	char buff[64] = "";
//	int first_byte = 0;
//
//	if ( write(fd, command, 5) == 5) {
//		read_uart( fd, buff, 64);
//		if ( strncmp(buff, "41 04", 5) == 0) {
//			first_byte = hex2dec( *(buff+6), *(buff+7));
//			load_value = first_byte*100.0/255.0;
//			return load_value;
//		}
//	}
//	else
//		return OBD_ERROR;
//}
static int intake_air_temperature_formula(int fd, char *command)
{
    int temperature = 0;
    char buff[64] = "";
    int first_byte = 0;

    if ( write(fd, command, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 0F", 5) == 0) {
            first_byte = hex2dec( *(buff+6), *(buff+7));
            temperature = first_byte - 40;
            return temperature;
        }
    }
    else
        return OBD_ERROR;
}
//static float Throttle_position_formula(int fd, char *command)
//{
//	float position = 0;
//	char buff[64] = "";
//	int first_byte = 0;
//
//	if ( write(fd, command, 5) == 5) {
//		read_uart( fd, buff, 64);
//		if ( strncmp(buff, "41 11", 5) == 0) {
//			first_byte = hex2dec( *(buff+6), *(buff+7));
//			position = first_byte*100.0/255.0;
//			return position;
//		}
//	}
//	else
//		return OBD_ERROR;
//}
//static float Absolute_throttle_position_B_formula(int fd, char *command)
//{
//	float position_B = 0;
//	char buff[64] = "";
//	int first_byte = 0;
//
//	if ( write(fd, command, 5) == 5) {
//		read_uart( fd, buff, 64);
//		if ( strncmp(buff, "41 47", 5) == 0) {
//			first_byte = hex2dec( *(buff+6), *(buff+7));
//			position_B = first_byte*100.0/255.0;
//			return position_B;
//		}
//	}
//	else
//		return OBD_ERROR;
//}
static int fuel_system1_status_formula(int fd, char *command)
{
    int status = 0;
    char buff[64] = "";
//	int first_byte = 0;

    if ( write(fd, command, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 03", 5) == 0) {
            status = hex2dec( *(buff+6), *(buff+7));
            return status;
        }
    }
    else
        return OBD_ERROR;
}
static int Intake_manifold_absolute_pressure_formula(int fd, char *command)
{
    int pressure = 0;
    char buff[64] = "";
//	int first_byte = 0;

    if ( write(fd, command, 5) == 5) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 0B", 5) == 0) {
            pressure = hex2dec( *(buff+6), *(buff+7));
            return pressure;
        }
    }
    else
        return OBD_ERROR;
}
static float LTFT_formula(int fd, char *command, int len_cmd)
{
    float ltft;
    char buff[64] = "";
    int first_byte;

    if ( write(fd, command, len_cmd) == len_cmd) {
        read_uart( fd, buff, 64);
        if ( strncmp(buff, "41 07", 5) == 0) {
            first_byte = hex2dec( *(buff+6), *(buff+7));
            ltft = (first_byte - 128) * 100.0 / 128.0;
            return ltft;
        }
    }
    else
        return OBD_ERROR;
}
static void read_obd( int fd, struct OBD *p)
{
    float ltft = 0;
    int rpm=0, ect=0, num_DTC=0, iat=0, fss=0, imap=0;
    float fcphk = 0; // Instantaneous fuel consumption per hundred kilometers
    float ful_cspt = 0; // fuel consumption
    float kmeters = 0;
    float accelt = 0; // acceleration

    float rep_maf = 0, mpg = 0;
    struct timeval start, end;
    unsigned long timeuse = 0;
    int vss_pre = 0, vss_now = 0;
    float meters_now = 0;

    vss_now = Vehicle_speed_formula(fd,"010D\r");
    while (1)
    {
        vss_pre = vss_now;

        gettimeofday( &start, NULL );
        ect = Engine_coolant_temperature_formula(fd, "0105\r");
        num_DTC = number_of_DTC_formula(fd, "0101\r");
        fss = fuel_system1_status_formula(fd, "0103\r");

        ltft = LTFT_formula(fd, "0107\r", 5);
        imap = Intake_manifold_absolute_pressure_formula(fd, "010B\r");
        iat = intake_air_temperature_formula(fd, "010F\r");
        rpm = Engine_rpm_formula(fd,"010C\r");
        vss_now = Vehicle_speed_formula(fd,"010D\r"); //Km/h

        if (rpm == 0) {
            fcphk = 0;
            accelt = 0;
            continue;
        }

        /* mpg ----- miles per gallon
         * http://www.mp3car.com/engine-management-obd-ii-engine-diagnostics-etc/75138-calculating-mpg-from-vss-and-maf-from-obd2.html
         */
        rep_maf = (iat + 273.15) / (float)(rpm * imap) * 12000.0 / (VE * ED) * (R / MM) ; // 1/maf   Reciprocal of maf
        mpg = 7.107 * vss_now * rep_maf * (1 + ltft / 100.0);
        if (mpg != 0) {
            fcphk = (OHK * G2L) / (mpg * M2KM); // L/100Km
        }
        else {
            fcphk = 0;
        }

        gettimeofday( &end, NULL );
        timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; //us

        // convert the units and calculate the mileage, fuel consumption
        accelt = (vss_now - vss_pre) / MS2KMH / (timeuse / 1000000.0); // m/s2
        meters_now = (timeuse / 1000000.0) * (vss_pre + vss_now) / (MS2KMH * 2.0); // m
        kmeters = kmeters + meters_now / 1000.0; // Km
        if (mpg != 0) {
            ful_cspt = ful_cspt + (meters_now * G2L) / (mpg * M2M); // L
        }

        p->ect = ect;
        p->accelt = accelt;
        p->fcphk = fcphk;
        p->fss = fss;
        p->ful_cspt = ful_cspt;
        p->iat = iat;
        p->imap = imap;
        p->kmeters = kmeters;
        p->ltft = ltft;
        p->num_DTC = num_DTC;
        p->rpm = rpm;
        p->vss = vss_now;

//		printf(" %3d \t %5d \t %3d \t %3d \t %3d \t %4.2f \t %5.2f \t %5.2f \t %6.3f \t %4.1f \n",
//			   ect,rpm,vss,imap,iat,
//			   ltft,fcphk,ful_cspt,kmeters,accelt);
    }
}
void *obd_thread(void *m)
{
    int fd;
    char *dev = "/dev/ttyUSB1";

    fd = open_uart(dev);
    init_uart( fd);
    init_obd( fd);
//	usleep(500000);

    printf("Start to read obd data!\n");
    read_obd( fd, &obd_info);

    close(fd);
}
