#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define CHIP_DEVICE "/dev/i2c-1"
#define DID 0x60

//compile with gcc -o searchdn searchdn.c -lm

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

  read(fd,radio,5);

  frequency=((((radio[0]&0x3F)<<8)+radio[1])*32768/4-225000)/10000;
  frequency = round(frequency * 10.0)/1000.0;
  frequency = round(frequency * 10.0)/10.0;
  printf ("Leaving Frequency = "); printf("%f\n",frequency);

	if(frequency <= 87.5) {frequency=108;}

  frequencyB=4*(frequency*1000000+225000)/32768; //calculating PLL word
  frequencyH=frequencyB>>8;
  frequencyL=frequencyB&0XFF;

  radio[0]=frequencyH; //FREQUENCY H
  radio[0]=radio[0]^0x40;  //sets search mode
  radio[1]=frequencyL; //FREQUENCY L
  radio[2]=0xB0; //3 byte (0xB0): high side LO injection is on,. 
  radio[2]=radio[2]&0x30; //sets stop level
  radio[3]=0x90; //4 byte (0x10) : Xtal is 32.768 kHz 
  radio[4]=0x00; //5 byte0x00)

  write (fd, radio, 5) ;
  sleep(1);

  read(fd,radio,5);

  frequency=((((radio[0]&0x3F)<<8)+radio[1])*32768/4-225000)/10000;
  frequency = round(frequency * 10.0)/1000.0;
  frequency = round(frequency * 10.0)/10.0;
  printf ("New Frequency = "); printf("%f\n",frequency);

  close(fd);
  return 0;
}

