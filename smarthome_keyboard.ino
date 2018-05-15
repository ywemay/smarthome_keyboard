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
//const byte COL3 = 11;
const byte BEEP = 2;

#define KEYBOARD_ID 2

// creates kpd as MultitapKeypad object
// for matrix 4 x 3 keypad
MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2 );
// for matrix 4 x 4 keypad
//MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, COL3 );
// creates key as Key object
Key key;

char ch[3];

void setup() {
  tone( BEEP, 4000, 50 );
  Wire.begin(KEYBOARD_ID);                // join i2c bus with given address
  Wire.onRequest(requestEvent); // register event
  strcpy(ch, "EE");
}

void loop() {
  key = kpd.getKey();

  char ky[3] = {'K', 'E' , '\0'};
  if ( key.character > 0 )
    ky[1] = char(key.character);

  switch ( key.state ) {
  case KEY_DOWN:
  case MULTI_TAP:
    tone( BEEP, 5000, 20 );
    if ( key.state == MULTI_TAP ) {
      if ( key.tapCounter < 2 )
        ky[0] = 'M';
      else {
        kpd.resetTapCounter();
      }
    }
    break;
  case LONG_TAP:
    tone( BEEP, 5000, 20 );
    ky[0] = 'L';
    break;
  case MULTI_KEY_DOWN:
    tone( BEEP, 4000, 100 );
    break;
  }

  ch[0] = ky[0];
  ch[1] = ky[1];
  ch[2] = ky[2];
}

void requestEvent() {
  Wire.write(ch); // respond with message
  // as expected by master
  // No key pressed:
  strcpy(ch, "NNN");
}
