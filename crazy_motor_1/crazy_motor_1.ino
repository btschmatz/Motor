//Global Variable Land
//EDIT

int pwm = 3;
int dir = 4;
int brn=6;          // Rotary Encoder Inputs
int yel=7;          // Rotary encoder inputs      Pin Declarations
int det_pg = 11;    // detector photogate pin
int act_pg = 12;    // activation photogate pin


int arm_position = 0;    // initialize arm_position to 0 for encoder
int current_yel;         // current state of yellow encoder pin            variable declarations for arm position
int previous_yel;        // previous state of yellow encoder pin


int pwm_val;
char user_input;           
float activation_stop;    // user defined parameter declarations for arm movement loops
float detector_stop;
int loopnumber;
int count_number;


//========================================================================
//Function Land


//Set Sample Function                                     // This is a "dumb" function. it does not read position it only waits
void set_sample(){
  while(digitalRead(act_pg)==HIGH){
    digitalWrite(dir,LOW);                                  // send arm CCW at a slow speed (pwm=30)
    analogWrite(pwm,30);
  }
  analogWrite(pwm,30);
  delay(100);
  digitalWrite(pwm,LOW);                                // stop motor running 
  Serial.println("Sample is at Activation Chamber");
  delay(500);
  //pwm LOW
  //dir LOW
}

//Sample Mount Function
void sample_mount(){
  while(digitalRead(act_pg)==HIGH){
    digitalWrite(dir,LOW);                        // This is a "dumb" function. it does not read position it only waits
    analogWrite(pwm,30);                          // send arm CCW at a slow speed (pwm=30)
  }
  analogWrite(pwm,30);
  delay(100);
  digitalWrite(pwm,LOW);
  delay(500);
  digitalWrite(dir,HIGH);                     // change arm direction to CW
  analogWrite(pwm,30);                        // move slowly
  delay(1900);                                // wait two seconds. at pwm=30 the motor should move to 90deg
  digitalWrite(pwm,LOW);                      // stop motor after running it for 2 seconds. arm should be near 90deg
  Serial.println("Sample Ready to Mount");
  //pwm LOW
  //dir HIGH
}
  


//Arm Position Function
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


//Set Parameter Function
void set_param(){
  Serial.println();                                          // blank line in serial monitor
  Serial.print("Input Activation Stop Period (sec) ==> ");   // user prompt for arm stop at activation chamber
  while (!Serial.available()){}                              // waiting for user input
  String as=Serial.readString();                             // reading string input from user (ex. 2.25)           
  activation_stop = as.toFloat()*1000;                       // casts as from string to float and converts from sec to millisec
  Serial.print(activation_stop/1000);                        // print activation stop time in seconds
  Serial.println(" seconds");                                // print                           
  
  
  Serial.print("Input Detector Stop Period (sec) ==> ");     // user prompt for arm stop at detector
  while (!Serial.available()){}                              // waiting for user input
  String ds = Serial.readString();                           // reading string input from user
  detector_stop = ds.toFloat()*1000;                         // casts ds from string to float and converts from sec to millisec
  Serial.print(detector_stop/1000);                          // print detector stop in seconds
  Serial.println(" seconds");                                // print

  Serial.print("Input Loop Number ==> ");     // user prompt for number of loops.... 1 loop= out then back from activation chamber
  while (!Serial.available()){}               // wait for user input
  String ln = Serial.readString();            // read in string for number of loops
  loopnumber = ln.toInt();                    // this is an integer. loopnumber must be an integer
  Serial.println(loopnumber);                 // print loopnumber
  Serial.println("");                         // blank space
}


//Kill Function
boolean check_kill(){                  // check kill function
  if(Serial.available()>0){
    char kill=Serial.read();
    if(kill=='k'){
       Serial.println();
       Serial.println("ROUTINE KILLED");
       Serial.println();
       return 1;
    }
       //return 0;
  }
  return 0;
}

// function that will eventually print = signs in the Progress Bar function
String r_string(int i,String s){
  String temp_string="";
  for (int k=0;k<i;++k){
    temp_string = temp_string + s;
  }
  return temp_string;
}

// Progress Bar Function
String prog(int bar_length,int iter,int total_iter ){
  String prog_string = "[";
  float percent=(float)(iter)/(float)(total_iter);        // calculating percentage complete
  int m = bar_length*percent;
  prog_string += r_string(m,"=");
  prog_string += ">";
  prog_string += r_string(bar_length - m, " ");
  prog_string += "]";
  prog_string += " ";
  prog_string += String (percent*100);
  prog_string += "%";

  return prog_string;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


void setup() {
  
  TCCR2B = TCCR2B & B11111000 | B00000001;      // changing pin 3 pwm frequency from 975Hz to 31372.55 Hz (makes motor quieter because signal freq. is supersonic
  pinMode(pwm, OUTPUT);                         // pin 3 is the pwm output (line 2)
  pinMode(dir, OUTPUT);                         // pin 4 is the direction control pin (line 3)
  pinMode(brn,INPUT);                           // Set encoder pins as inputs
  pinMode(yel,INPUT);
  pinMode(det_pg, INPUT);                       // Set photogate pins as inputs
  pinMode(act_pg, INPUT);
  
  Serial.begin(9600);                           // begin serial communnication with board at a 9600 bits/second rate

  Serial.println("*****Begin Control*****");                 // printing boot alert serial monitor//set_param();
  
  set_param();
  
  previous_yel = digitalRead(yel);       // Read the initial state of yel. Assign to previous_yel variable
  
  pwm_val=50;

}





// -----------------------------------------------------------------------------------------------------------------------------------------------------------------




void loop() {


  Serial.println("Ready to Begin");                           // print ready to begin motor run
  Serial.println("\t\t Enter 1 to Begin Routine");            // user promtpt enter true or false to start routine
  Serial.println("\t\t Enter k to KILL Routine");
  Serial.println("\t\t Enter r to reset parameters before running"); 
  Serial.println("\t\t Enter m to mount a sample");
  Serial.println("\t\t Enter s to send sample to Activation Area");
  Serial.println("");
  
  delay(500);                                                 // delay .5 sec
  
  while (!Serial.available()){}                               // wait for user to input true false to start routine
  
  user_input = Serial.read();                                 // read user input to start routine

  Serial.print("user_input=");
  Serial.println(user_input);
   
  if (user_input == 'r'){
    set_param();
    }
  else if (user_input == 'm'){
    sample_mount();  
  }
  else if (user_input == 's'){
    set_sample();
  }
  else if (user_input == '1'){                                                    // do the code below if 1 is the input    
    
    for (count_number = 1; count_number <= loopnumber ; ++count_number){                       
      
      //CONTROL THE MOTOR HERE IN THIS FOR LOOP
      if(check_kill())break;                      // check kill function looking for kill signal (k)   
      
      Serial.println(prog(30,count_number,loopnumber));
      if (count_number == loopnumber ){
        Serial.println("Routine Complete");
        Serial.println();
      }
    }
  }  
}            
