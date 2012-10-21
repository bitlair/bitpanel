void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  DDRA=0xff;
  DDRC=0xff;
  DDRC=0xff;
  DDRF=0xff;
  
}

unsigned char dispdata0[][10]=
{{0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00},
 
 {0x55,0x00,0x00,0x00,0x00, 0x80,0x00,0x00,0x00,0x00},
 {0x40,0x41,0x00,0x00,0x00, 0x80,0x00,0x00,0x80,0x00},
 {0x40,0x40,0x00,0x40,0x00, 0x80,0x00,0x00,0x00,0x00},
 {0x55,0x01,0x01,0x50,0x00, 0x80,0x0A,0xA0,0x80,0x2A},
 {0x40,0x41,0x00,0x40,0x00, 0x80,0x20,0x20,0x80,0x80},
 {0x40,0x41,0x00,0x40,0x00, 0x80,0x20,0x20,0x80,0x80},
 {0x40,0x41,0x00,0x50,0x00, 0x80,0x20,0xA0,0x80,0x80},
 {0x55,0x00,0x00,0x00,0x00, 0xAA,0x8A,0x20,0x00,0x00},
 
 {0xAA,0x00,0x00,0x00,0x00, 0x40,0x00,0x00,0x00,0x00},
 {0x80,0x82,0x00,0x00,0x00, 0x40,0x00,0x00,0x40,0x00},
 {0x80,0x80,0x00,0x80,0x00, 0x40,0x00,0x00,0x00,0x00},
 {0xAA,0x02,0x02,0xA0,0x00, 0x40,0x05,0x50,0x40,0x15},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40},
 {0x80,0x82,0x00,0x80,0x00, 0x40,0x10,0x10,0x40,0x40},
 {0x80,0x82,0x00,0xA0,0x00, 0x40,0x10,0x50,0x40,0x40},
 {0xAA,0x00,0x00,0x00,0x00, 0x55,0x45,0x10,0x00,0x00}};

#define DEPTH 10
#define ROWS 24
void loop() {
  unsigned char x,y,en;
  for(y=0;y<ROWS;y++)
  {
    if((y & 7)==0)
    {
       en=1;
    }
    PORTC=0x00;
    PORTK=0x00;
    PORTF=0x00;
    for (x=0;x<DEPTH;x++)
    {
      PORTA=dispdata0[y][x];
      digitalWrite(13, HIGH);   // set the LED on
      digitalWrite(13, LOW);    // set the LED off
//    delay(1);
    }
    if((y & ~7)==0)
    {
      PORTC=en;
    }else if((y & ~7)==8)
    {
      PORTK=en;
    }else if((y & ~7)==0x10)
    {
      PORTF=en;
    }
    en<<=1;
    //delay(1);
    delayMicroseconds(200);
  }
}