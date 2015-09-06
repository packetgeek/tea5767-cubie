#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define CHIP_DEVICE "/dev/i2c-1"
#define DID 0x60

//compile with: gcc -o mute mute.c -lm

// below is somewhat of a kluge in that 
// it reads the chip, does some bit 
// twiddling, and pushes new data back
// to the chip.  A better approach 
// would be to read the chip, mask the 
// mute flag, and push the data back 
// to the chip.  It's on the to do list.

int main( int argc, char *argv[]) {

	unsigned char radio[5] = {0};
	int fd;
	double frequency = 0;
	unsigned char frequencyH = 0;
	unsigned char frequencyL = 0;
	unsigned int frequencyB;

	// open access to the chip
	char *filename = CHIP_DEVICE;
	if((fd=open(filename, O_RDWR)) < 0) exit(1);
	if(ioctl(fd, I2C_SLAVE, DID) < 0) {
		close(fd);
		exit(1);
	}

	// get data from chip
	read(fd,radio,5);

	//determine currently used frequency
  frequency=((((radio[0]&0x3F)<<8)+radio[1])*32768/4-225000)/10000;

	//make human readable
	// double rounding is horrible code but it works
	frequency = round(frequency * 10.0)/1000.0;
	frequency = round(frequency * 10.0)/10.0;

	//optional print out (uncomment if needed)
	//printf ("Frequency = "); printf("%f\n",frequency);

  //figure out what to send back
  frequencyB=4*(frequency*1000000+225000)/32768; //calculating PLL word
  frequencyH=frequencyB>>8;
  frequencyH=frequencyH|0x80;   // mutes the radio
  frequencyL=frequencyB&0XFF;

  //load the above into the array
  radio[0]=frequencyH; //FREQUENCY H
  radio[1]=frequencyL; //FREQUENCY L
  radio[2]=0xB0; //3 byte (0xB0): high side LO injection is on,. 
  radio[3]=0x10; //4 byte (0x10) : Xtal is 32.768 kHz 
  radio[4]=0x00; //5 byte0x00)

  //send the array back
  //write (fd, (unsigned int)radio, 5) ;
  write (fd, radio, 5) ;

  close(fd);
  return 0;
}

