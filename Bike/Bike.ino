/*
 * Bike Firmware
 * Charles Frake
 * 14/06/15
 *
 * Sketch for Bike to MiniGPS
 *
 * ======================================================
 * Changelog:
 * - 00/06/15
 * - Initial Release V1.0.0
 *
 * ======================================================
 */

#include <SoftwareSerial.h>
#include <EEPROM.h>

// Set up Global Variables (Keep these to a minimum to save memory!
SoftwareSerial btSerial(11, 10); //(RX, TX)
boolean bikeParked;
int x, y, z;			// Accelerometer Axis
int x1, y1, z1, x2, y2, z2;     // Trigger values of Accelerometer
int firstrun;
int trigFlag = 0; // If value is exceeded 5 times, trigger alarm... Stops random alarm triggers
bool alarm;

#define buzzer 13   // CHANGE ACCORDING TO CIRCUIT LAYOUT!
#define alarmTrigger 999	// Send this to BaseStation to sound alarm
#define led 14
#define btPwr 12   // Turn off HC-05 module until needed. Saves battery and stops interference with ADXL355

void setup()
{
  // Init Inputs/Outputs
  pinMode(2, INPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(btPwr, LOW);

  // RESET EEPROM AND ALL PREVIOUS SETTINGS!!!
  int resetState = digitalRead(2);
  delay(500);
  if (resetState > 0) {
    Serial.println("GOING FOR A RESET... ERASING EEPROM...");
    __RESET__();
  }
  /////////////////////////////////////////////

  Serial.begin(9600);
  // Initialise Bluetooth Serial Module
  btSerial.begin(38400);

  // Check if first start
  firstRun();
}

void loop()
{
  // Flash LED to show that alarm is armed
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
  Serial.println("");
  btSerial.println("Parked...");
  // Read data from accelerometer
  for (int n = 0; n < 20; n++)
  {
    x = analogRead(15);
    y = analogRead(16);
    //z = analogRead(17)/2;
    //btSerial.println(x);
    delay(5);
    //btSerial.println(y);
    delay(5);
    //btSerial.println(z);
    delay(5);
    Serial.println(x);
    delay(5);
    Serial.println(y);
    delay(5);
    Serial.println(z);
    delay(5);
    Serial.println(trigFlag);
    if (x > x1) {
      trigFlag++;
      if (trigFlag > 10) {
        Alarm();
      }
    }
    else if (x < x2) {
      trigFlag++;
      if (trigFlag > 10) {
        Alarm();
      }
    }
    if (y > y1) {
      trigFlag++;
      if (trigFlag > 10) {
        Alarm();
      }
    }
    else if (y < y2) {
      trigFlag++;
      if (trigFlag > 10) {
        Alarm();
      }
    }

    if (x < x1 && x > x2)
    {
      trigFlag = 0;
    }
  }
}

void firstRun()
{
  // Check EEPROM for any previous settings
  // Read EEPROM Address 5
  // If data is not 0, just monitor bike
  // return;
  //firstrun = EEPROM.read(5);
  //Serial.println(firstrun);
  //if (firstrun > 0)
  //{
  //return;
  //}

  // If none, then perform firstRun()
  // ======
  // Wait 10 seconds for bike to settle (Parked) // 2 FOR DEBUGGING
  soundBuzzer(500, 500, 10, false);
  // Buzz each second for 10 second delay
  // Read X, Y, Z data from Accelerometer
  // Write these values to EERPOM Position 1, 2, 3 respectively
  writePosition();
  // Write firstRun complete bit to address 5
  //EEPROM.write(5, 1);
}

void writePosition()
{
  x = analogRead(15);
  y = analogRead(16);
  x1 = x + 20;
  x2 = x - 20;
  y1 = y + 20;
  y2 = y - 20;
  //EEPROM.write(x, 1);
  delay(10);
  //EEPROM.write(y, 2);
  delay(10);
  //EEPROM.write(z, 3);
  delay(10);
  soundBuzzer(2000, 0, 1, false);
  Serial.println(x);
  Serial.println(x1);
  Serial.println(x2);
}

void soundBuzzer(int on, int off, int count, bool alarm)
{
  for (int c = 0; c < count; c++)
  {
    if (alarm == true)
    {
      btSerial.println(alarmTrigger);
    }
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(on);
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    delay(off);
  }
}

void Alarm()	// Send alarm signal to BaseStation
{
  digitalWrite(btPwr, HIGH);
  Serial.write(btSerial.println(999));
  delay(1000);
  soundBuzzer(400, 100, 1000, true);
}

void __RESET__() {
  digitalWrite(led, HIGH);
  soundBuzzer(5000, 0, 1, false);
  for ( int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
  soundBuzzer(2000, 0, 1, false);
  digitalWrite(led, LOW);
  delay(60000);
}




