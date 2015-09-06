#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> //required
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define CHIP_DEVICE "/dev/i2c-1"
#define DID 0x60

//compile with gcc -o tune tune.c

int main( int argc, char *argv[]) {

	//printf ("RPi - tea5767 Philips FM Tuner v0.3 \n") ;

	unsigned char radio[5] = {0};
	int fd;
	unsigned char frequencyH = 0;
	unsigned char frequencyL = 0;
	unsigned int frequencyB;

	double frequency = strtod(argv[1],NULL);

	// calculate pieces of the PLL word
	frequencyB=4*(frequency*1000000+225000)/32768;
	frequencyH=frequencyB>>8;
	frequencyL=frequencyB&0XFF;

	// build what to send to radio
	radio[0]=frequencyH; //FREQUENCY H
	radio[1]=frequencyL; //FREQUENCY L
	radio[2]=0xB0; //3 byte (0xB0): high side LO injection is on,. 
	radio[3]=0x10; //4 byte (0x10) : Xtal is 32.768 kHz 
	radio[4]=0x00; //5 byte0x00)

	char *filename = CHIP_DEVICE;
	if((fd=open(filename, O_RDWR)) < 0) exit(1);
	if(ioctl(fd, I2C_SLAVE, DID) < 0) {
		close(fd);
		exit(1);
	}

	write (fd, radio, 5) ;
	close (fd);
	return 0;
}
