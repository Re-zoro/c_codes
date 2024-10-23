//LCD SYMBOL DISPLAY (LEFT)
#include<lpc21xx.h>
#include"4_BIT.h"

unsigned char CGRAM_LUT[]={0x01,0x03,0x07,0x0f,0x07,0x03,0x01,0x00};
void CGRAM_WRITE(unsigned char n)
{
unsigned char index=0;
LCD_COMMAND(0x40);
for(index=0;index<n;index++)
LCD_DATA(CGRAM_LUT[index]);
}

int main()
{
	PINSEL1|=0x00;
//int cnt=10;
LCD_INIT();
	



	LCD_DATA('A');
  delay_ms(2);
	CGRAM_WRITE(8);
	LCD_COMMAND(0x81);
  LCD_DATA(0);
	delay_ms(2);
}
