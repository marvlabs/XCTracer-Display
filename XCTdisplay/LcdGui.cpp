///////////////////
// LCD Nokia 3110, 5110
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Fonts/FreeSansBold9pt7b.h"
#define FONT FreeSansBold9pt7b

///////////////////
#include "LcdGui.h"
#include "xct_logo.h"

///////////////////
// Software SPI (slower updates, more flexible pin options):
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 Lcd = Adafruit_PCD8544(3, 4, 5, 6, 7);

///////////////////
#define T_DIVSEC  1000L
#define T_MIN  60L
#define T_10MIN 10*T_MIN

///////////////////
void InitLcd() {
  // Set up the LCD, display logo
  Lcd.begin();
  Lcd.setContrast(LCD_CONTRAST);
  Lcd.clearDisplay();
  Lcd.drawXBitmap(0, 0, LOGO_NAME, LOGO_WIDTH, LOGO_HEIGHT, BLACK);
  Lcd.display(); // show splashscreen
  delay(1000);
}

///////////////////
// Call with -100 .. +100
// e.g:
//#define VMAXCLIMB 4.0
//DrawVarioBar(Vario.toFloat()*100/VMAXCLIMB;)
void DrawVarioBar(int percent) {
  if (percent > 100) percent = 100;
  if (percent < -100) percent = -100;
                
  Lcd.drawRect(VX, VY, VW, VH, BLACK);
  Lcd.drawLine(VBARX, VBARY, VBARX2, VBARY, BLACK);
  
  int filledH = VH*percent/200;
  Lcd.fillRect(VX, VBARY, VW, -filledH, BLACK);
}

void DrawSpeedBar(int percent) {
  if (percent > 100) percent = 100;
  if (percent < -100) percent = -100;
                
  Lcd.drawRect(SX, SY, SW, SH, BLACK);
  Lcd.drawLine(SBARX, SBARY, SBARX, SBARY2, BLACK);
  
  int filledW = SW*percent/200;
  if (filledW > 0) {
    Lcd.fillRect(SBARX, SY, filledW, SH, BLACK);
  }
  else {
    Lcd.fillRect(SBARX+filledW, SY, -filledW, SH, BLACK);
  }
}

///////////////////

void DrawGui(int Vario, byte Speed, int HeightGps, int HeightBaro, byte Satellites, const String& Status, float VRaw, char Recording) {
  Lcd.clearDisplay();

  /////////////////////////////  
  Lcd.setTextColor(BLACK);
  Lcd.setFont(&FreeSansBold9pt7b);
  Lcd.setTextSize(1);

  Lcd.setCursor(POS_HBARO);
  Lcd.print(HeightBaro);
  Lcd.setCursor(POS_VARIO);
  Lcd.print(Vario < 0 ? "-" : "+"); 
  Lcd.print(abs(Vario)/10); Lcd.print('.'); Lcd.print(abs(Vario)%10);
  Lcd.setCursor(POS_SPEED);
  Lcd.print(Speed); 
  
  /////////////////////////////  
  //Lcd.setTextColor(BLACK);
  Lcd.setFont();
  Lcd.setTextSize(1);

  Lcd.setCursor(POS_HGPS);
  Lcd.print(HeightGps);

  Lcd.setCursor(POS_TIME);
  unsigned long secsSinceStart = millis() / T_DIVSEC;
  if (secsSinceStart < T_10MIN) {
    Lcd.print(secsSinceStart/T_MIN); Lcd.print('.');
    unsigned long sec = secsSinceStart%T_MIN;
    if (sec < 10) Lcd.print('0');
    Lcd.print(sec);
  }
  else {
    unsigned long minSinceStart = secsSinceStart/T_MIN;
    Lcd.print(minSinceStart/60); Lcd.print(':');
    unsigned long min = minSinceStart%60;
    if (min < 10) Lcd.print('0');
    Lcd.print(min);
  }

  if (Status.length()) {
    Lcd.setCursor(POS_STATUS);
    Lcd.print(Status);
  }
  else {
    Lcd.setCursor(POS_KMH);
    Lcd.print("km/h");

    Lcd.setCursor(POS_MS);
    Lcd.print("m/s");
  }


  Lcd.setCursor(POS_VOLT);
  Lcd.print(VRaw);  Lcd.print('V');

  /////////////////////////////  
  Lcd.setTextColor(WHITE, BLACK);

  Lcd.setCursor(POS_SAT);
  Lcd.print(Satellites);
  Lcd.print(Recording);
  
  /////////////////////////////  

  DrawVarioBar(Vario*10/VMAXCLIMB); 
  DrawSpeedBar((Speed-VAVERAGE)*100/VMAXDIFF);

  Lcd.display();
}

///////////////////
// DEBUG code only
void PrintScreenToSerial() {
  for (unsigned int y = 0; y < Lcd.height(); y++) {
    for (unsigned int x = 0; x < Lcd.width(); x++) {
      Serial.print(Lcd.getPixel(x,y) ? "#":" ");
    }
    Serial.println();
  }
}
///////////////////

