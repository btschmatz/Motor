#include <Encoder.h>
int det_pg = 11;    // detector photogate pin
int act_pg = 12;    // activation photogate pin
long positionLeft  = 0;
long newLeft;

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

Encoder knobLeft(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("TwoKnobs Encoder Test:");
}

void pos_func(){
   newLeft = abs(knobLeft.read());
  if (newLeft != positionLeft) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.println();
    positionLeft = newLeft;
  }
  if(digitalRead(act_pg)==LOW){
    knobLeft.write(0);
  }
  if(digitalRead(det_pg)==LOW){
    knobLeft.write(0);
  }
}

void loop() {

 pos_func();

 delay(1000);
  
 
 
}
