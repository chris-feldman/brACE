/*
Emily Lam
Project brACE by Team ACE
EC535 -- Professor Coskun
Reference -- Adafruit_MMA8451_Library

User Level code to interact with the MMA8451 accelerometer
*/

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "linux/i2c.h"
#include "linux/i2c-dev.h" 		// for I2C_SLAVE 
#include "MMA8451.h" 	   		// for Accelerometer

// I2C read/write functions
int read_register(uint8_t reg, int msg_len);
int write_register(uint8_t reg, uint8_t msg);
int initiate_i2c(void);
int close_i2c(void);
int read_acceleration(void);

// Global Variables
uint16_t x, y, z;
float x_g, y_g, z_g;
	
// Setup I2C and buffers on gumstix 	
struct i2c_msg msgs[2]; 
struct i2c_rdwr_ioctl_data rdwr; 
uint8_t write_buf[8]; 
uint8_t read_buf[8];
int fh;
