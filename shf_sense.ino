#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
double past_value = 9876;
int reset_counter = 0;
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    Serial.println(measure.RangeMilliMeter * 1.75);
    
    if(measure.RangeMilliMeter * 1.75 < 100 && measure.RangeMilliMeter == past_value)
    {
      reset_counter++;
      Serial.println("error");
      if(reset_counter > 10)
      {
        // NVIC_SystemReset();
        Serial.println(reset_counter);
        digitalWrite(2, LOW);
        delay(20);
        digitalWrite(2, HIGH);
      }
    }
    else
    {
      reset_counter = 0;
    }
  } else {
    //Serial.println(" out of range ");
  }
  past_value = measure.RangeMilliMeter;
  delay(10);
}
