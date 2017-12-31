#include "i2cControl.hpp"
#include "lcdDriver.hpp"

#define lcdAdd 0x3f // I2C device address

//Compile with gcc lcdDriver.c i2cControl.c test.c -o lcdDriverTestMain

int main(int argc, char *argv[])
{
    LcdDriver lcd(lcdAdd);

    lcd.lcdSendCommand(LCD_BEG_LINE_1);
    lcd.lcdString("Sensor ID:");

    lcd.setCursorPositionRowCol(1,10);
    lcd.lcdString("Here");

    lcd.setCursorPositionRowCol(2,0);
    lcd.lcdString("Temp: ");

    return 0;
}