/***************************************
* Chris Hamer's C Driver for LCD i2c device
* 30/12/17
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "lcdDriver.h"
#include <errno.h>

// Define some device parameters
#define I2C_ADDR 0x3f // I2C device address
#define LCD_WIDTH 20   // Maximum characters per line
#define LCD_ROWS 4 //The number of rows on the LCD

// Define some device constants
#define LCD_CHR 1 // Mode - Sending data
#define LCD_CMD 0 // Mode - Sending command

#define LCD_BEG_LINE_1 0x80 // LCD RAM address for the 1st line
#define LCD_END_LINE_1 0x93 // LCD RAM address for the 1st line

#define LCD_BEG_LINE_2 0xC0 // LCD RAM address for the 2nd line
#define LCD_END_LINE_2 0xD3 // LCD RAM address for the 2nd line

#define LCD_BEG_LINE_3 0x94 // LCD RAM address for the 3rd line
#define LCD_END_LINE_3 0xA7 // LCD RAM address for the 3rd line

#define LCD_BEG_LINE_4 0xD4 // LCD RAM address for the 4th line
#define LCD_END_LINE_4 0xE7 // LCD RAM address for the 4th line


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



void initI2c()
{
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		error("Failed to open the i2c bus");
		exit(1);
	}

	int addr = I2C_ADDR;
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		error("Failed to acquire bus access and/or talk to slave.\n");
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
		error("readI2c Failed to read from the i2c bus.\n");
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
		error("write_i2c Failed to write to the i2c bus.\n");
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

void lcdInit()
{
  lcdByte(0x33,LCD_CMD); // 110011 Initialise
  lcdByte(0x32,LCD_CMD); // 110010 Initialise
  lcdSendCommand(LCD_UNDERLINE_CURSOR);
  lcdSendCommand(LCD_CLEAR_DISPLAY_CLEAR_MEM);
  lcdSendCommand(LCD_ENTRY_MODE);
  lcdSendCommand(LCD_SET_4BIT_2LINE);
  usleep(E_DELAY);
}

void setCursorPositionHex(unsigned char position)
{
  if(position < LCD_BEG_LINE_1 || position > LCD_END_LINE_4)
  {
    char buffer[60];
    sprintf(buffer, "Invalid hex position 0x%02x\nValid position is 0x%02x - 0x%02x", position, LCD_BEG_LINE_1, LCD_END_LINE_4);
    error(buffer);
  }
  else
  {
    lcdSendCommand(position);    
  }
}

void setCursorPositionRowCol(int row, int col) 
{
  if(row < 1 || row > LCD_ROWS)
  {
    char buffer[60];
    sprintf(buffer, "Invalid position, no such row %i\nValid rows are 1 - %i", row, LCD_ROWS);
    error(buffer);
  }
  else if(col < 0 || col > LCD_WIDTH-1)
  {
    char buffer[60];
    sprintf(buffer, "Invalid position, no such col %i\nValid columns are 0 - %i", col, LCD_WIDTH-1);
    error(buffer);
  }
  else
  {
    lcdSendCommand(convertRowColtoHex(row, col));
  }
}

unsigned char convertRowColtoHex(int row, int col) 
{
  unsigned char currentLineHex;
  char buffer[70];  
  switch(row)
  {
    case 1:
      currentLineHex = LCD_BEG_LINE_1;
      break;
    case 2:
      currentLineHex = LCD_BEG_LINE_2;
      break;
    case 3:
      currentLineHex = LCD_BEG_LINE_3;
      break;
    case 4:
      currentLineHex = LCD_BEG_LINE_4;
      break;
    default:
      sprintf(buffer, "No such row number %i\nValid rows are 1 - %i", row, LCD_ROWS);
      error(buffer);
  }
  if(col < LCD_WIDTH & col >= 0)
  {
    return (currentLineHex+(col));
  }
  else
  {
    sprintf(buffer, "No such col number %i\n.Valid cols are 0 -  %i", col, LCD_WIDTH-1);
    error(buffer);
  }
}


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

void clearColumnsHex(unsigned char positionToClearTo, unsigned char positionToClearFrom)
{
  int numberOfColsToClear = positionToClearTo - positionToClearFrom;
  if(numberOfColsToClear < LCD_WIDTH & numberOfColsToClear > 0)
  {
    setCursorPositionHex(positionToClearFrom);

    for(int i = 0; i <= numberOfColsToClear; i++)
    {
      lcdString(" ");
    }
  }
  else
  {
    char buffer[60];
    sprintf(buffer, "Can only clear one line at a time. Exceeded LCD Width %i\n", LCD_WIDTH);
    error(buffer);
  }
}

void clearColumnsRowCol(int row, int colToClearTo, int colToClearFrom)
{
  unsigned char positionToClearTo = convertRowColtoHex(row, colToClearTo);
  unsigned char positionToClearFrom = convertRowColtoHex(row, colToClearFrom);
  clearColumnsHex(positionToClearTo, positionToClearFrom);
}

void clearLine(int lineNo)
{
  switch(lineNo)
  {
    case 1:
      clearColumnsHex(LCD_END_LINE_1,LCD_BEG_LINE_1);  
      break;
    case 2:
      clearColumnsHex(LCD_END_LINE_2,LCD_BEG_LINE_2);  
      break;
    case 3:
      clearColumnsHex(LCD_END_LINE_3,LCD_BEG_LINE_3);  
      break;
    case 4:
      clearColumnsHex(LCD_END_LINE_3,LCD_BEG_LINE_3);  
      break;
    default:
      error("No such line to clear!");
  }
}

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


int main(int argc, char *argv[])
{
    //initialise i2c
    initI2c();
    //Initialise display
    lcdInit();

    lcdSendCommand(LCD_BEG_LINE_1);
    lcdString("Sensor ID:");

    setCursorPositionHex(0x8b);
    lcdString("Here");

    setCursorPositionHex(LCD_BEG_LINE_2);
    lcdString("Temp: ");

    setCursorPositionHex(0xc6);
    lcdString("20.2c");

    setCursorPositionHex(LCD_BEG_LINE_3);
    lcdString("Humidity: ");

    setCursorPositionHex(0x9e);
    lcdString("75%");

    setCursorPositionHex(LCD_BEG_LINE_4);
    lcdString("Battery: ");

    setCursorPositionHex(0xde);
    // lcdString("         ");
    // setCursorPositionHex(0xde);    
    lcdString("1000%");
    clearColumnsHex(LCD_END_LINE_4,0xde);

    setCursorPositionHex(0xde);
    // lcdString("         ");
    // setCursorPositionHex(0xde);    
    lcdString("32%");

    // clearColumnsRowCol(2,11,2,8);
    // clearColumnsHex(LCD_END_LINE_2,LCD_BEG_LINE_2+7);
    clearColumnsRowCol(2,10,7);
    setCursorPositionRowCol(2,7);        
    lcdString("1c");

    return 0;
}
