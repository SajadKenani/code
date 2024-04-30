#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);


#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
int spo2;

#define buzzer 2

void setup() {
  Serial.begin(9600);
lcd.begin();
lcd.print("Welcome..");
delay(2000);
lcd.clear();
pinMode(buzzer,OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

}

void loop() {
  long irValue = particleSensor.getIR();
    float temp = particleSensor.readTemperature();


    int hr = map(irValue, 5000, 140000, 0, 120);
    hr = constrain(hr, 0, 120);

    spo2 = map(hr, 60, 89, 90, 99);
    spo2 = constrain(spo2, 0, 99);
    if (irValue < 50000)
    {
      beatAvg = 0;
      temp = 0;
      spo2 = 0;
      Serial.println(" No finger?");
    }
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print("  temp=");
    Serial.print(temp);

    Serial.print(", Avg BPM=");
    Serial.print(hr);
    Serial.print(", spo2=");
    Serial.println(spo2);
lcd.setCursor(0,0);
lcd.print("HR=");
lcd.print(hr);
lcd.print("  ");
lcd.setCursor(7,0);
lcd.print("spo2=");
lcd.print(spo2);
lcd.print("  ");


if(hr>10&&hr<55)
{
digitalWrite(buzzer,1);
delay(100);
lcd.setCursor(0,1);
lcd.print("HR is LOW");
delay(1000);
digitalWrite(buzzer,0);
lcd.clear();
}
  

if(hr>85&&hr<110)
{
digitalWrite(buzzer,1);
delay(100);
lcd.setCursor(0,1);
lcd.print("HR is HIGH");
delay(1000);
digitalWrite(buzzer,0);
lcd.clear();
}

    
}
