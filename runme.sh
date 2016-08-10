mknod /dev/mygpio c 61 0
insmod button.ko
./bluetooth &
./myi2c
