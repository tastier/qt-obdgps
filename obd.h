#ifndef OBD_H
#define OBD_H

#include     <stdlib.h>
#include	 <stdio.h>
#include 	 <string.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <termios.h>
#include     <errno.h>
#include 	 <pthread.h>
#include 	 <sys/time.h>

#define 	OBD_ERROR		-1
#define 	VE 				90.0 		// volumetric efficiency 	UNIT:%
#define 	ED  			1.6 	// engine displacement  	UNIT:liters
#define 	MM 				28.97	// the average molecular mass of air	g/mole
#define 	R 				8.314   // J/K/mole
#define 	M2KM			1.6093  // 1 mile = 1.6093 km
#define 	M2M				1609.3  // 1 mile = 1609.3 meters
#define 	G2L				3.7854	// 1 gallon = 3.7854 liters
#define 	OHK				100.0	// one hundred km
#define 	MS2KMH			3.6		// 1m/s = 3.6Km/h

// 12 members
struct OBD
{
    int ect;
    int rpm;
    int vss;
    int iat;
    int imap;
    int num_DTC;
    int fss; // fuel_system1_status
    float fcphk;
    float ful_cspt;
    float kmeters;
    float accelt;

    float ltft;
};

extern struct OBD obd_info;
//extern float ltft;
//extern int ect,rpm, vss, num_DTC, iat, fuel_system1_status, imap;
//extern float fcphk, ful_cspt;
//extern float kmeters, accelt;

void *obd_thread(void *m);

#endif

