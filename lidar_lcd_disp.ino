#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
#include <SoftwareSerial.h>
#include "TFMini.h"
TFMini tfmini;
 
SoftwareSerial SerialTFMini(2, 3);          //The only value that matters here is the first one, 2, Rx
 
void getTFminiData(int* distance, int* strength)
{
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  if (SerialTFMini.available())
  {
    rx[i] = SerialTFMini.read();
    if (rx[0] != 0x59)
    {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59)
    {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
      }
      i = 0;
    }
    else
    {
      i++;
    }
  }
}
 
 
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(115200);       //Initialize hardware serial port (serial debug port)
  while (!Serial);            // wait for serial port to connect. Needed for native USB port only
  Serial.println ("Initializing...");
  SerialTFMini.begin(TFMINI_BAUDRATE);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&SerialTFMini);            //Initialize the TF Mini sensor
}
 
void loop()
{
  int distance = 0;
  int strength = 0;
 
  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if (distance)
    {
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
 
      lcd.setCursor(0, 0);
      lcd.print("Dis: ");
      lcd.print(distance);
      lcd.print(" cm");
 
      lcd.setCursor(0, 1);
      lcd.print("Str: ");
      lcd.print(strength);
    }
  }
  delay(500);
  lcd.clear();
}
