# brACE Slouch Detection Wearable

By Team ACE: Alexandrea Mellen, Chris Feldman, Emily Lam

December 11th, 2014, EC535 Project -- Professor Coskun

[Demo Video](http://youtu.be/lZ92gk7oeJE)

See November 2015 issue of [Circuit Cellar Magazine](http://circuitcellar.com) for full writeup

## How to Run
### Load Executables onto Gumstix:

1. Make bluetooth

	```
	# cd bluetooth
	# LD_LIBRARY_PATH=$EC535/gumstix/bluez/bluez-libs/usr/lib:$LD_LIBRARY_PATH
	# make
	```

2. Make myi2c

	```
	# cd myi2c
	# make
	```

3. Make button.ko

	```
	# cd kernel
	# make
	```

4. Transfer executables (myi2c, button.ko, bluetooth) and runme.sh to Gumstix

### Load Android App:

1. Import code into Android Studio
2. Run and Deploy to Android smartphone from Android Studio

### Setup:

1. Connect Battery pack to turn on.
2. Connect Serial cable and log on, using minicom. user: root, password: gumstix
3. Run runme.sh script

	```
	# ./runme.sh
	```

	if the runme.sh does not exist or work:

	```
	# mknod /dev/mygpio c 61 0
	# insmod button.ko
	# ./bluetooth &
	# ./myi2c
	```
		
4. Connect to Gumstix using Android App
	* The password to pair the phone with the Gumstix is "BlueZ"
	* Pair them before initializing a connection
5. Unplug serial cable
6. Put around neck with device flush on your back

### Usage:

1. Get into a proper sitting position
2. Press button 3 on button module.
3. Stand up if Android App sets off an Alarm
4. Repeat steps 1-4.
