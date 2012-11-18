//#define __AVR_ATmega644__

#include <WProgram.h>
#include <inttypes.h>
#include <iom644p.h>


unsigned int remain;
unsigned char rxstate;
unsigned char * wrptr;
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(1, OUTPUT);  //B1 SH_CP 
  pinMode(2, OUTPUT);  //B2 ST_CP
  pinMode(3, OUTPUT);  //B3 OE
  pinMode(4, OUTPUT);  //B4 DS
  pinMode(13, OUTPUT);  //B5 clk board
  pinMode(0, OUTPUT);  //dbg
  
  DDRA=0xff;
  DDRC=0xff;  
  
//  Serial.begin(115200);
  
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
  sei();

  UDR0='S';

 // Serial.print("bitlair\n");  

}

unsigned char dispdata0[48][30]=
{{0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0},
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

#define DEPTH 30
#define ROWS 24

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
      wrptr=&dispdata0[0][0];
      break;
    case 3:  //direct handled
      break;
    default:
       rxstate=0;
       UDR0='?';
       break;
    }    
  }
//  if (b == CMD_NEW_DATA)  {UDR0=b; pos=0; ptr=display_buffer;  return;}    
//  if (pos == 384) {UDR0=b; return;} else {*ptr=b; ptr++; pos++;}  
}
#endif


//  pinMode(1, OUTPUT);   //B1 SH_CP 
//  pinMode(2, OUTPUT);   //B2 ST_CP
//  pinMode(3, OUTPUT);   //B3 OE
//  pinMode(4, OUTPUT);   //B4 DS
//  pinMode(13, OUTPUT);  //D5 clk board




void loop() {
  unsigned char x,y,z,dat1,dat2;
  
//      delayMicroseconds(20);
      asm("SBI 0x05,3");          //
//      delayMicroseconds(20);
      
  asm("CBI 0x05,1");          //
//      delayMicroseconds(20);
  asm("SBI 0x05,4");          //data high
//      delayMicroseconds(20);
  asm("SBI 0x05,1");          //shift
//      delayMicroseconds(20);
  
  unsigned char * dp1;
  unsigned char * dp2;
  
  for(y=0;y<ROWS;y++)
  {
//      delayMicroseconds(20);
      asm("SBI 0x05,3");          // out dis
//      delayMicroseconds(20);
      asm("SBI 0x05,1");          //shift
//      delayMicroseconds(20);
      asm("CBI 0x05,1");          //
//      delayMicroseconds(20);
      asm("CBI 0x05,4");         //
//      delayMicroseconds(20);
      asm("SBI 0x05,2");          //
//      delayMicroseconds(20);
      asm("CBI 0x05,2");  
//      delayMicroseconds(20);

      dp1=&dispdata0[y][0];
      dp2=&dispdata0[y+ROWS][0];
    
    delayMicroseconds(10);
    for (x=0;x<DEPTH;x++)
    {
      dat1=dp1[x];
      dat2=dp2[x];
      PORTA=dat1;
      PORTC=dat2;
//      delayMicroseconds(1);
      asm("SBI 0x0b,5");    //PD7
//      delayMicroseconds(1);
      asm("CBI 0x0b,5");
//      delayMicroseconds(1);
    }
    PORTA=0;
    PORTC=0;
//      delayMicroseconds(50);
      asm("CBI 0x05,3");          // out en
//    delayMicroseconds(50); //50   //100
    int xdel;
    for(xdel=0;xdel<100;xdel++)
	{
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}

    //delayMicroseconds(500); //50   //100
      asm("SBI 0x05,3");          //out dis
     //delayMicroseconds(10);
  }
//     delayMicroseconds(10);
//  UDR0='U';

}

