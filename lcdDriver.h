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

// Define some device parameters
#define I2C_ADDR 0x3f // I2C device address
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

void lcdToggleEnable(unsigned char bits);
void lcdByte(unsigned char bits, unsigned char mode);
void lcdInit();
void lcdSendCommand(unsigned char command);
void lcdString(char * message);
unsigned char convertRowColtoHex(int row, int col);
void setCursorPositionRowCol(int row, int col);
void setCursorPositionHex(unsigned char position);
void clearColumnsHex(unsigned char positionToClearTo, unsigned char positionToClearFrom);
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