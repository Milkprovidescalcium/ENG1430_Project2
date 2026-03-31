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

int whiteEggCount = 0;
int coloredEggCount= 0;
int brownEggCount = 0;

void loop(){



  uint16_t r;
  uint16_t g;
  uint16_t b;
  uint16_t c;

  tcs.getRawData(&r, &g, &b, &c);



  //egg checking logic--------------------------------
  if (c < 500) { //pparently this checks if an egg is there (light level I think)
   // Serial.println("no eggs detected");

    eggColor = 'b'; 
  } 
  else{


    Serial.println();
    Serial.println(">>> NEW EGG DETECTED <<<");

    printRGB(r,g,b,c);


    if (r > 1.4 * b && g > 1.2 * b) { //check brown
      Serial.println("Brown egg detected");

      eggColor = 'B';
      brownEggCount++;
    }
    else if (c > 2000 && abs(r - b) < 400) { //check white
      Serial.println("White egg detected");

      eggColor = 'W';
      whiteEggCount++;
    }
    else {  //not black, not brown, not white
      Serial.println("GETTTT OUT COLORED EGG");

      eggColor = 'c'; 
      coloredEggCount++;
    }

    moveMotors(eggColor);
    printEggCount();

  }

}

void printRGB(uint16_t r, uint16_t g, uint16_t b, uint16_t c){
  

  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("c: "); Serial.print(c);
  Serial.println();
  delay(500);
}

//moves the motors accordingly
void moveMotors(char eggColor){




  switch (eggColor){

    case 'B':



      myMotor->step(25, BACKWARD, DOUBLE);
      delay(300);

      kickEgg();

      myMotor->step(25, FORWARD, DOUBLE);
      delay(2000);

    break;

    case 'W':


      myMotor->step(25, FORWARD, DOUBLE);
      delay(300);

      kickEgg();

      myMotor->step(25, BACKWARD, DOUBLE);
      delay(2000);



    break;

    case 'b':

    break;

    default:


      myMotor->step(50, FORWARD, DOUBLE);
      delay(600);

      kickEgg();

      myMotor->step(50, BACKWARD, DOUBLE);
      delay(2000);



  }


      


}

//turns the servo that kicks the egg
void kickEgg(){

 // Serial.print("kick egg");

  servo.attach(servoPin); //only use servo when needed 
  delay(100);
  servo.write(180); //the servo is not accurate but this is roughly 180 degrees UI think
  delay(500);
  servo.write(90);
  delay(100);
  servo.detach();

}

void printEggCount(){ 

  Serial.println("---------------");
  Serial.print("White egg count: ");
  Serial.println(whiteEggCount);

  Serial.print("Brown egg count: ");
  Serial.println(brownEggCount);

  Serial.print("Colored egg count: ");
  Serial.println(coloredEggCount);
  Serial.println("---------------");



  if(whiteEggCount > 2){
    Serial.println("!!omg!! White egg bin full");
  }

  if(brownEggCount > 2){
    Serial.println("Dude... Brown egg bin full");
  }

  if(coloredEggCount > 2){
    Serial.println("Colored egg bin full");
  }


}







