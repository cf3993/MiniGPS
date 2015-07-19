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
int buzzer = 13; 	// CHANGE ACCORDING TO CIRCUIT LAYOUT!
int x, y, z;			// Accelerometer Axis
int x1, y1, z1, x2, y2, z2;     // Trigger values of Accelerometer
int firstrun;
int alarmTrigger = 999;	// Send this to BaseStation to sound alarm
int led = 14;

void setup()
{
  // Init Inputs/Outputs
  pinMode(2, INPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);

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
  delay(500);
  // Flash LED to show that alarm is armed
  Serial.println("");
  btSerial.println("Parked...");
  // Read data from accelerometer
  x = analogRead(15);
  y = analogRead(16);
  //z = analogRead(17)/2;
  btSerial.println(x);
  delay(5);
  btSerial.println(y);
  delay(5);
  btSerial.println(z);
  delay(5);
  Serial.println(x);
  delay(5);
  Serial.println(y);
  delay(5);
  Serial.println(z);
  delay(5);
  if (x > x1) {
    Alarm();
  }
  delay(2);
  if (x < x2) {
    Alarm();
  }
  delay(2);
  if (y > y1) {
    Alarm();
  }
  delay(2);
  if (y < y2) {
    Alarm();
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
  // Wait 10 seconds for bike to settle (Parked)
  soundBuzzer(500, 500, 10);
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
  soundBuzzer(2000, 0, 1);
  Serial.println(x);
  Serial.println(x1);
  Serial.println(x2);
}

void soundBuzzer(int on, int off, int count)
{
  for (int c = 0; c < count; c++)
  {
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
  if (btSerial.available())
  {
    Serial.write("Yeah we got BT..");
    btSerial.write(alarmTrigger);
  }
  delay(1000);
  soundBuzzer(400, 100, 1000);
}

void __RESET__() {
  digitalWrite(led, HIGH);
  soundBuzzer(5000, 0, 1);
  for ( int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
  soundBuzzer(2000, 0, 1);
  digitalWrite(led, LOW);
  delay(60000);
}




