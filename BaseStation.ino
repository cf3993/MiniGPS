/*
 * BaseStation Firmware
 * Charles Frake
 * 14/06/15
 *
 * Sketch for BaseStation to MiniGPS
 *
 * ======================================================
 * Changelog:
 * - 00/06/15
 * - Initial Release V1.0.0
 *
 * ======================================================
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial btSerial(12, 11);

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

String text;
int x, y, FS;
boolean monitored = false;
boolean trigger = false;		// Initialise alarm as untriggered
boolean bikefound = false;	// Search for bike on each power up
int alarmTrigger = 9;				// Trigger alarm if this is received
int recvByte;
int buzzer = 13;						// Buzzer pin

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.invertDisplay(true);	//Checking for dead pixels
  delay(2000);
  display.invertDisplay(false);
  delay(1000);
  displayText("Initialising Serial...", 0, 0, 1, true);
  Serial.begin(9600);	//DEBUGGING
  delay(1000);
  displayText("Initialising BT...", 0, 0, 1, true);
  btSerial.begin(9600);
  delay(1000);
  displayText("Checking Buzzer...", 0, 0, 1, true);
  soundBuzzer(200, 100, 3, false);
  delay(1000);
}

void loop()
{
  Serial.println("ENTERED THE LOOP...");
  if (btSerial.available())
  {
    Serial.println(btSerial.read());
    Serial.println("GOT BT SERIAL M8");
    digitalWrite(buzzer, HIGH);
    delay(100);
    if (recvByte == alarmTrigger)
    { 
      alertTriggered();
    }
    digitalWrite(buzzer, LOW);
    delay(300);
  }
  delay(500);
}

void displayText(String text, int x, int y, int FS, bool clearScreen)
{
  // FOR DEBUGGING PURPOSES ONLY
  Serial.println(text);

  if (clearScreen == true)
  {
    display.clearDisplay();
  }
  display.setTextColor(WHITE);
  display.setTextSize(FS);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}

void alertTriggered()	//Go nuts!
{
  while (trigger == true)
  {
    soundBuzzer(200, 100, 10, true);
  }
}

void soundBuzzer(int on, int off, int count, bool trigger)
{
  for (int c = 0; c < count; c++)
  {
    if (trigger == true)
    {
      display.invertDisplay(true);
    }
    digitalWrite(buzzer, HIGH);
    delay(on);
    if (trigger == true)
    {
      display.invertDisplay(false);
    }
    digitalWrite(buzzer, LOW);
    delay(off);
  }
}










