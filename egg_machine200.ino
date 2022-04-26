// include the library code:
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 4, 13, 3, 2, 12);
 



// defines variables

//ARM STATUS
bool arm;



//DISTANCE SENSOR
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
#define echoPin 5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D3 Arduino to pin Trig of HC-SR04
static unsigned long LastTime;
int strength=0;

//STEPPER MOTOR
const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

unsigned long saveMill = millis();
int prevStrength;
bool saved = false;

 void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
//  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
//  Serial.println("with Arduino UNO R3");
 
  // set up the LCD's number of columns and rows:
  lcd.begin(1, 1);

   // set the speed at 60 rpm:
  myStepper.setSpeed(15);

  LOAD();
}
void LOAD(){
  int temp = 0;
  EEPROM.get(0,temp);
  if(temp == 7)
    EEPROM.get(10,strength);
   else
    SAVE(); 

    prevStrength = strength;
}
void SAVE(){
    EEPROM.put(10,strength);
    int temp = 7;
    EEPROM.put(0,temp);
}

 void loop() { 
CHECK_DISTANCE();

if (distance < 10)
  {
    if (arm = true)
    {Serial.println(" FIRST");
      Serial.println(arm);
     
    if (millis() - LastTime < 5000)
      {
      // if close for  less than 5 seconds do this
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
lcd.clear();
lcd.print ("EGG DETECTED");
Serial.println("EGG DETECTED");
  
       }
    else
    {
      // print EGG if close for more than 5 seconds
 //    Serial.println("EGG");
    
lcd.clear();
lcd.print ("COOKING");
Serial.println("EGG DETECTED - LOWERING THE STEPPER");
arm = false;


//MOVE ACTUATOR DOWN
// step one revolution  in one direction:
  Serial.println("lowering the lid");
  myStepper.step(stepsPerRevolution);


  //show starting pump on screen
  lcd.clear();
  lcd.print ("starting pump");
   Serial.println("starting pump");
  
  delay(1000);

//INSERT CODE TO START PUMP depending on strength


//INSERT CODE TO START RELAY

 lcd.clear();
  lcd.print ("starting relay");
   Serial.println("starting relay");

   delay(1000);
//INSERT CODE TO SHOW COUNTDOWN TO FINISH AND EGG PICTURE2
//INSERT CODE TO STOP RELAY when sound is played (or when circuit breaks in cooker)





//INSERT CODE TO MOVE ACTUATOR UP
// step one revolution  in one direction:
  Serial.println("raising the lid");

   //show egg is ready on screen
  lcd.clear();
  lcd.print ("egg is ready");

  
  myStepper.step(-stepsPerRevolution);
  
 // lcd.setCursor(0, 1);
 // lcd.print ("bon appetit");






  //wait for distance to read high more than 5s
  
  
Serial.println("waiting removal");

Serial.println(arm);
Serial.println(" SECOND");


  while(true){ // after raising the lid get into this loop and wait
   CHECK_DISTANCE();  // keep cheking distance
   lcd.clear();
  lcd.print ("bon appetit");

   if(distance > 10){ // and if it gets  higher than 10 cm 
        delay(2000); // wait for 2 sec
        break;  // come out of the while loop
   }

   JOYSTICK_HANDLER();
  }


    }  } else{ 

Serial.println(arm);
    
    }
//RESET BACK TO STEP1 AND SHOW EGG PICTURE1


  
  }
  
  
  
  //IF DISTANCE IS MORE THAN 10 CM THEN PRINT NO EGG (FAR) ALSO SHOW %HARDBOILED
  
  else
  {
    //print No Egg if not close
  Serial.println("No Egg ");

 
  Serial.println("ARMING");
   arm = true;



   
 // CHECK JOYSTICK VALUES
  
JOYSTICK_HANDLER();



    LastTime = millis();
  }
 
 }
void JOYSTICK_HANDLER(){
    int joyx=analogRead(A1);

if(joyx==511){
 //strength;
} 
else if(joyx < 400) {
  --strength;
  saveMill = millis();
  saved = true;
} 
else if(joyx > 600) {
  ++strength;
  saveMill = millis();
  saved  = true;
}
if(prevStrength != strength){
 prevStrength = strength;
}
else{
 if(millis() - saveMill > 5000 && saved){
   SAVE();
   saved = false;
 }
}
delay(50);
// SHOW % ON SCREEN

lcd.setCursor (0,1);
lcd.print(constrain(strength,0,100));
lcd.print ("% hardboiled");

}
void CHECK_DISTANCE(){
 //CHECK DISTANCE
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = (duration-10) * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  
}
 
