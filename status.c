#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define CHIP_DEVICE "/dev/i2c-1"
#define DID 0x60

//compile with gcc -o status status.c -lm

int main( int argc, char *argv[]) {

  unsigned char radio[5] = {0};
  int fd;
  double frequency = 0;

        // open access to the chip
        char *filename = CHIP_DEVICE;
        if((fd=open(filename, O_RDWR)) < 0) exit(1);
        if(ioctl(fd, I2C_SLAVE, DID) < 0) {
                close(fd);
                exit(1);
        }

  read(fd,radio,5);

  close(fd);

  frequency=((((radio[0]&0x3F)<<8)+radio[1])*32768/4-225000)/10000;
  frequency = round(frequency * 10.0)/1000.0;
  frequency = round(frequency * 10.0)/10.0;
  printf ("Frequency = "); printf("%f\n",frequency);

  return 0;
}
