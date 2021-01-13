#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
 
const int TRIGGER_PIN_1 = 5;
const int ECHO_PIN_1 = 6;
const int MAX_DISTANCE_1 = 400; //400
const int TRIGGER_DISTANCE_1 = 300;//300

const int TRIGGER_PIN_2 = 9; 
const int ECHO_PIN_2 = 8;
const int MAX_DISTANCE_2 = 400; //400
const int TRIGGER_DISTANCE_2 = 250;//300

const byte SERVO_PIN = 3;
const byte RED_PIN_1 = 13;
const byte YELLOW_PIN_1 = 12;
const byte GREEN_PIN_1 = 11;
const byte ARROW_PIN_1 = 10;
const byte RED_PIN_2 = A0;
const byte YELLOW_PIN_2 = A1;
const byte GREEN_PIN_2 = A2;
const byte ARROW_PIN_2 = A3;

boolean isGoing = false;

unsigned long counter = 0;
int led = 0;

int lightChangeCounter = 0;
const int STOP_CHANGE_THRESHOLD = 1; //2
const int GO_CHANGE_THRESHOLD = 10; //30

Servo myServo;

const int SERVO_UP_DEGREE = 40; //0
const int SERVO_DOWN_DEGREE = 180; //123
const int SERVO_MOVE_DELAY = 5;

// NewPing setup of pins and maximum distance
NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE_1); 
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE_2); 

void setup() {
  lcd.begin();
  lcd.backlight();
  
  Serial.begin(9600);

  myServo.attach(SERVO_PIN, 1000, 2000);

  pinMode(RED_PIN_1, OUTPUT);
  pinMode(YELLOW_PIN_1, OUTPUT);
  pinMode(GREEN_PIN_1, OUTPUT);
  pinMode(ARROW_PIN_1, OUTPUT);
  pinMode(RED_PIN_2, OUTPUT);
  pinMode(YELLOW_PIN_2, OUTPUT);
  pinMode(GREEN_PIN_2, OUTPUT);
  pinMode(ARROW_PIN_2, OUTPUT);

  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  goLight();
}



void loop() {
  //test();
  //return;

  //calculate distance
  int objectDistance1 = calculateDistance(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE_1);
  int objectDistance2 = calculateDistance(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE_2);

  //log
  Serial.print("Distance1: ");
  Serial.print(objectDistance1);
  Serial.print(", Distance2: ");
  Serial.println(objectDistance2);
  
  lcd.clear();
  lcd.print("D1: ");
  lcd.print(objectDistance1);
  lcd.print(", D2: ");
  lcd.print(objectDistance2);

 
  //operations
  if(isGoing){
    if(objectDistance1 <= TRIGGER_DISTANCE_1 || objectDistance2 <= TRIGGER_DISTANCE_2){
      lightChangeCounter++;

      lcd.setCursor(0,1);
      lcd.print(lightChangeCounter);
      
      if(lightChangeCounter == STOP_CHANGE_THRESHOLD){
        lightChangeCounter = 0;

        stopLight();
        
      }
    }else{
      lightChangeCounter = 0;
    }
  }else{
    if(objectDistance1 > TRIGGER_DISTANCE_1 && objectDistance2 > TRIGGER_DISTANCE_2){
      lightChangeCounter++;

      lcd.setCursor(0,1);
      lcd.print(lightChangeCounter);
      
      if(lightChangeCounter == GO_CHANGE_THRESHOLD){
        lightChangeCounter = 0;

        goLight();
        
      }
    }else{
      lightChangeCounter = 0;
    }
  }
}

int calculateDistance(int triggerPin, int echoPin, int maxDistance){ 
  // Clears the trigPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration*0.034/2;
  if(distance > maxDistance){
    distance = maxDistance;
  }

  return distance;
}

int calculateDistance2(NewPing sonar){ 
  //delay(50);
  unsigned int distance = sonar.ping_cm();
   

  return distance;
}

void stopLight(){
  lcd.clear();
  lcd.print("delay");
  
  isGoing = false;

  //yellow
  digitalWrite(RED_PIN_1, LOW);
  digitalWrite(YELLOW_PIN_1, HIGH);
  digitalWrite(GREEN_PIN_1, LOW);
  digitalWrite(ARROW_PIN_1, LOW);
  digitalWrite(RED_PIN_2, LOW);
  digitalWrite(YELLOW_PIN_2, HIGH);
  digitalWrite(GREEN_PIN_2, LOW);
  digitalWrite(ARROW_PIN_2, LOW);
  delay(500);

  //red
  digitalWrite(RED_PIN_1, HIGH);
  digitalWrite(YELLOW_PIN_1, LOW);
  digitalWrite(GREEN_PIN_1, LOW);
  digitalWrite(ARROW_PIN_1, HIGH);
  digitalWrite(RED_PIN_2, HIGH);
  digitalWrite(YELLOW_PIN_2, LOW);
  digitalWrite(GREEN_PIN_2, LOW);
  digitalWrite(ARROW_PIN_2, HIGH);

  //servo
  servoDown();

  //delay
  delay(5000);

  
}

void goLight(){
  lcd.clear();
  lcd.print("delay");
  
  isGoing = true;

  //green
  digitalWrite(RED_PIN_1, LOW);
  digitalWrite(YELLOW_PIN_1, LOW);
  digitalWrite(GREEN_PIN_1, HIGH);
  digitalWrite(ARROW_PIN_1, LOW);
  digitalWrite(RED_PIN_2, LOW);
  digitalWrite(YELLOW_PIN_2, LOW);
  digitalWrite(GREEN_PIN_2, HIGH);
  digitalWrite(ARROW_PIN_2, LOW);

  //servo
  servoUp();

  //delay
  delay(1000);
}

void servoUp(){
  for(int i = SERVO_DOWN_DEGREE; i > SERVO_UP_DEGREE; i--){
    myServo.write(i);
    delay(SERVO_MOVE_DELAY);
  }
}

void servoDown(){
  for(int i = SERVO_UP_DEGREE; i < SERVO_DOWN_DEGREE; i++){
    myServo.write(i);
    delay(SERVO_MOVE_DELAY);
  }
}

void test(){
  
  digitalWrite(RED_PIN_1, HIGH);
  digitalWrite(YELLOW_PIN_1, HIGH);
  digitalWrite(GREEN_PIN_1, HIGH);
  digitalWrite(ARROW_PIN_1, HIGH);
  digitalWrite(RED_PIN_2, HIGH);
  digitalWrite(YELLOW_PIN_2, HIGH);
  digitalWrite(GREEN_PIN_2, HIGH);
  digitalWrite(ARROW_PIN_2, HIGH);
  return;
  
  
  
  /*
  //
  int objectDistance1 = calculateDistance(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE_1);
  Serial.print("Distance1: ");
  Serial.println(objectDistance1);*/

  /*int objectDistance2 = calculateDistance(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE_2);
  Serial.print("Distance2: ");
  Serial.println(objectDistance2);
  return;
  */

  /*
  delay(2);
  
  //
  int objectDistance1 = calculateDistance(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE_1);
  int objectDistance2 = calculateDistance(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE_2);
  //int objectDistance1 = calculateDistance2(sonar1);
  //int objectDistance2 = calculateDistance2(sonar2);

  //log
  Serial.print("Distance1: ");
  Serial.print(objectDistance1);
  Serial.print(", Distance2: ");
  Serial.println(objectDistance2);
  
  lcd.clear();
  lcd.print("D1: ");
  lcd.print(objectDistance1);
  lcd.print(", D2: ");
  lcd.print(objectDistance2);

  if(objectDistance1 <= TRIGGER_DISTANCE_2){
    digitalWrite(RED_PIN_1, HIGH);
    digitalWrite(YELLOW_PIN_1, HIGH);
    digitalWrite(GREEN_PIN_1, LOW);
    digitalWrite(ARROW_PIN_1, LOW);
  }else{
    digitalWrite(RED_PIN_1, LOW);
    digitalWrite(YELLOW_PIN_1, HIGH);
    digitalWrite(GREEN_PIN_1, HIGH);
    digitalWrite(ARROW_PIN_1, LOW);
  }

  if(objectDistance2 <= TRIGGER_DISTANCE_2){
    digitalWrite(RED_PIN_2, HIGH);
    digitalWrite(YELLOW_PIN_2, HIGH);
    digitalWrite(GREEN_PIN_2, LOW);
    digitalWrite(ARROW_PIN_2, LOW);
  }else{
    digitalWrite(RED_PIN_2, LOW);
    digitalWrite(YELLOW_PIN_2, HIGH);
    digitalWrite(GREEN_PIN_2, HIGH);
    digitalWrite(ARROW_PIN_2, LOW);
  }
  
  return;
  */
  
  
  /*
  //
  myServo.write(40);
  delay(1000);
  myServo.write(180);
  delay(1000);
  return;
  */
  
  /*
  //
  stopLight();
  delay(5000);
  goLight();
  delay(5000);
  return;
  */

  
  /*
  //
  int distance = calculateDistance();
  if(distance <= TRIGGER_DISTANCE){
    digitalWrite(RED_PIN_1, HIGH);
    digitalWrite(YELLOW_PIN_1, HIGH);
    digitalWrite(GREEN_PIN_1, LOW);
    digitalWrite(ARROW_PIN_1, LOW);
  }else{
    digitalWrite(RED_PIN_1, LOW);
    digitalWrite(YELLOW_PIN_1, HIGH);
    digitalWrite(GREEN_PIN_1, HIGH);
    digitalWrite(ARROW_PIN_1, LOW);
  }
  return;
  */

   /*
  //arrow blink
  if(!isGoing){
    counter++;
    //Serial.println(counter);
    if(counter == 10){
      counter = 0;
  
      digitalWrite(ARROW_PIN_1, led);
      led = !led;
    }
  }*/
}
