//luca cobeaga, brushless and solenoid flywheel blaster SHADOW PRICE
//last updated 12/2/18
//for the JC_Button.h library go to https://github.com/JChristensen/JC_Button and download to your libraries in arduino IDE
//lucaslegos@gmail.com for questions
#include <JC_Button.h>   //include code libraries for a button with debounce, effectively its like a cited source, and below im writing the evience i gained from reading it
#include <Servo.h>       //include code libraries for servos, this is PWM and is how we are contrlling the brushless motors

#define TRIGGER_PIN 9    //sets trigger limit switch to pin 9
#define MOTOR_PIN 10     //sets ESC control to pin 10
#define ACTUATOR_PIN 11  //sets solenoid pusher pin to 11

Servo flywheelESC;                              // create servo object to control the esc 
Button trigger(TRIGGER_PIN, 25, true, true);    //creates trigger button

void setup() {
  flywheelESC.attach(MOTOR_PIN);     //attaches the servo on pin 10 to the servo object
  pinMode(ACTUATOR_PIN,OUTPUT);      //tells the chip to treat the mosfet controlling the solenoid as an outbound signal, as in it doesnt return a signal

  flywheelESC.writeMicroseconds(1864);  //runs the ESC startup sequence by giving highest possible speed value
  delay(2000);                          //waits, this is when you hear the three beeps, last two sound the same
  flywheelESC.writeMicroseconds(1064);  //gives lowest possible speed value
  delay(2000);                          //waits to make sure esc took code, here you hear two more beeps, last one very high pitched

  trigger.begin();          //initializes button and connected pin
}                                       

void loop() {
  trigger.read();

  //semiAutoFire();
  pulseFire();
}
/*
void semiAutoFire () {

  if(trigger.wasPressed()){               //checks to see if trigger is pressed 
    flywheelESC.writeMicroseconds(1840);  //gives a maximum speed value when trigger is pressed
    delay(30);
    digitalWrite(ACTUATOR_PIN,HIGH);      //gives a signal to MOSFET to allow current flow (MOSFETs are like normally closed switches you open with positive current)
    delay(25);                            //gives signal for 30 milliseconds since solenoids work on change in current, not just curre
    digitalWrite(ACTUATOR_PIN,LOW);       //closes current flow to solenoid
  } 

  else{                                  //if trigger not depressed
    flywheelESC.writeMicroseconds(1140); //gives the lowest possible value, or a speed of 0 when no trigger is pressed
    digitalWrite(ACTUATOR_PIN,LOW);      //also turns off solenoid, can never be too safe (solenoid heats up fast)
  }
}
*/
void pulseFire() {

  if(trigger.wasPressed()){                //checks to see if trigger is pressed 
    flywheelESC.writeMicroseconds(1840);  //gives a maximum speed value when trigger is pressed
    delay(10);

    for(int i = 0; i < 3; i++){
      digitalWrite(ACTUATOR_PIN,HIGH);      //gives a signal to MOSFET to allow current flow (MOSFETs are like normally closed switches you open with positive current)
      delay(30);                            //gives signal for 30 milliseconds since solenoids work on change in current, not just curre
      digitalWrite(ACTUATOR_PIN,LOW);       //closes current flow to solenoid
      delay(100);
    }
  } 

  else{                                  //if trigger not depressed
    flywheelESC.writeMicroseconds(1140); //
    digitalWrite(ACTUATOR_PIN,LOW);      //also turns off solenoid, can never be too safe (solenoid heats up fast)
  }

}
