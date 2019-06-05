
//luca cobeaga, brushless and solenoid flywheel blaster SHADOW PRICE
//last updated 06/05/19
//for the JC_Button.h library go to https://github.com/JChristensen/JC_Button and download to your libraries in arduino IDE
//lucacobeaga@gmail.com for questions

#include <JC_Button.h>   //include code libraries for a button with debounce, effectively its like a cited source, and below im writing the evience i gained from reading it
#include <Servo.h>       //include code libraries for servos, this is PWM and is how we are contrlling the brushless motors

#define TRIGGER_PIN 9      //sets trigger limit switch to pin 9
#define MOTOR_PIN 10       //sets ESC control to pin 10
#define ACTUATOR_PIN 11    //sets solenoid pusher pin to 11
#define POT_PIN 1          //sets poteniometer pin to 1
#define FIRE_MODE_PIN 7    //sets on off switch pin to 7

Servo flywheelESC;                              //create servo object to control the esc 
Button trigger(TRIGGER_PIN, 25, true, true);    //creates trigger button, read button library for syntax
Button selectFire(FIRE_MODE_PIN, 25, true, true);

void setup() {
  flywheelESC.attach(MOTOR_PIN);     //attaches the servo on pin 10 to the servo object
  pinMode(ACTUATOR_PIN,OUTPUT);      //tells the chip to treat the mosfet controlling the solenoid as an outbound signal, as in it doesnt return a signal

  delay(2000);
  flywheelESC.writeMicroseconds(1864);  //runs the ESC startup sequence by giving highest possible speed value
  delay(2000);                          //waits, this is when you hear the three beeps, last two sound the same
  flywheelESC.writeMicroseconds(1064);  //gives lowest possible speed value
  delay(2000);                          //waits to make sure esc took code, here you hear two more beeps, last one very high pitched

  trigger.begin();          //initializes button and connected pin
  selectFire.begin();       //initializes slide switch and connected pin
}                                       

void loop() {
  trigger.read();     //only time the trigger is actually checked
  selectFire.read();
  
  if(selectFire.isPressed()){
    //pulseFire();
    AutoFire();
  }

  AutoFire();
  //pulseFire();

}


void AutoFire() {

  if(trigger.wasPressed()){               //if trigger was previously pressed 

    revSequence();

    while(trigger.isPressed()){
      digitalWrite(ACTUATOR_PIN,HIGH);      //gives a signal to MOSFET to allow current flow (N-channel MOSFETs are like normally closed switches you open with positive current)
      delay(30);                            //wait for solenoid to move out far enough
      digitalWrite(ACTUATOR_PIN,LOW);       //close current flow to solenoid
      delay(70);                           //wait for wheels to get back to speed
      trigger.read();
    } 
  } 

  flywheelESC.writeMicroseconds(1060); //gives lowest tested value where motors move so trigger can be more responsive, but draws more current so you need a big battery
  digitalWrite(ACTUATOR_PIN,LOW);      //also turns off solenoid, can never be too safe (solenoid heats up fast)
}


void pulseFire() {
  if(trigger.wasPressed()){     
    
    revSequence();

    for(int i = 0; i < 3; i++){         //cycle pusher 3 times, if longer or shorter pulse needed change the 3 value
      digitalWrite(ACTUATOR_PIN,HIGH);
      delay(70);
      digitalWrite(ACTUATOR_PIN,LOW);
      delay(90);
    }
  } 

  flywheelESC.writeMicroseconds(1060); 
  digitalWrite(ACTUATOR_PIN,LOW);
}


void revSequence(){
  flywheelESC.writeMicroseconds(1860);                //max speed to get best acceleration
  delay(map(analogRead(POT_PIN), 0, 1023, 150, 50));    //delay inverse to speed to accelerate to below speed
  flywheelESC.writeMicroseconds(map(map(analogRead(POT_PIN), 0, 1023, 0, 100), 0, 100, 1250, 1650));
  delay(70);
}
