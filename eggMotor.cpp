#include <Servo.h>

#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include <Adafruit_MotorShield.h>


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Servo servo;
int servoPin = 9; 


Adafruit_MotorShield stepper = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = stepper.getStepper(100, 1);

void setup(){
  Serial.begin(9600);

  //initialize motors and sensors
  stepper.begin(); 
  myMotor->setSpeed(10);



  // if (tcs.begin()) {
  //   Serial.println("Sensor Found!");
  // } else {
  //   Serial.println("No sensor found. Check SDA/SCL wiring.");
  //   while (1); // Stop everything if sensor isn't working
  // }


}



char eggColor; //parameter for the motor moving function

void loop(){



  uint16_t r;
  uint16_t g;
  uint16_t b;
  uint16_t c;


  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("c: "); Serial.print(c);
  Serial.println();
  delay(500);


  //egg checking logic--------------------------------
  if (c < 500) { //pparently this checks if an egg is there (light level I think)
    eggColor = 'b'; 
  } 
  else if (r > 1.4 * b && g > 1.2 * b) { //check brown
    eggColor = 'B';
  }
  else if (c > 3000 && abs(r - b) < 400) { //check white
    eggColor = 'W';
  }
  else {  //not black, not brown, not white
    eggColor = 'c'; 
  }

  if (eggColor != 'b') { //only move if egg is there
    moveMotors(eggColor);
  }
       
}

//moves the motors accordingly
void moveMotors(char eggColor){


  switch (eggColor){

    case 'B':
      Serial.print("Brown egg detected");



      myMotor->step(25, BACKWARD, DOUBLE);
      delay(300);

      kickEgg();

      myMotor->step(25, FORWARD, DOUBLE);
      delay(2000);

    break;

    case 'W':
      Serial.print("White egg detected");


      myMotor->step(25, FORWARD, DOUBLE);
      delay(300);

      kickEgg();

      myMotor->step(25, BACKWARD, DOUBLE);
      delay(2000);



    break;

    case 'b':
      Serial.print("no eggs detected");

    break;

    default:
      Serial.print("GETTTT OUT COLORED EGG");


      myMotor->step(50, FORWARD, DOUBLE);
      delay(600);

      kickEgg();

      myMotor->step(50, BACKWARD, DOUBLE);
      delay(2000);



  }
      


}

//turns the servo that kicks the egg
void kickEgg(){

  servo.attach(servoPin); //only use servo when needed 
  delay(100);
  servo.write(120);
  delay(500);
  servo.write(90);
  delay(100);
  servo.detach();

}







