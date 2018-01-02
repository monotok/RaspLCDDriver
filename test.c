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

#include "i2cControl.h"
#include "lcdDriver.h"

//Compile with gcc lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain

int main(int argc, char *argv[])
{
    //initialise i2c
    initI2c(I2C_ADDR);
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