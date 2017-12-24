void initI2c();
void readI2c();
void writeByte(unsigned char address, unsigned char byte);
void lcdToggleEnable(unsigned char bits);
void lcdByte(unsigned char bits, unsigned char mode);
void lcdInit();
void lcdSendCommand(unsigned char command);
void lcdString(char * message, unsigned char line);
int main(int argc, char *argv[]);