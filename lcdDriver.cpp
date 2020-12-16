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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "i2cControl.hpp"
#include "lcdDriver.hpp"


// Define some device constants
#define LCD_CHR 1 // Mode - Sending data
#define LCD_CMD 0 // Mode - Sending command

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

#define LCD_BACKLIGHT_ON 0x08  // On
#define LCD_BACKLIGHT_OFF 0x00  //Off

#define ENABLE 0b00000100 // Enable bit
#define READWRITE 0b00000010  // Read/Write bit
#define REGISTERSELECT 0b00000001  // Register select bit
#define LCD_SETCGRAMADDR 0x40 // Used for setting the custom chars

// Predefined Custom Char Addresses - Upper 4 bits come first from datasheet
#define LEFT_ARROW 0x7F
#define RIGHT_ARROW 0x7E
#define DEGREE_SYMBOL 0xDF
#define FULL_CHAR 0xFF

// Timing constants
#define E_PULSE 500 //micro Seconds
#define E_DELAY 500 //micro Seconds


LcdDriver::LcdDriver(unsigned char lcdAdd, I2cControl *i2c)
{
    this->I2C_ADDR = lcdAdd;
    this->i2c = i2c;
    backlight = BACKLIGHT_ON;
    lcdByte(0x33,LCD_CMD); // 110011 Initialise
    lcdByte(0x32,LCD_CMD); // 110010 Initialise
    lcdSendCommand(LCD_UNDERLINE_CURSOR);
    lcdSendCommand(LCD_CLEAR_DISPLAY_CLEAR_MEM);
    lcdSendCommand(LCD_ENTRY_MODE);
    lcdSendCommand(LCD_SET_4BIT_2LINE);
    usleep(E_DELAY);
}

void LcdDriver::blinkCursor() {  lcdSendCommand(LCD_BLINK); }
void LcdDriver::enableUnderlineCursor() {  lcdSendCommand(LCD_UNDERLINE_CURSOR); }
void LcdDriver::clearDisplayClearMem() {  lcdSendCommand(LCD_CLEAR_DISPLAY_CLEAR_MEM); }
void LcdDriver::clearDisplayKeepMem() {  lcdSendCommand(LCD_CLEAR_DISPLAY_KEEP_MEM); }
void LcdDriver::setEntryMode() {  lcdSendCommand(LCD_ENTRY_MODE); }
void LcdDriver::setDisplayOnCursorOff() {  lcdSendCommand(LCD_DISPLAY_ON_CURSOR_OFF); }
void LcdDriver::set4Bit2Line() {  lcdSendCommand(LCD_SET_4BIT_2LINE); }
void LcdDriver::set8Bit2Line() {  lcdSendCommand(LCD_SET_8BIT_2LINE); }
void LcdDriver::moveCursorRight() {  lcdSendCommand(LCD_MOVE_CURSOR_RIGHT); }
void LcdDriver::moveCursorLeft() {  lcdSendCommand(LCD_MOVE_CURSOR_LEFT); }
void LcdDriver::resetCursorPosition() {  lcdSendCommand(LCD_RESET_CURSOR_POSITION); }
void LcdDriver::scroll1CharRightAllLines() {  lcdSendCommand(LCD_SCROLL_1_CHAR_RIGHT_ALL_LINES); }
void LcdDriver::scroll1CharLeftAllLines() {  lcdSendCommand(LCD_SCROLL_1_CHAR_LEFT_ALL_LINES); }


void LcdDriver::lcdToggleEnable(unsigned char bits)
{
    // Toggle enable
    usleep(E_DELAY);
    i2c->writeByte(this->I2C_ADDR, (bits | ENABLE));
    usleep(E_PULSE);
    i2c->writeByte(this->I2C_ADDR,(bits & ~ENABLE));
    usleep(E_DELAY);
}

void LcdDriver::error(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


void LcdDriver::lcdByte(unsigned char bits, unsigned char mode)
{
    unsigned char bits_high, bits_low;
    // Send byte to data pins
    // bits = the data
    // mode = 1 for data
    //        0 for command

    if(backlight == BACKLIGHT_OFF)
    {
        bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT_OFF;
        bits_low = mode | ((bits<<4) & 0xF0) | LCD_BACKLIGHT_OFF;
    }
    else
    {
        bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT_ON;
        bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT_ON;
    }

    // High bits
    i2c->writeByte(this->I2C_ADDR, bits_high);
    lcdToggleEnable(bits_high);

    // Low bits
    i2c->writeByte(this->I2C_ADDR, bits_low);
    lcdToggleEnable(bits_low);
}

void LcdDriver::setCursorPositionHex(unsigned char position)
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

void LcdDriver::setCursorPositionRowCol(int row, int col)
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

unsigned char LcdDriver::convertRowColtoHex(int row, int col)
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


void LcdDriver::lcdSendCommand(unsigned char command)
{
    lcdByte(command, LCD_CMD);
}

void LcdDriver::lcdSendCustomChar(char cgramAddress)
{
    lcdSendChar(cgramAddress);
}

void LcdDriver::lcdSendChar(char singleChar)
{
    lcdByte(singleChar, LCD_CHR);
}

void LcdDriver::lcdString(const char * message)
{
    for(int i = 0; i<strlen(message); i++)
    {
        lcdByte(message[i],LCD_CHR);
    }
}

void LcdDriver::clearColumnsHex(unsigned char positionToClearTo, unsigned char positionToClearFrom)
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

void LcdDriver::clearColumnsRowCol(int row, int colToClearTo, int colToClearFrom)
{
    unsigned char positionToClearTo = convertRowColtoHex(row, colToClearTo);
    unsigned char positionToClearFrom = convertRowColtoHex(row, colToClearFrom);
    clearColumnsHex(positionToClearTo, positionToClearFrom);
}

void LcdDriver::clearLine(int lineNo)
{
    switch(lineNo)
    {
        case 1:
            LcdDriver::clearColumnsHex(LCD_END_LINE_1,LCD_BEG_LINE_1);
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

void LcdDriver::changeBacklight()
{
    if(backlight == BACKLIGHT_ON)
        backlight = BACKLIGHT_OFF;
    else if(backlight == BACKLIGHT_OFF)
        backlight = BACKLIGHT_ON;
}

void LcdDriver::createCustomChar(unsigned char location, unsigned char charMap[])
{
    location &= 0x7; // we only have 8 locations 0-7. location = location & 0x7
    lcdSendCommand(LCD_SETCGRAMADDR | (location << 3));
    for (int i=0; i<8; i++) {
        lcdByte(charMap[i], 0b00000001);
    }
}

void LcdDriver::drawLeftArrow()
{
    lcdSendChar(LEFT_ARROW);
}

void LcdDriver::drawRightArrow()
{
    lcdSendChar(RIGHT_ARROW);
}

void LcdDriver::drawDegreeSymbol()
{
    lcdSendChar(DEGREE_SYMBOL);
}

void LcdDriver::drawFullChar()
{
    lcdSendChar(FULL_CHAR);
}
