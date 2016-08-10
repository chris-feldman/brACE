/*
Chris Feldman
Project brACE by Team ACE
EC535 -- Professor Coskun

*/
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h> 	//from bluez library
#include <bluetooth/rfcomm.h> 		//from bluez library

/*

----------- TO COMPILE -------------

1. Set up the bluetooth path
LD_LIBRARY_PATH=$EC535/gumstix/bluez/bluez-libs/usr/lib:$LD_LIBRARY_PATH

2. Compile
/ad/eng/courses/ec/ec535/gumstix/oe/cross/bin/arm-linux-gcc -o bluetooth bluetooth.c -lbluetooth

------------------------------------

The password to pair the phone with the Gumstix is "BlueZ"
Pair them before initializing a connection

Write /dev/rfcomm0 to send text to the phone. I have been using echo "text" > /dev/rfcomm0,
but you should be able to use the write function.

*/


int main()
{

	struct sockaddr_rc myAddress, remoteAddress;
	int listenfd, client, cc;
	char buf[100];

	//Create RFCOMM Socket
	listenfd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(listenfd < 0){
		perror("RFCOMM Socket");
		exit(1);
	}

	//Setup information for device to be connected
	myAddress.rc_family = AF_BLUETOOTH;
	myAddress.rc_bdaddr = *BDADDR_ANY;
	myAddress.rc_channel = (uint8_t) 3; 	//defining binding port 2

	//bind myAddress to RFCOMM socket
	bind(listenfd, (struct sockaddr *)&myAddress, sizeof(myAddress));
	
	// listen to socket
	listen(listenfd, 3);
	printf("Waiting for connection...\n");

	//accept the requested connection
	//because of *BDADDR_ANY, this will accept the first connection request it gets
	int fsize = sizeof(remoteAddress);
	client = accept(listenfd, (struct sockaddr *)&remoteAddress, &
	fsize);
	memset(buf, 0, sizeof(buf));

	
	//ba2str takes 6-byte remoteAddress.rc_bdaddr and copy it into buf
	cc = ba2str( & remoteAddress.rc_bdaddr, buf );
	if(cc<0){
		perror("ba2Str");
	}
	printf("Connected to '%s'\n",buf);

	//as necessary, information can be written to /dev/rfcomm0
	


	//close connection
	close(client);
	close(listenfd);
	return 0;
}

	

