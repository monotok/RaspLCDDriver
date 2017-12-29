#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port


extern int file_i2c;

void initI2c(unsigned char I2C_ADDR);
extern void error(char *msg);
void readI2c();
void writeByte(unsigned char address, unsigned char byte);