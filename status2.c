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

  // set up misc. variables

  unsigned char radio[5] = {0}; 
  int fd;
  double frequency = 0;

  // following to be used later to test flags

  int bit0 = 1;
  int bit1 = 2;
  int bit2 = 4;
  int bit3 = 8;
  int bit4 = 16;
  int bit5 = 32;
  int bit6 = 64;
  int bit7 = 128;

  //open access to the board, send error msg and exit if fails
  //otherwise read 5 bytes into the array called radio

        // open access to the chip
        char *filename = CHIP_DEVICE;
        if((fd=open(filename, O_RDWR)) < 0) exit(1);
        if(ioctl(fd, I2C_SLAVE, DID) < 0) {
                close(fd);
                exit(1);
        }

	read(fd,radio,5);
	close(fd);

  //calculate the current frequency (needs cleaning)

  frequency=((((radio[0]&0x3F)<<8)+radio[1])*32768/4-225000)/10000;
  frequency = round(frequency * 10.0)/1000.0;
  frequency = round(frequency * 10.0)/10.0;
  printf ("Frequency = %.1f MHz\n", frequency);

  // test if stereo is being received

  if(radio[2] & bit7) {
	printf("stereo    = yes\n");
  } else {
	printf("stereo    = no\n");
  }

  return 0;
}


