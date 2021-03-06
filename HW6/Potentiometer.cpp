#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "../common/Peripherals.h"

using namespace std;

/* LEDs 
	1st - 5
	2nd - 6
	3rd - 13
	4th - 19
	5th - 26
*/

class mcp3008Spi
{
	public:
		mcp3008Spi();
		mcp3008Spi(string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spiBitsPerWord);
		~mcp3008Spi();
		int spiWriteRead(unsigned char *data, int length);
	private:
		unsigned char mode;
		unsigned char bitsPerWord;
		unsigned int speed;
		int spifd;

		int spiOpen(std::string devspi);
    		int spiClose();
};

int mcp3008Spi::spiOpen(std::string devspi)
{
	int statusVal = -1;
    	this->spifd = open(devspi.c_str(), O_RDWR);
    	if(this->spifd < 0)
	{
        	perror("could not open SPI device");
        	exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    	if(statusVal < 0)
	{
        	perror("Could not set SPIMode (WR)...ioctl fail");
        	exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    	if(statusVal < 0)
	{
      		perror("Could not set SPIMode (RD)...ioctl fail");
      		exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    	if(statusVal < 0)
	{
      		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      		exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    	if(statusVal < 0)
	{
      		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      		exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    	if(statusVal < 0)
	{
      		perror("Could not set SPI speed (WR)...ioctl fail");
      		exit(1);
    	}

    	statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    	if(statusVal < 0)
	{
      		perror("Could not set SPI speed (RD)...ioctl fail");
      		exit(1);
    	}

	return statusVal;
}

/***********************************************************
 * spiClose(): Responsible for closing the spidev interface.
 * Called in destructor
 * *********************************************************/

int mcp3008Spi::spiClose()
{
	int statusVal = -1;
    	statusVal = close(this->spifd);
        if(statusVal < 0)
	{
      		perror("Could not close SPI device");
      		exit(1);
    	}
	return statusVal;
}

/********************************************************************
 * This function writes data "data" of length "length" to the spidev
 * device. Data shifted in from the spidev device is saved back into
 * "data".
 * ******************************************************************/
int mcp3008Spi::spiWriteRead( unsigned char *data, int length)
{
	struct spi_ioc_transfer spi[length];
  	int i = 0;
  	int retVal = -1;

	// one spi transfer for each byte
	for (i = 0 ; i < length ; i++)
	{
		memset(&spi[i], 0, sizeof (spi[i]));
	    	spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
	    	spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
	    	spi[i].len           = sizeof(*(data + i)) ;
	    	spi[i].delay_usecs   = 0 ;
	    	spi[i].speed_hz      = this->speed ;
	    	spi[i].bits_per_word = this->bitsPerWord ;
    		spi[i].cs_change = 0;
	}

	retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
    		perror("Problem transmitting spi data..ioctl");
    		exit(1);
 	}

	return retVal;
}

/*************************************************
 * Default constructor. Set member variables to
 * default values and then call spiOpen()
 * ***********************************************/

mcp3008Spi::mcp3008Spi()
{
	this->mode = SPI_MODE_0 ;
    	this->bitsPerWord = 8;
    	this->speed = 1000000;
    	this->spifd = -1;

	this->spiOpen(std::string("/dev/spidev0.0"));
}

/*************************************************
 * overloaded constructor. let user set member variables to
 * and then call spiOpen()
 * ***********************************************/
mcp3008Spi::mcp3008Spi(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord)
{
    this->mode = spiMode ;
    this->bitsPerWord = spibitsPerWord;
    this->speed = spiSpeed;
    this->spifd = -1;

    this->spiOpen(devspi);
}

/**********************************************
 * Destructor: calls spiClose()
 * ********************************************/
mcp3008Spi::~mcp3008Spi()
{
    this->spiClose();
}

int main(void)
{
	mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    	int i = 20;
        int a2dVal = 0;
    	int a2dChannel = 0;
        unsigned char data[3];

    	while(i > 0)
    	{
        	data[0] = 1;  //  first byte transmitted -> start bit
        	data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
        	data[2] = 0; // third byte transmitted....don't care

        	a2d.spiWriteRead(data, sizeof(data) );

        	a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);
        	sleep(1);
        	cout << "The Result is: " << a2dVal << endl;
        	i--;
    	}

	return 0;
}
