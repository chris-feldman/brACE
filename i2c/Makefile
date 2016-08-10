default: myi2c

myi2c: main.c myi2c.c MMA8451.h
	/ad/eng/courses/ec/ec535/gumstix/oe/cross/bin/arm-linux-gcc main.c myi2c.c -o myi2c 

.PHONY: clean
clean:
	rm myi2c
