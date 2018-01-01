#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

class I2cControl
{
    private:
        int file_i2c;
        unsigned char I2C_ADDR;
        void error(const char *msg);

    public:
        I2cControl(int i2cNumber);
        void readI2c();
        void writeByte(unsigned char address, unsigned char byte);
};