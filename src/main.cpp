#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DEPTH 30
#define ROWS 24

unsigned int remain;
unsigned char rxstate;
unsigned char rxbuffer;
register unsigned char * wrptr asm("r26");
//register unsigned char * wrptr asm("r28");
unsigned char * dstart1;
unsigned char * dstart2;
unsigned char mask;

unsigned char dispdata0[48][30]=
{
 {0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x82,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {}, {}, {}, {},  {}, {}, {}, {},
 {}, {}, {}, {},  {}, {}, {}, {},
 {0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x82,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {}, {}, {}, {},  {}, {}, {}, {},
 {}, {}, {}, {},  {}, {}, {}, {},
 };
unsigned char dispdata1[48][30]=
{
 {0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x82,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0xaa},
 {}, {}, {}, {},  {}, {}, {}, {},
 {}, {}, {}, {},  {}, {}, {}, {},
 {0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x82,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0x55},
 {}, {}, {}, {},  {}, {}, {}, {},
 {}, {}, {}, {},  {}, {}, {}, {},
};

void setup() {                
  // initialize the digital pin as an output.
  DDRB=0x1F;
  DDRD=0x20;
//  pinMode(0, OUTPUT);  //dbg
//  pinMode(1, OUTPUT);  //B1 SH_CP 
//  pinMode(2, OUTPUT);  //B2 ST_CP
//  pinMode(3, OUTPUT);  //B3 OE
//  pinMode(4, OUTPUT);  //B4 DS
//  pinMode(13, OUTPUT);  //B5 clk board

  DDRA=0xff;
  DDRC=0xff;  
  
    //UART Initialisation
  UCSR0A |= (1<<U2X0);                                 // Double up UART
  UCSR0B |= (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);    // UART RX, TX und RX Interrupt enable
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00)             ;    // Asynchrous 8N1 
  UBRR0H = 0;
  UBRR0L = 3;  //8;  //1; //Baud Rate 1 MBit   --> 0% Error at 16MHz :-)
        //guess 8 230400 16 115200
        //7 1/4 M
        //3 1/2M
        //1 1M
  remain=0;
  rxstate=0;
  dstart1=&dispdata0[0][0];
  dstart2=&dispdata0[ROWS][0];
  rxbuffer=1;
  sei();

  UDR0='S';

 // Serial.print("bitlair\n");  

  mask=0x00;
}



ISR(USART0_RX_vect) 
{
  unsigned char b;
  b=UDR0;
  if(rxstate==3)
  {
      if(remain)
      {
        *wrptr=b;
         wrptr++;
         remain--;
      }else{
        UDR0='!';
        rxstate=0;
	if(rxbuffer==0)
	{
  	   dstart1=&dispdata0[0][0];
  	   dstart2=&dispdata0[0+ROWS][0];
	   rxbuffer=1;
	   //mask=0x00;
	}else{
  	   dstart1=&dispdata1[0][0];
  	   dstart2=&dispdata1[0+ROWS][0];
	   rxbuffer=0;
	   //mask=0xFF;
	   //mask=0xff;
	}
      }
  }else{
//  while((UCSR0A & 0x20)==0){};
//  UDR0=b^0x20;
//  while((UCSR0A & 0x20)==0){};
//}
#if 1
  switch(rxstate)
  {
    case 0:
      if(b==':')
      {
        UDR0='H';
        rxstate++;
      }else{
        UDR0='?';
          rxstate=0;
      }
      break;
    case 1:
      remain=b;
      remain<<=8;
      UDR0='L';
      rxstate++;
      break;
    case 2:
      remain+=b;
      if (remain>(30*24*2))
        remain=30*24*2;
      UDR0='D';
      rxstate++;
      wrptr=(rxbuffer==0) ? &dispdata0[0][0] : &dispdata1[0][0];
      break;
    case 3:  //direct handled
      break;
    default:
       rxstate=0;
       UDR0='?';
       break;
    }    
  }
#endif
}


//  pinMode(1, OUTPUT);   //B1 SH_CP 
//  pinMode(2, OUTPUT);   //B2 ST_CP
//  pinMode(3, OUTPUT);   //B3 OE
//  pinMode(4, OUTPUT);   //B4 DS
//  pinMode(13, OUTPUT);  //D5 clk board

int xdel;
unsigned char dat1,dat2;

void loop() {
  unsigned char x,y;
  unsigned char delaycnt;
  while(1)
  {  
	//mask^=0xaa;
  	asm("SBI 0x05,3");          //out dis
  	asm("CBI 0x05,1");          //shift low
  	asm("SBI 0x05,4");          //data high
  	asm("SBI 0x05,1");          //shift high
  
  	unsigned char * dp1;
  	unsigned char * dp2;
  	dp1=dstart1;
  	dp2=dstart2;
  
	for(y=0;y<ROWS;y++)
	{
//	      asm("SBI 0x05,3");        // out dis
	      asm("CBI 0x05,3");        // out en 
	      asm("SBI 0x05,1");        //shift
	//	asm("nop");		asm("nop");		asm("nop");		asm("nop");
	      asm("CBI 0x05,1");        //
	      asm("SBI 0x05,3");        // out dis
	      asm("CBI 0x05,4");        //
	      asm("SBI 0x05,2");        //
	      asm("CBI 0x05,2");  	//

	   //was 5 
	//    delayMicroseconds(15);
	    //5 ... 55
	    //for(delaycnt=30;delaycnt;delaycnt--)
	    for(xdel=0;xdel<37;xdel++)
	    //for(xdel=0;xdel<35;xdel++)
		{
			asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");
		}

	    for (x=0;x<DEPTH;x++)
	    {
	      dat1= *dp1++;
	      dat2= *dp2++;
	      PORTA=dat1 ^mask;
	      PORTC=dat2 ^mask;
	      asm("SBI 0x0b,5");    //PD7
	      asm("CBI 0x0b,5");
	    }
	    PORTA=0;
	    PORTC=0;
	      asm("CBI 0x05,3");          // out en
			// was 600
	    //for(xdel=0;xdel<55;xdel++)
	//45 also ok
	    for(xdel=0;xdel<55;xdel++)
		{
			asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");
		}
	    //delayMicroseconds(500); //50   //100
	      asm("SBI 0x05,3");          //out dis
	  }
	}
}

int main(void)
{
	setup();
	while(1) 
	{
		loop();
	}
}
