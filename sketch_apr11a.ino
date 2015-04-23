
//Pins
int switchPin1 = 2;
int switchPin2 = 4;
int switchPin3 = 7;

int buttonPin = 8;

int greenLedPin = 12;
int redLedPin = 13;

int engineForwardPin = 6;
int engineBackwardsPin = 5;

int turnLeftPin = 11;
int turnRightPin = 10;

//Always false at start, sets to true if serial output is enabled.
boolean serialRun = false;

boolean lastActive[2];

void setup() {
  
 //All the pins being set up
 pinMode(switchPin1, OUTPUT);
 pinMode(switchPin2, OUTPUT);
 pinMode(switchPin3, OUTPUT);
 
 pinMode(buttonPin, INPUT);
 
 pinMode(redLedPin, OUTPUT);
 pinMode(greenLedPin, OUTPUT);
 
 pinMode(engineForwardPin, OUTPUT);
 digitalWrite(engineForwardPin, LOW);
 pinMode(engineBackwardsPin, OUTPUT);
 digitalWrite(engineBackwardsPin, LOW);
 
 pinMode(turnLeftPin, OUTPUT);
 digitalWrite(turnLeftPin, LOW);
 pinMode(turnRightPin, OUTPUT);
 digitalWrite(turnRightPin, LOW);
 
 //Doing the start red LED, and wait for button
 digitalWrite(greenLedPin, HIGH);
 digitalWrite(redLedPin, LOW);
 
 boolean buttonPress = true;
 while(buttonPress) {
   if(digitalRead(buttonPin) == HIGH) {
     buttonPress = false;
     delay(2000);
     if(digitalRead(buttonPin) == HIGH) {
       digitalWrite(redLedPin, HIGH);
       digitalWrite(greenLedPin, LOW);
       delay(2000);
       loop2();
     }
   }
 }
 //When button pressed, turns on green LED
 digitalWrite(redLedPin, HIGH);
 digitalWrite(greenLedPin, LOW);
}

void loop() {
  //Initializing the output string for the serial output 
  boolean input[2];
  //For loop that changes sensor and reads from the sensor that has the LED turned on.
  for(int i = 0; i < 2; i++){
    //Changes sensor
    changeSensor(i);
    //The sensors need a delay before reading, because the LEDs are a bit slow to get turned on. Thus this here delay beloweth!
    delay(1);
    //Getting the reading from the analog port
    int reading = analogRead(A4 - i);
    
    if(reading > 85) {
      input[i] = true;
    } else {
      input[i] = false;
    }
    delay(1);
  }
  //Printing the input. Outcomment the next method call to diable all serial activity!
  //printToSerial(input);
  
  followLine(input);
}

void loop2(){
  while(true) {
    turn(true);
    digitalWrite(engineForwardPin, HIGH);
    delay(5000);    
  }
}

void followLine(boolean input[]) {
  if(input[0]) {
    if(input[1]){
      lastActive[0] = true;
      lastActive[1] = true;
      driveStraight();
      changeDirection(true);
    } else {
      lastActive[0] = true;
      lastActive[1] = false;
      driveStraight();
      changeDirection(true);
    }
  } else if(input[1]) {
      lastActive[0] = false;
      lastActive[1] = true;   
      driveStraight(); 
      changeDirection(true);
  } else {
    if(lastActive[0]) {
      if(lastActive[1]){
        driveStraight();
        changeDirection(false);
      } else {
        turn(true);
      }
    } else if(lastActive[1]) {
        turn(false);
    }
  }
}



//Method that uses a switch to change which sensor has its LED turned on
void changeSensor(int sensNum) {
  digitalWrite(switchPin1, LOW);
  digitalWrite(switchPin2, LOW);
  digitalWrite(switchPin3, LOW);
  switch(sensNum) {
  case 1:
    digitalWrite(switchPin1, HIGH);
    break;
  case 2:
    digitalWrite(switchPin2, HIGH);
    break;
  case 3:
    digitalWrite(switchPin1, HIGH);
    digitalWrite(switchPin2, HIGH);
    break;
  case 4:
    digitalWrite(switchPin3, HIGH);
    break;
  default:
    break;
  }
}

void turn(boolean dir) {
  digitalWrite(turnLeftPin, LOW);
  digitalWrite(turnRightPin, LOW); 
  
  delay(3);
  
  if(dir) {
    digitalWrite(turnLeftPin, HIGH);
  } else {
    digitalWrite(turnRightPin, HIGH);
  }
}

void driveStraight() {
  digitalWrite(turnLeftPin, LOW);
  digitalWrite(turnRightPin, LOW);
}

void changeDirection(boolean dir) {
  digitalWrite(engineForwardPin, LOW);
  digitalWrite(engineBackwardsPin, LOW);
  
  if(dir) {
    analogWrite(engineForwardPin, 140);
  } else {
    analogWrite(engineBackwardsPin, 120);
  }
}

void fullBreak() {
  digitalWrite(engineForwardPin, LOW);
  digitalWrite(engineBackwardsPin, LOW);
  digitalWrite(turnLeftPin, LOW);
  digitalWrite(turnRightPin, LOW); 
  delay(100);
}


void printToSerial(boolean input[]) {
  if(!serialRun) {
    //initializing the serial output
    Serial.begin(9600);
    serialRun = true;
  }
  
  String output = "";
  for(int i = 0; i < 2; i++){
    if (input[i]) {
      output += "|";
    } else {
      output += "_";
    }
  }
  Serial.println(output);
}
