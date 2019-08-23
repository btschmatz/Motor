

 
 
 
int brn=6;          // Rotary Encoder Inputs
int yel=7;          // Rotary encoder inputs
int det_pg = 11;    // detector photogate pin
int act_pg = 12;    // activation photogate pin

int arm_position = 0;    // initialize arm_position to 0 for encoder
int current_yel;    // current state of yellow encoder pin
int previous_yel;   // previous state of yellow encoder pin


void pos_func(){
  current_yel = digitalRead(yel);                 // Read the current state of brn
  
  if (current_yel != previous_yel){               // If the previous and the current state of the yel are different then a pulse has occured
    arm_position ++;                                
    Serial.println(arm_position);
  } 

  if (digitalRead(act_pg)==LOW){                  // if activation gate goes low, set arm_position to 0
    arm_position=0;   
  }
  else if (digitalRead(det_pg)==LOW){             // if detector gate goes low set arm position to 0
    arm_position=0;
  }
  
  previous_yel = current_yel;                     // Update previous_yel with the current state
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() { 
   
  pinMode (brn,INPUT);         // Set encoder pins as inputs
  pinMode (yel,INPUT);
  pinMode (det_pg, INPUT);     // Set photogate pins as inputs
  pinMode (act_pg, INPUT);
   
  Serial.begin (9600);
   
  previous_yel = digitalRead(yel);       // Read the initial state of yel. Assign to previous_yel variable

} 

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop() {

  pos_func();

}
