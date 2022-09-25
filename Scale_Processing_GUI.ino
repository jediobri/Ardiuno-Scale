//Headers
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#include <Wire.h>

//pins:
//Scale 1
const int HX711_dout = 10; //mcu > HX711 dout pin
const int HX711_sck = 11; //mcu > HX711 sck pin

// Scale 2
const int HX711_dout_2 = 3; //mcu > HX711 dout pin
const int HX711_sck_2 = 2; //mcu > HX711 sck pin


//const byte buttonPin = 7; // button pin for Tare function
HX711_ADC LoadCell(HX711_dout, HX711_sck);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //second loadcell object

const int calVal_eepromAdress = 0;
const int calVal_eepromAdress_2 = 100;
unsigned long t = 0;

int delayPeriod = 1000; // 1 second delay


//


void setup(){
  
  Wire.begin();
  Serial.begin(9600); delay(10);
 
  //Serial.println("Starting...");
  
  LoadCell.begin();
  LoadCell_2.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  float calibrationValue_2; // calibration value (see example file "Calibration.ino")
  //calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
  
  

#if defined(ESP8266)|| defined(ESP32) // evaluated only at complie time
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif

  EEPROM.get(calVal_eepromAdress, calibrationValue); // fetch the calibration value from eeprom
  EEPROM.get(calVal_eepromAdress_2, calibrationValue_2); // fetch the calibration value from eeprom
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  LoadCell_2.start(stabilizingtime, _tare);
  
  if (LoadCell.getTareTimeoutFlag()) {
    //Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    LoadCell_2.setCalFactor(calibrationValue_2); // set calibration value (float)
    //Serial.println("Startup is complete");
    
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  getLoadCell_1_Data();

}

float percentData(float i){
  //full EBR bottle
  float percent;
  percent = (((i - 15)/40)*100); 
  return percent;
  }

void getLoadCell_1_Data(){
    //Serial.println("load cell");
    static boolean newDataReady = 0;
    const int serialPrintInterval = 1000; //increase value to slow down serial print activity
    
    while(true){
     
      
      // check for new data/start next conversion:
      if (LoadCell.update() && LoadCell_2.update() ) newDataReady = true;
      
      // get smoothed value from the dataset:
      else if (newDataReady) { //loop for printing 
    
        if (millis() > t + serialPrintInterval) {
           
            float i = LoadCell.getData();
            float j = LoadCell_2.getData();
            Serial.print(i);
            Serial.print(",");
            Serial.println(j);

            newDataReady = 0;
            t = millis();
        }
      }
    }
}
