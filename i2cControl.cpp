#include "i2cControl.hpp"
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


I2cControl::I2cControl(unsigned char I2C_ADDR)
{
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((this->file_i2c = open(filename, O_RDWR)) < 0)
	{
		error("Failed to open the i2c bus");
		exit(1);
	}

	int addr = I2C_ADDR;
	if (ioctl(this->file_i2c, I2C_SLAVE, addr) < 0)
	{
		error("Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}
}

void I2cControl::writeByte(unsigned char address, unsigned char byte)
{
	//----- WRITE BYTES -----
	unsigned char buffer[60] = {0};
	buffer[0] = byte;
	int length = 1;			//<<< Number of bytes to write
	if (write(this->file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written
	{
		error("write_i2c Failed to write to the i2c bus.\n");
	}
}

void I2cControl::readI2c()
{
	//----- READ BYTES -----
	unsigned char buffer[60] = {0};
	int length = 4;			//<<< Number of bytes to read
	if (read(this->file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read
	{
		error("readI2c Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}
}

void I2cControl::error(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}
