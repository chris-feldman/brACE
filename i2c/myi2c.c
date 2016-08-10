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
#include "myi2c.h" 	   		// for Accelerometer

int initiate_i2c(void) { 

	
	// Open Device on Gumstix associated with I2C	 
	fh = open("/dev/i2c-0", O_RDWR);
	if (fh < 0) { 
		printf("Error connecting to I2C bus\n"); 
	} 

	// Check to See if accelerometer is connected		
	read_register(REG_WHOAMI, 1);
	 
	//printf("%x\n", read_buf[0]);
	if (read_buf[0] != 0x1A) {
		printf("Accelerometer not detected\n");
		return -1;
	}
	
	// Activate accelerometer
	write_register(REG_CTRL_REG1,0x01);
	//printf("%x\n", read_buf[0]);
	if (read_buf[0] != 0x01) {
		printf("Accelerometer not activated\n");
		return -1;
	}

	return 0; 
  
}

int close_i2c(void) {
	close(fh);
	return 0; 
} 

int read_acceleration(void) {

	// Read and Parse X,Y,Z acceleration
	read_register(REG_OUT_X_MSB,2);
	x = read_buf[0];
	x <<= 8;
	x |= read_buf[1];
	x >>= 2;
	
	read_register(REG_OUT_Y_MSB,2);
	y = read_buf[0];
	y <<= 8;
	y |= read_buf[1];
	y >>= 2;

	read_register(REG_OUT_Z_MSB,2);
	z = read_buf[0];
	z <<= 8;
	z |= read_buf[1];
	z >>= 2;

	//x = ~x + 1 ;
	//y = ~y + 1;
	//z = ~z + 1;
	

	// Convert to float
	x_g = (float)x/DIVIDER -2;
	y_g = (float)y/DIVIDER -2;
	z_g = (float)z/DIVIDER -2; 

	//printf("%f ", x_g);
	//printf("%f ", y_g);
	//printf("%f\n", z_g);
	
	//sleep(1);
		
	return 0;
}

// Read function to read registers on accelerometer
int read_register(uint8_t reg, int msg_len) {
	
	write_buf[0] = reg;	// Which register to read	
	
	// Repeated Start I2C
	msgs[0].addr  = ACC_ADDRESS; 
	msgs[0].flags = 0; 
	msgs[0].buf   = write_buf; 
	msgs[0].len   = 1; 
	msgs[1].addr  = ACC_ADDRESS; 
	msgs[1].flags = I2C_M_RD; 
	msgs[1].buf   = read_buf; 
	msgs[1].len   = msg_len;

	rdwr.msgs  = msgs; 
	rdwr.nmsgs = 2;

	// Read
	if (ioctl(fh,I2C_RDWR,&rdwr)<0) { 
		printf("Error with I2C READ/WRITE\n"); 
		return -1; 
	}
	return 0; 

}

// Write function to write to registers on accelerometer
int write_register(uint8_t reg, uint8_t msg) {

	write_buf[0] = reg; // Register
	write_buf[1] = msg;	// Command

	// Repeated Start I2C
	msgs[0].addr  = ACC_ADDRESS; 
	msgs[0].flags = 0; 
	msgs[0].buf   = write_buf; 
	msgs[0].len   = 2; 
	msgs[1].addr  = ACC_ADDRESS; 
	msgs[1].flags = I2C_M_RD; 
	msgs[1].buf   = read_buf; 
	msgs[1].len   = 1;

	rdwr.msgs  = msgs; 
	rdwr.nmsgs = 2; 

	// Write
	if (ioctl(fh,I2C_RDWR,&rdwr)<0) { 
		  printf("Error with I2C READ/WRITE\n"); 
		  return -1; 
	}
	return 0; 

}

