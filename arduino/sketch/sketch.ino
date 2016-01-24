// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int maxVolume = 60;

int prevVal = -1;

void setup() {
    Serial.begin(9600);
    // set up the number of columns and rows on the LCD
    lcd.begin(16, 2);

    // Print a message to the LCD.
    lcd.clear();
}

void loop() {
    // check the status of the switch
    unsigned long val = (1023 - analogRead(A0));
    // val = map(val, 0, 1023, 0, 100);
    val = sqrt(val * 10) * 10.13; // 0 - 1024
    val = val * maxVolume/1024;
 
  
    if (val != prevVal) {
        if (prevVal == -1)
        {
            lcd.clear();
            lcd.print("Volume: ");
        }
        Serial.println(val);
      
        // set the cursor to column 0, line 0
        lcd.setCursor(9, 0);
        lcd.print(val);
        lcd.print("   ");
        lcd.setCursor(0, 1);
        for (int i=0; i<16; i++) {
            if (i < val*16/maxVolume) {
                lcd.print((char) 0xff);
            } else {
                lcd.print(" ");
            }
        }
        prevVal = val;
        delay(50);
 
    } else if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
    delay(20);
 
}

