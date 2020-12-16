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
#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

// Define some device parameters
#define LCD_WIDTH 20   // Maximum characters per line
#define LCD_ROWS 4 //The number of rows on the LCD

#define LCD_BEG_LINE_1 0x80 // LCD RAM address for the 1st line
#define LCD_END_LINE_1 0x93 // LCD RAM address for the 1st line

#define LCD_BEG_LINE_2 0xC0 // LCD RAM address for the 2nd line
#define LCD_END_LINE_2 0xD3 // LCD RAM address for the 2nd line

#define LCD_BEG_LINE_3 0x94 // LCD RAM address for the 3rd line
#define LCD_END_LINE_3 0xA7 // LCD RAM address for the 3rd line

#define LCD_BEG_LINE_4 0xD4 // LCD RAM address for the 4th line
#define LCD_END_LINE_4 0xE7 // LCD RAM address for the 4th line

#define BACKLIGHT_ON 1
#define BACKLIGHT_OFF 0

class LcdDriver
{
private:
    unsigned char I2C_ADDR;
    I2cControl *i2c;
    void lcdToggleEnable(unsigned char bits);
    void lcdByte(unsigned char bits, unsigned char mode);
    unsigned char convertRowColtoHex(int row, int col);
    void setCursorPositionHex(unsigned char position);
    void clearColumnsHex(unsigned char positionToClearTo, unsigned char positionToClearFrom);
    void error(const char *msg);
    short backlight;

public:
    LcdDriver(unsigned char I2C_ADDR, I2cControl *i2c);
    void lcdSendCommand(unsigned char command);
    void lcdString(const char * message);
    void setCursorPositionRowCol(int row, int col);
    void clearColumnsRowCol(int row, int colToClearTo, int colToClearFrom);
    void blinkCursor();
    void enableUnderlineCursor();
    void clearDisplayClearMem();
    void clearDisplayKeepMem();
    void setEntryMode();
    void setDisplayOnCursorOff();
    void set4Bit2Line();
    void set8Bit2Line();
    void moveCursorRight();
    void moveCursorLeft();
    void resetCursorPosition();
    void scroll1CharRightAllLines();
    void scroll1CharLeftAllLines();
    void clearLine(int lineNo);
    void changeBacklight();

    //Custom Chars
    void lcdSendChar(char singleChar);
    void lcdSendCustomChar(char cgramAddress);
    void createCustomChar(unsigned char location, unsigned char charMap[]);

    //Predefined Chars from datasheet
    void drawLeftArrow();
    void drawRightArrow();
    void drawDegreeSymbol();
    void drawFullChar();

};
#endif //LCD_DRIVER_H
