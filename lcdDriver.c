/***************************************
* Maurices quick port of LCD_I2C.PY to C
* 03/05/16
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "lcdDriver.h"

// Define some device parameters
#define I2C_ADDR 0x3f // I2C device address
#define LCD_WIDTH 20   // Maximum characters per line

// Define some device constants
#define LCD_CHR 1 // Mode - Sending data
#define LCD_CMD 0 // Mode - Sending command

#define LCD_BEG_LINE_1 0x80 // LCD RAM address for the 1st line
#define LCD_BEG_LINE_2 0xC0 // LCD RAM address for the 2nd line
#define LCD_BEG_LINE_3 0x94 // LCD RAM address for the 3rd line
#define LCD_BEG_LINE_4 0xD4 // LCD RAM address for the 4th line

//LCD Available Commands
#define LCD_BLINK 0x0F
#define LCD_UNDERLINE_CURSOR 0x0E
#define LCD_CLEAR_DISPLAY_CLEAR_MEM 0x01
#define LCD_CLEAR_DISPLAY_KEEP_MEM 0x08
#define LCD_ENTRY_MODE 0x06 //04, 05, 06 & 07
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0C
#define LCD_SET_4BIT_2LINE 0x28
#define LCD_SET_8BIT_2LINE 0x38
#define LCD_MOVE_CURSOR_RIGHT 0x14
#define LCD_MOVE_CURSOR_LEFT 0x10
#define LCD_RESET_CURSOR_POSITION 0x2
#define LCD_SCROLL_1_CHAR_RIGHT_ALL_LINES 0x1E
#define LCD_SCROLL_1_CHAR_LEFT_ALL_LINES 0x18


#define LCD_BACKLIGHT 0x08  // On
//#LCD_BACKLIGHT = 0x00  # Off

#define ENABLE 0b00000100 // Enable bit
#define READWRITE 0b00000010  // Read/Write bit
#define REGISTERSELECT 0b00000001  // Register select bit

// Timing constants
#define E_PULSE 500 //micro Seconds
#define E_DELAY 500 //micro Seconds


int file_i2c;

void blinkCursor() {  lcdSendCommand(LCD_BLINK); }
void enableUnderlineCursor() {  lcdSendCommand(LCD_UNDERLINE_CURSOR); }
void clearDisplayClearMem() {  lcdSendCommand(LCD_CLEAR_DISPLAY_CLEAR_MEM); }
void clearDisplayKeepMem() {  lcdSendCommand(LCD_CLEAR_DISPLAY_KEEP_MEM); }
void setEntryMode() {  lcdSendCommand(LCD_ENTRY_MODE); }
void setDisplayOnCursorOff() {  lcdSendCommand(LCD_DISPLAY_ON_CURSOR_OFF); }
void set4Bit2Line() {  lcdSendCommand(LCD_SET_4BIT_2LINE); }
void set8Bit2Line() {  lcdSendCommand(LCD_SET_8BIT_2LINE); }
void moveCursorRight() {  lcdSendCommand(LCD_MOVE_CURSOR_RIGHT); }
void moveCursorLeft() {  lcdSendCommand(LCD_MOVE_CURSOR_LEFT); }
void resetCursorPosition() {  lcdSendCommand(LCD_RESET_CURSOR_POSITION); }
void scroll1CharRightAllLines() {  lcdSendCommand(LCD_SCROLL_1_CHAR_RIGHT_ALL_LINES); }
void scroll1CharLeftAllLines() {  lcdSendCommand(LCD_SCROLL_1_CHAR_LEFT_ALL_LINES); }
void setCursorPosition(unsigned char position) {  lcdSendCommand(position); }




void initI2c()
{
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus");
		exit(1);
	}

	int addr = I2C_ADDR;
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}
}

void readI2c()
{
	//----- READ BYTES -----
  unsigned char buffer[60] = {0};
	int length = 4;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read
	{
		printf("readI2c Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}
}

void writeByte(unsigned char address, unsigned char byte)
{
	//----- WRITE BYTES -----
  unsigned char buffer[60] = {0};
	buffer[0] = byte;
	int length = 1;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written
	{
		printf("write_i2c Failed to write to the i2c bus.\n");
	}
}

void lcdToggleEnable(unsigned char bits)
{
  // Toggle enable
  usleep(E_DELAY);
  writeByte(I2C_ADDR, (bits | ENABLE));
  usleep(E_PULSE);
  writeByte(I2C_ADDR,(bits & ~ENABLE));
  usleep(E_DELAY);
}


void lcdByte(unsigned char bits, unsigned char mode)
{
    unsigned char bits_high, bits_low;
  // Send byte to data pins
  // bits = the data
  // mode = 1 for data
  //        0 for command

  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
  bits_low = mode | ((bits<<4) & 0xF0) | LCD_BACKLIGHT;

  // High bits
  writeByte(I2C_ADDR, bits_high);
  lcdToggleEnable(bits_high);

  // Low bits
  writeByte(I2C_ADDR, bits_low);
  lcdToggleEnable(bits_low);
}

// void lcdInit()
// {
//   lcdByte(0x33,LCD_CMD); // 110011 Initialise
//   lcdByte(0x32,LCD_CMD); // 110010 Initialise
//   lcdByte(0x28,LCD_CMD); // 101000 Data length, number of lines, font size
//   usleep(E_DELAY);
// }

void lcdSendCommand(unsigned char command)
{
  lcdByte(command, LCD_CMD);
}

void lcdString(char * message)
{
  for(int i = 0; i<strlen(message); i++)
  {
    lcdByte(message[i],LCD_CHR);
  }
}

int main(int argc, char *argv[])
{
    //initialise i2c
    initI2c();
    //Initialise display
    // lcdInit();

    lcdSendCommand(LCD_UNDERLINE_CURSOR);
    lcdSendCommand(LCD_CLEAR_DISPLAY_CLEAR_MEM);
    lcdSendCommand(LCD_ENTRY_MODE);
    lcdSendCommand(LCD_SET_4BIT_2LINE);
    usleep(E_DELAY);

    lcdSendCommand(LCD_BEG_LINE_1);
    lcdString("Sensor ID:");

    setCursorPosition(0x8b);
    lcdString("Here");

    setCursorPosition(LCD_BEG_LINE_2);
    lcdString("Temp: ");

    setCursorPosition(0xc6);
    lcdString("20.2c");

    setCursorPosition(LCD_BEG_LINE_3);
    lcdString("Humidity: ");

    setCursorPosition(0x9e);
    lcdString("70%");

    setCursorPosition(LCD_BEG_LINE_4);
    lcdString("Battery: ");

    setCursorPosition(0xde);
    // lcdString("         ");
    // setCursorPosition(0xde);    
    lcdString("52%");

    return 0;
}
