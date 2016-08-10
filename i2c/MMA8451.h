/*
Emily Lam
Project brACE by Team ACE
EC535 -- Professor Coskun

Protocol and register definitions for MMA8451 accelerometer
*/

#ifndef MMA8451_H
#define MMA8451_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// I2C Slave Adress
#define ACC_ADDRESS 		0x1D		// 0x1C if A pin grounded

// Registers
#define REG_WHOAMI        	0x0D		// Register containing Device ID
#define REG_CTRL_REG1		0x2A		// Register for activating accelerometer
#define REG_OUT_X_MSB     	0x01		// Register containing x acceleration
#define REG_OUT_Y_MSB		0x03		// Register containing y acceleration
#define REG_OUT_Z_MSB		0x05		// Register containing z acceleration

// Varibles 
#define DIVIDER			4096		// This divider is associated with the +/- 2G range


#endif
