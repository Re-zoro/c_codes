#include"UART_DRIVER.h"
typedef struct can_2 //standard frame of CAN
{
	unsigned int id;//message id
	unsigned int rtr;//rtr bit
	unsigned int dlc;//dlc bits 
	unsigned int byteA;//lower byte
	unsigned int byteB;//higher byte
}CAN2_MSG;

void CAN2_INIT(void);
void CAN2_TX(CAN2_MSG m1)
{
	C2TID1=m1.id;
	C2TFI1=m1.dlc<<16;
	if(m1.rtr==0)//if rtr=0 then standard frame
	{
		C2TFI1&=~(1<<30);
		C2TDA1=m1.byteA;
		C2TDB1=m1.byteB;
	}
	else//else if rtr=1 remote frame
	{
		C2TFI1|=(1<<30);
	}
	C2CMR=(1<<0)|(1<<5);//transmission request and select transmit  buffer 
	while((C2GSR&(1<<3))==0);//waiting till message is transmitted
}
void CAN2_RX(CAN2_MSG* m1)
{
	while((C2GSR&(0x1))==0);
	m1->id=C2RID;
	m1->rtr=(C2RFS>>30)&0x1;
	m1->dlc=(C2RFS>>16)&0xf;
	if(m1->rtr==0)
	{
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	
	C2CMR=(1<<2);
}
void delay_ms(int);
int main()
{
		int cnt;
	CAN2_MSG m1;
	IODIR0|=1<<17;
	IOSET0|=1<<17;

	CAN2_INIT();
	uart0_init(115200);

	//m1.id=0x01;
	//m1.rtr=0;
	//m1.dlc=4;
	//m1.byteA=0xDDCCBBAA;
	//m1.byteB=0;
	
	
	while(1)
	{ 
	
		CAN2_RX(&m1);
		delay_ms(2000);
	
		if(m1.id==0x01)
		{
		uart0_string("LEFT INDICATOR TESTING");	
		if(m1.rtr==0){
			uart0_string("LEFT INDICATOR:DATA-FRAME \r\n");
			uart0_hex(m1.id);
			uart0_tx(' ');
			uart0_hex(m1.dlc);
			uart0_tx(' ');
			uart0_hex(m1.byteA);
			uart0_tx(' ');
			uart0_hex(m1.byteB);
			uart0_string("\r\n");
			
	  for(cnt=0;cnt<10;cnt++)
	  {
		IOCLR0=1<<17;
		delay_ms(1000);
		IOSET0=1<<17;
		delay_ms(1000);
	  }
		}	
		else 
		{
			uart0_string("LEFT INDICATOR:REMOTE-FRAME \r\n");
			uart0_hex(m1.id);
			uart0_tx(' ');
			uart0_hex(m1.dlc);
			uart0_tx(' ');
			uart0_string("\r\n");
		}	
	  }
	}
	}
	

void CAN2_INIT(void)
{
	PINSEL1|=0x00014000;//select pins for CAN 2
	VPBDIV=1;//pclk=60Mhz
	
	C2MOD=0x01;// CAN2 goes to reset mode 
	
	
	AFMR=0x2;//accept all messages 
	C2BTR=0x001c001d; // b@125 kbps
	
	
	C2MOD=0x00;//CAN2 back to normal mode
}
void delay_ms(int ms)
{
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<ms);
	T0TCR=0x03;
	T0TCR=0x00;
}
