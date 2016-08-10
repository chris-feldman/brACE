/*
Emily Lam & Chris Feldman
Project brACE by Team ACE
EC535 -- Professor Coskun

Main function -- Reads Values from accelerometer registers, alerts Android App,
and implements slouch and stand algorithm
*/

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "linux/i2c.h"
#include "linux/i2c-dev.h" 			// for I2C_SLAVE 
#include "MMA8451.h" 
#include "myi2c.h"
#include <math.h>
#include <signal.h>

// Defines for threshold changes.
#define DELTA_SLOUCH 	 0.06
#define DELTA_STAND 	 0.15


// Declare Variables
void sighandler(int);				// Sig Handler for Button Interrupt
char line[256];						// Msg Buffer
int slouch = 0;						// Slouch Flag
int count = 0;						// Count to keep track of constant motion duration
float x_default;					// Default x value
float x_, y_, z_;					// current x,y,z value
int pFile = NULL;					// used to open bluetooth device
char s_msg[] = "slouch\n";			// Slouch message sent to Android App
char r_msg[] = "reset\n";			// Reset message sent to Android App


int main(void){

	
	// Declare more variables 
	struct sigaction action;		// Related to SIGIO
	int oflags;						// Related to SIGIO

	int pFile2;						// used to open mygpio character device
	
	// Open mygpio character device.
	pFile2 = open("/dev/mygpio", O_RDWR);
	if (pFile2 < 0) {
		fprintf (stderr, "mygpio module isn't loaded\n");
		return 1;
	}

	// Initiate i2C
	initiate_i2c();
	
	// Create SIGIO
	memset(&action, 0, sizeof(action));
	action.sa_handler = sighandler;
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&action.sa_mask);
	sigaction(SIGIO, &action, NULL);
	fcntl(pFile2, F_SETOWN, getpid());
	oflags = fcntl(pFile2, F_SETFL, oflags | FASYNC);

	// Pause and wait for SIGIO
	pause();

	// Update current x,y,z values
	x_ = x_g;
	y_ = y_g;
	z_ = z_g;	
	
	// Loop
	while (1) {

		
		// Open bluetooth device
		if (pFile != NULL) {
		close(pFile);
		pFile = NULL;
		}
		pFile = open("/dev/rfcomm0", O_RDWR);		
		if (pFile < 0) {
			fprintf (stderr, "rfcomm0 module isn't loaded\n");
		} 

		else {
			// Read Acceleration
			read_acceleration();
			
			// If drastic motion in y and z axis, ignore.
			if (abs(y_ - y_g) + 0.1 < abs(x_ - x_g) /*|| abs(z_g - z_) + 0.1 > abs(x_ - x_g)*/) {

				//IGNORE MOVEMENT
				printf( "Y CHANGE %.2f, X CHANGE %.2f\n", abs(y_ - y_g) + 0.1, abs(x_ - x_g));

			} else {

				// Check that x deviation only
				if ( (x_g > x_default + DELTA_SLOUCH || x_g < x_default - DELTA_SLOUCH)  && slouch == 0) {
					printf("Slouch\n");
					// Slouch state
					slouch = 1;
					// Send message to android
					write(pFile,s_msg,sizeof(s_msg));	
					//sleep(1);
					//slouch = 0;
				}
	
				// Check if slouch state, if so, check for stand.
				if (slouch == 1 && x_g > x_default + DELTA_STAND) {
					count++;
					//printf("%d\n", count);
					// Make sure it is a stand and not a rapid movement.
					if (count > 70) {
						printf("reset\n");
						// Reset state
						slouch = 0;
						// Send message to Android
						write(pFile,r_msg,sizeof(r_msg));
						// Wait for button interrupt for reset of cycle.
						pause();
						count = 0;
					}
	
				}
			}
			
			// update x, y, z
			x_ = x_g;
			y_ = y_g;
			z_ = z_g;
			
		}
		// Close bluetooth
		close(pFile);
		pFile = NULL;
		
	}
	// close i2c
	close_i2c();	
	

	return 0;

}

// Handles SIGIO and resets on interrupt
void sighandler(int signo)
{
	if (pFile = NULL) {	
	pFile = open("/dev/rfcomm0", O_RDWR);
		if (pFile < 0) {
			fprintf (stderr, "rfcomm0 module isn't loaded\n");
		}
	}
	write(pFile,r_msg,sizeof(r_msg)); 
	read_acceleration();
	x_default = x_g;	
	printf("button, default x: %.3f\n",x_default);
	slouch = 0;
}





