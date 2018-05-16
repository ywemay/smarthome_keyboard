/**
 * I2C Keyboard
 */

#include <MultitapKeypad.h>
#include <Wire.h>

const byte ROW0 = 9;
const byte ROW1 = 8;
const byte ROW2 = 7;
const byte ROW3 = 6 ;
const byte COL0 = 5;
const byte COL1 = 4;
const byte COL2 = 3                                                                                                                                                              ;
const byte BEEP = 2;

// The pin that triggers interrupt in the hooked device
const byte signalPin = 10;

#define KEYBOARD_ID 2

MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2 );
Key key;

char ch[3];

void setup() {
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, HIGH);
  
  tone( BEEP, 4000, 50 );
  
  Wire.begin(KEYBOARD_ID);  // join i2c bus with given address
  Wire.onRequest(requestEvent); // register event
  strcpy(ch, "EE");
}

void loop() {
  
  key = kpd.getKey(); 
  strcpy(ch, "KE");
  
  if ( key.character > 0 )
    ch[1] = char(key.character);
  switch ( key.state ) {
  case KEY_DOWN:
  case MULTI_TAP:
    tone( BEEP, 5000, 20 );
    informMaster();
    kpd.resetTapCounter();
    break;
  case LONG_TAP:
    tone( BEEP, 5000, 20 );
    ch[0] = 'L';
    informMaster();
    break;
  case MULTI_KEY_DOWN:
    // Do nothing but beep
    tone( BEEP, 4000, 100 );
    break;
  }
}

/**
 * Pulls LOW masters interrupt for tiny period of time
 */
void informMaster(){
  digitalWrite(signalPin, LOW);
  delay(5);
  digitalWrite(signalPin, HIGH);
}

/**
 * On interrupt from I2C wire - answer with key code
 */
void requestEvent() {
  Wire.write(ch); // respond with message
}
