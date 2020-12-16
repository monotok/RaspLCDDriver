# RaspLCDDriver
Rasberry  Pi C++ LCD 20x4 Driver via I2C

More info on my blog: https://blog.monotok.org/raspberry-pi-20x4-lcd-i2c-c-driver/

Here is a list of functions available:

```
voidlcdSendCommand(unsignedchar command);
voidlcdString(constchar* message);
voidsetCursorPositionRowCol(int row, int col);
voidclearColumnsRowCol(int row, int colToClearTo, int colToClearFrom);
voidblinkCursor();
voidenableUnderlineCursor();
voidclearDisplayClearMem();
voidclearDisplayKeepMem();
voidsetEntryMode();
voidsetDisplayOnCursorOff();
voidset4Bit2Line();
voidset8Bit2Line();
voidmoveCursorRight();
voidmoveCursorLeft();
voidresetCursorPosition();
voidscroll1CharRightAllLines();
voidscroll1CharLeftAllLines();
voidclearLine(int lineNo);
```
The most important functions available allow you to clear certain parts of the display and then move the cursor to any position. This allows you to write logic to update only what has changed etc.

An example:

I wanted to keep the i2c and LCD parts separate as you don't want to call the LCD driver just to open the i2c bus for another i2c device. As a result the LcdDriver constructor takes a pointer to an i2c object.

```
int main(int argc, char *argv[])
{
I2cControl *i2c = new I2cControl(1);
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
//you might need to sleep for a short while before trying to display the custom char
lcd.setCursorPositionRowCol(4,8);
lcd.lcdSendCustomChar(1);

delete i2c;

return0;
}
```
