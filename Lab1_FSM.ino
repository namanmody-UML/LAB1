#define RED       10
#define GREEN      9
#define YELLOW     8
#define buttonPin 11
#define buzzer    13
#define latch      3  //74HC595  pin 3 STCP
#define clock      2  //74HC595  pin 2 SHCP
#define data       4  //74HC595  pin 4 DS

enum State_enum {SigR, SigG, SigY};

uint8_t state = SigR;

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void setup() 
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT); 
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT); 
  reset_Display();
}

void Display_RG()
{
  int count = 15;
  while(count >= 0)
  {
      digitalWrite(latch,LOW);
      shiftOut(data,clock,MSBFIRST,table[count]);
      digitalWrite(latch,HIGH);
      delay(1000);
      count--;
  }
}

void Display_Y()
{
    int count = 3;
    while(count >= 0)
    {
      digitalWrite(latch,LOW);
      shiftOut(data,clock,MSBFIRST,table[count]);
      digitalWrite(latch,HIGH);
      delay(1000);
      count--;
    }
}

void reset_Display()
{
    digitalWrite(latch,LOW);
    shiftOut(data,clock,MSBFIRST,table[15]);
    digitalWrite(latch,HIGH);
}

void FSM()
{ 
  switch(state)
  {
    case(SigR):
       digitalWrite(RED, HIGH);
       Display_RG();
       digitalWrite(RED, LOW);
       reset_Display();
       digitalWrite(buzzer, HIGH);
       delay(3000);
       digitalWrite(buzzer,LOW);
       state = SigG;
     case(SigG):
       digitalWrite(GREEN, HIGH);
       Display_RG();
       digitalWrite(GREEN, LOW);
       reset_Display();
       digitalWrite(buzzer, HIGH);
       delay(3000);
       digitalWrite(buzzer,LOW);
       state = SigY;
     case(SigY):
       digitalWrite(YELLOW, HIGH);
       Display_Y();
       digitalWrite(YELLOW, LOW);
       reset_Display();
       digitalWrite(buzzer, HIGH);
       delay(3000);
       digitalWrite(buzzer,LOW);
       state = SigR;             
  }
}

void loop() 
{
  digitalWrite(RED, HIGH);
  delay(1000);
  digitalWrite(RED, LOW);
  delay(1000);
  if (digitalRead(buttonPin) == LOW)
  { 
    while(1)
      FSM();
  }
}
