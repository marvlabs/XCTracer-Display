/*********************************************************************
XCTracer -> LCD

Receive XCTracer Vario and GPS sentences over BLE 
and Display the values on an LCD.

Connect BLE Module HM-10 via serial port.
Connect Nokia 3110/5110 via SPI

*********************************************************************/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "NmeaSentence.h"
#include "LcdGui.h"

///////////////////
// Values to control stuff

#define V_FACTOR      157.0	// Battery voltage calibration
#define UPDATE_DELAY  500	// LCD update interval
#define LCD_BACKLIGHT_ON_STARTUP   0	// Start with light or not

#define SERIAL_TIMEOUT 50	// Wait for input lines from BLE
#define BLE_CMD_WAIT  200	// Delay while setting up BLE

// Button press thresholds
#define BUTTON_LONG_PRESS 800
#define BUTTON_MIN_PRESS 10



///////////////////
// Defines for testing and debugging
#define BLE_HWSERIAL    1	// Use BLE over RX/TX pins (instead of test input)

//#define TESTSENTENCES
#ifdef TESTSENTENCES
String TestSentences[3] = { 
  "$GPRMC,213651.800,A,4728.9319,N,00841.9431,E,37.10,183.08,270116,,,A*65",
  "$GPGGA,213652.400,4728.9319,N,00841.9431,E,1,8,0.95,1464.5,M,48.0,M,,*5B",
  "$LXWP0,N,,404.6,+1.21,,,,,,190,,*4F"
};
byte TestId = 0;
#endif

///////////////////
// I/O pins used
#define PIN_LCD_LED   8	// LCD backlight control
#define PIN_BUTTON_1  9	// Input button
#define PIN_VRAW     A3	// Measure battery voltage



///////////////////
// BLE Bluetooth LE module HM-10
#if BLE_HWSERIAL
#define BleHm10 Serial
#define DEBUG_SERIAL_LN(x) do { } while(0)
#define DEBUG_SERIAL(x) do { } while(0)
#else
#include <SoftwareSerial.h>
#define rxPin 8
#define txPin 9
SoftwareSerial BleHm10 =  SoftwareSerial(rxPin, txPin);
#define DEBUG_SERIAL_LN(x) Serial.println(x)
#define DEBUG_SERIAL(x) Serial.print(x)
#endif

//////////////////
// Technical constants

#define BLE_BUFSIZE 100


//////////////////
// Forwards
void buttonPressed();
void buttonLongPressed();
void InitBLE();

//////////////////
// Global vars

// Input string from BLE
char buffer[BLE_BUFSIZE];

// Current values for display
int Vario = 0;
byte Speed = 0;
int HeightGps = 0;
int HeightBaro = 0;
int HeightBaroCorrection = 0;
byte Satellites = 0;
String Status = "";
float VRaw;

// State 
unsigned long ButtonTime = 0;
boolean ButtonDown = 0;
boolean LcdLightOn = LCD_BACKLIGHT_ON_STARTUP;
unsigned long UpdateTime = 0;
char Recording = '-';

//////////////////
// Init code, run once

void setup()   {
#if !BLE_HWSERIAL
  // Alternative for RX/TX pins. 
  // Might have timing issues
  Serial.begin(57600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
#endif  

  BleHm10.setTimeout(SERIAL_TIMEOUT); // Read timeout on input
  
  InitLcd();
  // I/O pins
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_LCD_LED, OUTPUT);
  digitalWrite(PIN_LCD_LED, LcdLightOn);
  
  // Find the BLE module and set communication	
  InitBLE();
  
}

//////////////////
// Main loop

void loop() {

  //////////////////
  // Input data sentences
#ifdef TESTSENTENCES
  TestSentences[TestId++].toCharArray(buffer, BLE_BUFSIZE-1);
  if (TestId >= 3) TestId = 0;
#else
  int read = BleHm10.readBytesUntil('\n', buffer, BLE_BUFSIZE-1);
  buffer[read] = 0;
#endif

  //////////////////
  // If a NMEA line was seen, parse it and fill in the values
  if (buffer[0] == '$') {
    NmeaSentence nmea(buffer);
  
    if (nmea.length() > 0) {
        String type(nmea.name());
        
        if (type.equals(LXWP0)) {
            Vario = nmea.fieldFloat(4)*10;
            HeightBaro = nmea.fieldFloat(3);
            Recording = nmea.fieldChar(1);
        }
        else if (type.equals(GPGGA)) {
            Satellites = nmea.fieldFloat(7);
            HeightGps = nmea.fieldFloat(9);
        }
        else if (type.equals(GPRMC)) {
            Speed = nmea.fieldFloat(7);
        }
    }
    
  }
  
  //////////////////
  // Handle input button, call Action functions
  if (digitalRead(PIN_BUTTON_1) == LOW) {
    if (!ButtonDown) {
      ButtonDown = true;
      ButtonTime = millis();
    }
  }
  else {    
    if (ButtonDown) {
      ButtonTime = millis() - ButtonTime;
      ButtonDown = false;
      if (ButtonTime > BUTTON_LONG_PRESS) {
        buttonLongPressed();
      }
      else if (ButtonTime > BUTTON_MIN_PRESS) {
        buttonPressed();
      }
    }
  }

  //////////////////
  // Update the display all n milliseconds
  if (millis() - UpdateTime > UPDATE_DELAY) {
    UpdateTime = millis();
    VRaw = analogRead(PIN_VRAW)/V_FACTOR;
    DrawGui(Vario, Speed, HeightGps, HeightBaro+HeightBaroCorrection, Satellites, Status, VRaw, Recording);
  }
  else {
    delay(10);
  }
}

//////////////////
// Actions

void buttonPressed() {
  // Toggle LCD backlight
  LcdLightOn = !LcdLightOn;
  digitalWrite(PIN_LCD_LED, LcdLightOn);
  
  //PrintScreenToSerial();
}

void buttonLongPressed() {
  // Take current difference to GPS HEIGHT to calibrate BARO HEIGHT from sensor
  HeightBaroCorrection = HeightGps - HeightBaro;
}


//////////////////
// Set up the HM-10 BLE module

void BleCmd(const char* cmd) {
  BleHm10.write(cmd);
  delay(BLE_CMD_WAIT);
}

void BleReset(unsigned int baud) {
  BleHm10.begin(baud);
  delay(BLE_CMD_WAIT);
  BleCmd("AT+RENEW");
  BleCmd("AT+RESET");
}

void InitBLE()   {
  //  BleReset(19200);
  //  BleReset(38400);
  //  BleReset(57600);
  //  BleReset(115200);
  BleReset(9600);

  // Set name and master role
  BleCmd("AT+NAMEMBLCD");
  BleCmd("AT+ROLE1");

  BleCmd("AT+START");   // Start forwarding

  
//  BleCmd("AT+BAUD2"); // Set Baudrate to 38400
//  BleHm10.begin(38400);
//  BleCmd("AT+IMME1"); // Make it start immediately next time
//  BleHm10.print("AT+RESET"); // Restart and start forwarding
}

//////////////////





