#include "i2cControl.hpp"
#include "lcdDriver.hpp"

#define lcdAdd 0x3f // I2C device address

//Compile with gcc lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain -lstdc++
//Or g++ lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain

int main(int argc, char *argv[])
{
    I2cControl *i2c = new I2cControl(1);
    LcdDriver lcd(lcdAdd, i2c);

    lcd.lcdSendCommand(LCD_BEG_LINE_1);
    lcd.lcdString("Sensor ID:");

    lcd.setCursorPositionRowCol(1,10);
    lcd.lcdString("Here");

    lcd.setCursorPositionRowCol(2,0);
    lcd.lcdString("Temp: ");

    lcd.setCursorPositionRowCol(2,8);
    lcd.lcdString("22.4 C");

    return 0;
}