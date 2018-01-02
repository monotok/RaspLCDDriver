// Copyright 2018 Chris Hamer
/*    This file is part of RaspLCDDriver.

    RaspLCDDriver is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RaspLCDDriver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RaspLCDDriver.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include "i2cControl.hpp"
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


I2cControl::I2cControl(int i2cBusNumber)
{
	char i2cBus[10];
    sprintf(i2cBus, "/dev/i2c-%i", i2cBusNumber);
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)i2cBus;
	if ((this->file_i2c = open(filename, O_RDWR)) < 0)
	{
		error("Failed to open the i2c bus");
		exit(1);
	}
}

void I2cControl::writeByte(unsigned char address, unsigned char byte)
{
	if(address != this->I2C_ADDR)
	{
		this->I2C_ADDR = address;
		if (ioctl(this->file_i2c, I2C_SLAVE, this->I2C_ADDR) < 0)
		{
			error("Failed to acquire bus access and/or talk to slave.\n");
			exit(1);
		}
	}
	else
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
