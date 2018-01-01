
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
};