#include<LPC21XX.h>
#include"delay_ms.h"
#define LCD_D 0xf<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19



void LCD_COMMAND(unsigned char cmd)
{
	IOPIN1=(cmd&0xf0)<<16;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
	
	IOPIN1=(cmd&0x0f)<<20;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}
void LCD_INIT(void)
{
	
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X02);
	LCD_COMMAND(0X0C);
	LCD_COMMAND(0X28);
	LCD_COMMAND(0X80);
}
void LCD_DATA(unsigned char D)
{
	IOPIN1=(D&0xf0)<<16;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
	
	IOPIN1=(D&0x0f)<<20;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}
void LCD_STR(unsigned char *s)
{
	while(*s)
	{
	LCD_DATA(*s++);
	
	}
}
void LCD_INTEGER(int n)
{
	unsigned char a[5];
	signed char i=0;
	if(n==0)
	{
		LCD_DATA('0');
	}
	else
		if(n<0)
		{
			LCD_DATA('-');
			n=-n;
		}
		while(n>0)
		{
			a[i++]=n%10;
			n=n/10;
		}
		for(--i;i>=0;i--)
		{
			LCD_DATA(a[i]+48);
		}
		
}
void LCD_FLOAT(float n)
{
	int temp;
	temp=n;
	LCD_INTEGER(temp);
	LCD_DATA('.');
	temp=(n-temp)*1000;
	LCD_INTEGER(temp);
}
