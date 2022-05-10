//Headers
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#include <Wire.h>

//pins:
const int HX711_dout = 21; //mcu > HX711 dout pin
const int HX711_sck = 20; //mcu > HX711 sck pin

int red_light = 5; // pin for red LED 5 is red
int yellow_light = 3; // pin for yellow LED 3 is yellow
int green_light = 6; //pin for green LED 6 is green


//const byte buttonPin = 7; // button pin for Tare function
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

//
// since scale will remain under object, program should start from zero since each power cycle will begin with a tare function
//
int maxWeight = 60; // this is the max weight of bottle
int minWeight = 35; // min weight at which alarming happens
int midWeight = 45; // yellow light


int delayPeriod = 1000; // 1 second delay


//


void setup(){
  
  Wire.begin();
  Serial.begin(57600); delay(10);
 
  Serial.println("Starting...");
  
  LoadCell.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
  
  pinMode(red_light,OUTPUT);
  pinMode(yellow_light,OUTPUT);
  pinMode(green_light,OUTPUT);
  

#if defined(ESP8266)|| defined(ESP32) // evaluated only at complie time
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif

  EEPROM.get(calVal_eepromAdress, calibrationValue); // fetch the calibration value from eeprom
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
    
    
  }
}

void loop() {
  
  loadCellFunction();

}

void loadCellFunction(){
  //Serial.println("load cell");
    static boolean newDataReady = 0;
    const int serialPrintInterval = 100; //increase value to slow down serial print activity
    
    while(true){
     
      
      // check for new data/start next conversion:
      if (LoadCell.update()) newDataReady = true;
    
    
      //software shoould check if bottle is empty if it's empty sound an alarm for tech to switch bottle.
      
      // get smoothed value from the dataset:
      else if (newDataReady) { //loop for printing 
    
        if (millis() > t + serialPrintInterval) {
            //float time = millis();
            unsigned long now = millis()/1000; //made unsigned long to not have millis rollover 
            
            int Seconds = (now%60); // rounding for extraction
            //int OldSeconds;

            int Minutes = (now / 60)%60; // we cast float to int
            int Hours = (now /3600)%24;
            int Days = (now/(3600))/24;
            //String timeDisplay = (Days + "-" + Hours + ":" + Minutes + ":" + Seconds) ;
            //OldSeconds = Seconds;
            //priniting time
            float i = LoadCell.getData();

            Serial.println(String(Days) + "-" + String(Hours )+ ":" + String(Minutes) + ":" + String(Seconds) + "," + String(i));
    /*
            //lcd.lcdClear();
            float i = LoadCell.getData();
            //Serial.print("Load_cell output val: ");
            Serial.print(
            Serial.println(i);
           */
         
            newDataReady = 0;
            t = millis();

            if (i >= midWeight && i < 70){
              analogWrite(red_light,0); // ensure red led is off when entering this loop
              analogWrite(yellow_light,0); //ensure yellow led is off when entering this loop
              analogWrite(green_light,255);
              //Serial.println("Green Status");
              Serial.println("green");
              // set green light on for optimal weight
                         
              }
            else if ((i >= minWeight) && (i <= midWeight)  ){
              analogWrite(green_light,0); //turn off green light
              analogWrite(yellow_light,255);
              analogWrite(red_light,0);
              Serial.println("Yellow status");
              // set yellow light on for warning weight
              }
            else if (i <= minWeight && i > 1) {
              analogWrite(green_light,0);
              analogWrite(yellow_light,0);
              analogWrite(red_light,255);
              Serial.println("red");
              // Low chemical alarm
              
              //delay(1000); //replace with millis
              /*
              analogWrite(red_light,0);
              Serial.println("Red Status");
              delay(1000); //replace with millis
              */

              
              }
            else if (i >= 0 && i < 10 || i <= 0  ){ 
              
              // scale is tared with chemical on or no chemical on alarm 
              analogWrite(red_light,0);
              analogWrite(green_light,0);
              analogWrite(yellow_light,255);
              delay(1000); //delay for flashing light
              
              analogWrite(yellow_light,0);
              delay(1000);
              Serial.println("Tare Error Status");
              
              }
            else if (i >= 70 ){ 
              
              // scale is greater than max weight alarm
              analogWrite(yellow_light,0);
              analogWrite(green_light,0);
              analogWrite(red_light,255); //turn red light on 
              delay(1000); 
              
              //analogWrite(yellow_light,0);
              analogWrite(red_light,0); //turn red light off
              delay(1000);
              Serial.println("over weight Error Status");
              
              }
            
            else {
              // error detection
              // will trigger if values fall outside range
              analogWrite(green_light,255); //turn off green light
              analogWrite(yellow_light,255);
              analogWrite(red_light,255); //turn off green light
              delay(1000); // replace with millis
              analogWrite(green_light,0);
              analogWrite(yellow_light,0);
              analogWrite(red_light,0);
              delay(1000);
              Serial.println("Error Status");
              }   
            }
        }
      }
}
