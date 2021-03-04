#define RED       10  // Red LED    pin 10 
#define GREEN      9  // Green LED  pin 9 
#define YELLOW     8  // Yellow LED pin 8
#define buttonPin 11  // Button     pin 11
#define buzzer    13  // Buzzer     pin 13
#define latch      3  //  74HC595   pin 3 STCP
#define clock      2  //  74HC595   pin 2 SHCP
#define data       4  //  74HC595   pin 4 DS

// Enumerated the states to represent the different the routines for each LED color, 
// i.e. red, green and yellow
enum State_enum {SigR, SigG, SigY};   

uint8_t state = SigR;                 // Initialized state variable to SigR

// character array of hexadecimal values that is used in outputting the values from 0 to 15 on the 4-digit 7-segment display
unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00}; 

// purpose of this function that assigns the pin modes and also resets the seven segment display
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

// purpose of this function that displays the count from 15 to 0 on the 4-digit 7-segment display 
// with a delay of 1 second between each count for red and green LED's.
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

// purpose of this function that displays the count from 3 to 0 on the 4-digit 7-segment display 
// with a delay of 1 second between each count for yellow LED's. 
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

// purpose of this function is to reset the display of the 4-digit 7-segment display back to 15. 
void reset_Display()
{
    digitalWrite(latch,LOW);
    shiftOut(data,clock,MSBFIRST,table[15]);
    digitalWrite(latch,HIGH);
}


// purpose of this function is to toggle the states in sequence of SigR->SigG->SigY 
// to execute the following routine for each state,
// 1) switch on the LED 
// 2) display the remaining time depending on the LED on the 4-digit 7-segment display 
// 3) switch off the LED once timer reaches 0
// 4) reset the 4-digit 7-segment display using the reset() function
// 5) switch on the buzzer 
// 6) keep it on for 3 seconds
// 7) switch off the buzzer 
// 8) switch the state 
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


// purpose of this function is to flash the red LED in a loop till an event triggered button pin is pressed
// that indefinitely loops the FSM() function. 
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
