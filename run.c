#include "global.h"
#define NOKEY 0
#define KEY7 7
#define KEY6 6
#define KEY5 5
#define KEY4 4
#define SEGMENT_TIME 500
void lcd_command_write(BYTE command);

void ShortDelay(BYTE i) {
    while (i--); // 7us delay about 7 Cycle
}
void LongDelay(WORD i) {
	while (i--) {
		ShortDelay(145); // 1ms delay }
	}
}

BYTE SegDigit = 0;

BYTE code FND[10] = { ~(0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20), // 0
~(0x02 | 0x04), // 1
~(0x01 | 0x02 | 0x40 | 0x10 | 0x08), // 2
~(0x01 | 0x02 | 0x40 | 0x04 | 0x08), // 3
~(0x20 | 0x40 | 0x02 | 0x04), // 4
~(0x01 | 0x20 | 0x40 | 0x04 | 0x08), // 5
~(0x20 | 0x10 | 0x40 | 0x04 | 0x08), // 6
~(0x20 | 0x01 | 0x02 | 0x04), // 7
~(0x01 | 0x02 | 0x04 | 0x40 | 0x08 | 0x10 | 0x20), // 8
~(0x20 | 0x01 | 0x02 | 0x40 | 0x04), // 9
};

void Initial_CPU(void) {
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;

	LongDelay(40);
	lcd_command_write(0x38);
	lcd_command_write(0x08);
	lcd_command_write(0x01);
	lcd_command_write(0x06);
	lcd_command_write(0x0C);
}
void lcd_command_write(BYTE command){ // LCD command write 
	LCD_WR_COMMAND = command; // LCD command write
	LongDelay(1); // time delay for ms - 1 ms
}
void lcd_char_display(BYTE character){ // LCD character display 
	LCD_WR_DATA = character; // data write to CG RAM or DD RAM
	LongDelay(1); // time delay for ms - 1 ms
}
void LcdString(BYTE x, BYTE y, char* str) {
	BYTE index;
	BYTE character;
	lcd_command_write(0x80 + y + 0x40 * x); // LCD command : cursor position
	index = 0;
	character = str[index];
	while (character != '\0') {
		lcd_char_display(character); // LCD character display
		index++; character = str[index];
	}
}
void SegmentDisplay(WORD num)
{
    WORD TempNumber;
    BYTE i;
    BYTE Digit = 0x08;
    TempNumber = num;
    for (i = 0; i < SegDigit; i++) {
        TempNumber = TempNumber / 10;
        Digit = Digit >> 1;
    }
    SEG_SIGAL = FND[TempNumber % 10];
    SEG_DIGIT = ~Digit;
    if (SegDigit++ >= 3)
        SegDigit = 0;
}

int count1 = 0;

char level1[10][10] = {"  happy", "  bread", "  frame", 
						"  march", "  shelf", "  wrong", "  usual", "  tight", "  storm", "  smart" };
	
WORD FndData = 0;
WORD FndNextCount = SEGMENT_TIME;

		
BYTE GetPortKey(void) {
	BYTE value, KeyNo;
	value = P1;
	if (!(value & 0x80)) {
		LcdString(0, 1, level1[count1]);
		LcdString(1, 3, level1[count1 +1]);
		SegmentDisplay(FndData);
		LongDelay(500);
		count1++;
		FndData++;
	}
	else if (!(value & 0x40))
		LcdString(0, 1, "LATER..");
	else if (!(value & 0x20))
		LcdString(0, 1, "LATER..");
	else if (!(value & 0x10))
		LcdString(0, 1, "LATER..");
}

void main(void) {
	
	
	Initial_CPU();	
	LcdString(0, 1, "Press");
	LcdString(1, 3, "1,2,3,4");
	
	
	
	while (1) {
		GetPortKey();
		if(count1 == 10) break;
	}
}
