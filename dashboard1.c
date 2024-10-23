#include"body_control_header.h"
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
	CAN2_MSG m1,m2,m3;
		CAN2_INIT();
	
	IODIR0=1<<17|1<<18|1<<19;
	IOSET0=1<<17|1<<18|1<<19;
		m1.id=0x01;
    m1.rtr=0;
	  m1.dlc=4;
	  m1.byteA=0xDDCCBBAA;
	  m1.byteB=0;
			
		m2.id=0x02;
	  m2.rtr=0;
	  m2.dlc=4;
	  m2.byteA=0xDDCCBBAA;
	  m2.byteB=0;
	
		m3.id=0x03;
	  m3.rtr=0;
 	  m3.dlc=4;
    m3.byteA=0xDDCCBBAA;
  	m3.byteB=0;
			
	while(1)
	{ 
		if((IOPIN0&SW1)==0)
		{
		
		CAN2_TX(m1);//standard frame transmit
		delay_ms(2000);
			IOCLR0|=1<<17;	
			delay_ms(5);
		//	IOSET0|=1<<17;
		//	delay_ms(5);
	  
	}
	
		if((IOPIN0&SW2)==0)
		{			
		CAN2_TX(m2);//standard frame transmit
		delay_ms(2000);
		
		IOCLR0|=1<<19;
			delay_ms(5);
		//	IOSET0=1<<19;
		//	delay_ms(5);
		}
		
	if((IOPIN0&SW3)==0)
		{		
		CAN2_TX(m3);//standard frame transmit
		delay_ms(2000);
		
		IOCLR0|=1<<18;
			delay_ms(5);
		//	IOSET0|=1<<18;
		//	delay_ms(5);
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
