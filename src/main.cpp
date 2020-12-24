#include <Arduino.h>
#include "../.pio/libdeps/uno/Adafruit LiquidCrystal/Adafruit_LiquidCrystal.h"
#include "../.pio/libdeps/uno/Keypad/src/Keypad.h"
#define red 10
#define green 11


const byte passcodeLength = 4; // Set the length of the passcode
char setPasscode[passcodeLength]; // Create a character array to store the correct passcode in
char passcode[passcodeLength];
bool noGrant;
const byte rows = 4; // Set a rows variable to 4 for use in creating the keypad object
const byte cols = 4; // Set a cols variable to 4 for use in creating the keypad object
// Define which places on the keypad will be translated to which characters using a char array
char keys[rows][cols] = {
        {'D','#','0','*'},
        {'C','9','8','7'},
        {'B','6','5','4'},
        {'A','3','2','1'}
};
// Define which Arduino pins the keypad will be plugged into
byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3, 2};

Adafruit_LiquidCrystal lcd(0); // Create the lcd object for controlling Adafruit LCD
// Create the keypad object for controlling the Keypad using variables defined earlier
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);


// Function to record characters from the keypad into an array
void getPasscode(const String& msg, char passcodeArray[passcodeLength]) {
    lcd.begin(16, 2);
    lcd.print(msg);
    for (byte i = 0; i < passcodeLength; i++) {
        char key = NO_KEY;
        while (key == NO_KEY) {
            key = keypad.getKey();
        }
        passcodeArray[i] = key;
    }
}


void setup() {
    // Call the getPasscode function so the user can set the correct passcode
    getPasscode("Enter a passcode", setPasscode);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
}


void loop() {
    lcd.clear(); // Clear the lcd screen
    noGrant = false; // Make sure that noGrant is false at the start of the loop
    // Call the getPasscode function so the user can verify their passcode (this will loop)
    getPasscode("Verify passcode:", passcode);

    // Compare the contents of the two arrays and break from the loop if there is a discrepancy
    for (byte i = 0; i < passcodeLength; i++) {
        if (setPasscode[i] != passcode[i]) {
            noGrant = true;
            break;
        }
    }

    lcd.clear();
    // If the two passcode arrays matched
    if (!noGrant) {
        lcd.print("Access Granted!");
        digitalWrite(green, HIGH);
        delay(1500);
        digitalWrite(green, LOW);
    } else {
        lcd.print("Access Denied!");
        digitalWrite(red, HIGH);
        delay(1500);
        digitalWrite(red, LOW);
    }
}