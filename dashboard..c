#include"body_control_header.h"
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
	C2TFI1=(m1.dlc<<16);
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
//	int cnt=0;
	CAN2_MSG m1,m2,m3;
		CAN2_INIT();
	
	
	m1.id=0x01;
	m1.rtr=0;
	m1.dlc=4;
	m1.byteA=0x0000000A;
	m1.byteB=0;
	
	m2.id=0x02;
	m2.rtr=0;
	m2.dlc=4;
	m2.byteA=0x0000000B;
	m2.byteB=0;
	
	m3.id=0x03;
	m3.rtr=0;
	m3.dlc=4;
	m3.byteA=0x0000000C;
	m3.byteB=0;
	
IODIR0|=LEFT|RIGHT|MOTOR;
IOSET0|=LEFT|RIGHT|MOTOR;
	while(1)
	{
		
		
		if((IOPIN0&SW2)==0)
		{
			 IOCLR0=LEFT;
			 CAN2_TX(m1);//standard frame transmit
		   delay_ms(2000);
			
			
			
			 //delay_ms(2);
			 //IOSET0=LEFT;
		  
		 }
	if((IOPIN0&SW1)==0)
	{
		 IOCLR0=RIGHT;
		   CAN2_TX(m2);//standard frame transmit
		   delay_ms(2000);
		
			
			// delay_ms(2);
			 //IOSET0=RIGHT;
		
	  
	
	}
	
	if((IOPIN0&SW3)==0)
	{
		
		
			 IOCLR0=MOTOR;
			 
		   CAN2_TX(m3);//standard frame transmit
		   delay_ms(2000);
		
			// delay_ms(2);
			// IOSET0=MOTOR;
		
  }
}
}
void CAN2_INIT(void)
	{
//	 unsigned int *p;
	PINSEL1|=0x00014000;
	VPBDIV=1;
	
	C2MOD=0x1;
	
	
	C2BTR=0x001c001d;
	
 
	//p[1]=0x20022001;
	//p[2]=0x00002003;
	//SFF_sa=0x0;
	//SFF_GRP_sa=0x8;
	//EFF_sa=0x8;
	//EFF_GRP_sa=0x8;
	
	//ENDofTable=0x8;
	
	AFMR=0x2;
	C2MOD=0x0;
}

void delay_ms(int ms)
{
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<ms);
	T0TCR=0x03;
	T0TCR=0x00;
}
