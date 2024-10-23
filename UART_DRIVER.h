#include<lpc21xx.h>

void uart0_init(int baud)
{
	PINSEL0|=0x00000005;
	U0LCR=0x83;
	switch(baud)
	{	
		case 115200:U0DLL=32;	break;//PCLK=60MHz
		case 921600:U0DLL=4;	break;
		default:	U0DLL=32	;	break;
	}
	U0LCR=0x03;
}
void uart0_tx(unsigned char data)
{
	U0THR=data;
	while(U0LSR&(1<<5)==0);
}
unsigned char uart0_rx(void)
{
	while(U0LSR&(1<<0)==0);
	return U0RBR;
}
void uart0_string( char *p)
{
	while(*p!=0)
	{
		U0THR=*p;
		while((U0LSR&(1<<5))==0);
		p++;
	}
}
#include<stdio.h>
void uart0_int(int num )
{
	char buf[10];
	sprintf(buf,"%d",num);
	uart0_string(buf);
}

void uart0_hex(int num)
{
	char buf[10];
	sprintf(buf,"%x",num);
	uart0_string(buf);
}
