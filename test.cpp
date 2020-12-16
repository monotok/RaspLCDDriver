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
#include "lcdDriver.hpp"

//Only included to allow a sleep to show clearing display etc
#include <chrono>
#include <thread>

#define lcdAdd 0x3f // I2C device address

//Compile with gcc lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain -lstdc++
//Or g++ lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain

int main(int argc, char *argv[])
{
    I2cControl *i2c = new I2cControl(3); //Using software i2c
    LcdDriver lcd(lcdAdd, i2c);

    lcd.lcdSendCommand(LCD_BEG_LINE_1);
    lcd.lcdString("SensorID:");
    lcd.setCursorPositionRowCol(1,11);
    lcd.lcdString("Here");

    lcd.setCursorPositionRowCol(2,0);
    lcd.lcdString("Temp: ");
    lcd.setCursorPositionRowCol(2,8);
    lcd.lcdString("22.4 C");

    lcd.setCursorPositionRowCol(3,0);
    lcd.lcdString("Battery: ");
    lcd.setCursorPositionRowCol(3,11);
    lcd.lcdString("45%");

    //Sleep for 4 seconds so we can observe the display
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    //Change battery to the word humidity
    lcd.clearColumnsRowCol(3,8,0);
    lcd.setCursorPositionRowCol(3,0);
    lcd.lcdString("Humidity:");

    //Create and draw custom half battery symbol
    uint8_t customChar[8] = {
            0x0E,
            0x11,
            0x11,
            0x11,
            0x1F,
            0x1F,
            0x1F,
            0x1F
    };
    lcd.createCustomChar(1, customChar);
    lcd.setCursorPositionRowCol(4,8);
    lcd.lcdSendCustomChar(1);

    delete i2c;

    return 0;
}