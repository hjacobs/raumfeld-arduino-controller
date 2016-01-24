// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int maxVolume = 60;

int reading;
int button1;
int lastButton1;
long lastDebounce1;
int button2;
int lastButton2;
int lastDebounce2;
long debounceDelay = 50;
int prevVal = -1;

void setup() {
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);

    Serial.begin(9600);
    // set up the number of columns and rows on the LCD
    lcd.begin(16, 2);

    // Print a message to the LCD.
    lcd.clear();
}

void loop() {
    // check the status of the switch
    reading = digitalRead(8);

    if (reading != lastButton1) {
        lastDebounce1 = millis();
    }

    if ((millis() - lastDebounce1) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != button1) {
            button1 = reading;

            if (button1 == LOW) {
                Serial.println("prev");
            }
        }
    }
    lastButton1 = reading;

    // next
    reading = digitalRead(9);

    if (reading != lastButton2) {
        lastDebounce2 = millis();
    }

    if ((millis() - lastDebounce2) > debounceDelay) {
        // if the button state has changed:
        if (reading != button2) {
            button2 = reading;

            if (button2 == LOW) {
                Serial.println("next");
            }
        }
    }
    lastButton2 = reading;

    unsigned long val = (1023 - analogRead(A0));
    val = sqrt(val * 10) * 10.13; // 0 - 1024
    val = val * maxVolume/1024;

    if (val != prevVal) {
        if (prevVal == -1)
        {
            lcd.clear();
            lcd.print("Volume: ");
        }
        Serial.print("volume=");
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
        lcd.setCursor(0, 0);
        // read all the available characters
        while (Serial.available() > 0) {
            // display each character to the LCD
            lcd.write(Serial.read());
        }
    }
    delay(20);
}
