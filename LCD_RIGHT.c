//LCD SYMBOL DISPLAY (RIGHT)
#include<lpc21xx.h>
#include"4_BIT.h"

unsigned char CGRAM_LUT[]={0x10,0x18,0x1c,0x1e,0x1c,0x18,0x10,0x00};
void CGRAM_WRITE(unsigned char n)
{
unsigned char index=0;
LCD_COMMAND(0x40);
for(index=0;index<n;index++)
LCD_DATA(CGRAM_LUT[index]);
}
void delay_ms(int);
int main()
{
int cnt=10;
LCD_INIT();
CGRAM_WRITE(8);
while(cnt--)
{
LCD_COMMAND(0x80);
delay_ms(2000);
LCD_DATA(0);
}
}
void delay_ms(int ms)
{
T0PR=15000-1;
T0TCR=0x01;
while(T0TC<ms);
T0TCR=0x03;
T0TCR=0x00;
}