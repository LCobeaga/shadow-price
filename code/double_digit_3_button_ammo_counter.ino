/*
Luca Cobeaga (u/Band3rsnach) 

Ammo Counter Code


last updated 06/23/19


as this is open source I cant stop anyone from taking credit for my code, but I ask that you please give me credit if you do use this 

i tried to add comments to all code because im bad at this stuff, so if you are confused look at what i wrote to clear up confusion, or direct message me 
with any questions you cant find the answer too



this code is simple because i am NOT multiplexing the two 7 segment displays, and im using decoders to make my 10 pins off the 7 segment into 4 
thus this ammo counter has some specific hardware limit:

1. you MUST be using either TWO SEPARATE 7 segment displays or a double digit display that has 18 TOTAL pins. the two digits are controlled SEPARATELY
2. you MUST be using decoders that match your 7 segment displays, they are categorized by either common anode or common cathode. 
3. this code is designed for common anode(common positive pin), if you have something different you just need to update the HIGH and LOW sets at the 
   bottom of this code to make numbers, and plug the common into Vcc for common anode or Gnd for common cathode


Definitions:

MAXmag - Maximum Magazine capacity, as in the largest magazine you own that will be shot with this ammo counter (mine is a 35 dart drum, so my MAXmag is 35)

BASEmag - BASE magazine size, as in the value that is displayed the moment the ammo counter is turned on (this doesn't really matter since you can change 
          this value at any moment and the code will remember the new value) 



oh and to note, this counter only supports up to 99 rounds in a magazine, if you want to go up to 999 (for like a proton pack or something) you need to add another digit
and another decoder, and add a digthree A through D
*/

//next 9 are names of the pinouts of the outbound signals (code applies voltages), so change based on where you plugged things in
//digits are from right to left, or digit one is the tens and digit two is the ones

#define DIG_ONE_A 2
#define DIG_ONE_B 3
#define DIG_ONE_C 4
#define DIG_ONE_D 5
#define DIG_TWO_A 6
#define DIG_TWO_B 7
#define DIG_TWO_C 8
#define DIG_TWO_D 9

//these are also named pins, but of inbound signals (code is checking for voltages), change based on where you plugged things in

#define IR_BEAM 1
#define MAG_SIZE_UP 10
#define MAG_SIZE_DOWN 11
#define MAG_SENSOR 12

//this variable is what we paste the IR beam results to, all it does is simplify writing, dont change 

int sensorState = 0;

//these are just variables I mentioned, change based on what I said above 

int BASEmag  = 17;
int MAXmag   = 35;
int CURRENTmag;

void setup() {

//this is just telling the code what each pin does, so all the variables above are called upon down here
//think like algebra 2 where you plug in the value for the variable
  
pinMode(DIG_ONE_A,OUTPUT);
pinMode(DIG_ONE_B,OUTPUT);
pinMode(DIG_ONE_C,OUTPUT);
pinMode(DIG_ONE_D,OUTPUT);
pinMode(DIG_TWO_A,OUTPUT);
pinMode(DIG_TWO_B,OUTPUT);
pinMode(DIG_TWO_C,OUTPUT);
pinMode(DIG_TWO_D,OUTPUT);
pinMode(MAG_SIZE_UP,INPUT_PULLUP);        //for all 3 im turning on an internal resistor so when it sees the voltage pass through it doesnt burn the chip
pinMode(MAG_SIZE_DOWN,INPUT_PULLUP);
pinMode(MAG_SENSOR,INPUT_PULLUP);
pinMode(IR_BEAM,INPUT);

digitalWrite(IR_BEAM, HIGH);    //turns on ir beam light
}

void loop() {
  sensorState = digitalRead(IR_BEAM);                //sets output of IR_BEAM to the variable sensorstate
  func(CURRENTmag);                                 //func is all the binary for the decoders saved and CURRENTmag is the CURRENT magazine size
  
  while(!digitalRead(MAG_SENSOR)==LOW){       //this keeps code within this loop until statement is true
    buttonValue();                                  //the function that allows the up and down buttons to change BASE magazine size
    func(BASEmag);                                  //displays the BASE magazine size 
    CURRENTmag=BASEmag;                             //sets the CURRENT magazine size to the BASE magazine size
  }
  
  if(digitalRead(MAG_SENSOR)==LOW&&sensorState==LOW){    //checks if the magazine is inserted AND if a dart passed the sensor
  CURRENTmag--;                                                //makes the CURRENT magazine size decrease by 1
  delay(100);                                                  //debounce, if this wasnt here some darts could actaully set off the breakbeam twice 
  }
}

void buttonValue(){                    //the function that allows the up and down buttons to change the BASE magazine size
  if(digitalRead(MAG_SIZE_UP)==LOW){    //looks to see if you are pressing the up button
    delay(100);                        //debounce
    if (BASEmag<MAXmag){               //checks to see if the BASE magazine size is less than 35, if you have larger magazine size you change this and add code below
    BASEmag = ++BASEmag;               //increases BASE magazine size by 1
    }
    else {                             //pretty much just cycles back around so if a button breaks you can still change your BASE magazine size
      BASEmag = 0;                     //if you reach a value greater than maximum magazine size it cycles back to 0
    }
  }
  if(digitalRead(MAG_SIZE_DOWN)==LOW){   //looks to see if you are pressing the down button
    delay(100);                         //debugger
    if(BASEmag>0){                      //looks to see if BASE magazine size is greater than 0
      BASEmag = --BASEmag;              //decrease BASE magazine size by 1
    }
    else{                               //pretty much just cycles back around so if a button breaks you can still change your BASE magazine size
      BASEmag = MAXmag;                 //if you reach a value lower than 0 you cycle back to maximum magazine size
    }
  }
}
void func(int CURRENTmag){
/*
 
the next few values are for the FIRST digit of the 7 segment, the 10s position 
it works by an else if statement, where the first statement, in order, found to be true is run, and all others are ignored

 */
  if(CURRENTmag<10){
 //zero
  digitalWrite(DIG_ONE_A,LOW);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<20){
 //teens
  digitalWrite(DIG_ONE_A,HIGH);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<30){
 //twenty
  digitalWrite(DIG_ONE_A,LOW);
  digitalWrite(DIG_ONE_B,HIGH);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<40){
 //thirty
  digitalWrite(DIG_ONE_A,HIGH);
  digitalWrite(DIG_ONE_B,HIGH);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<50){
 //fourty
  digitalWrite(DIG_ONE_A,LOW);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,HIGH);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<60){
 //fifty
  digitalWrite(DIG_ONE_A,HIGH);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,HIGH);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<70){
 //sixty
  digitalWrite(DIG_ONE_A,LOW);
  digitalWrite(DIG_ONE_B,HIGH);
  digitalWrite(DIG_ONE_C,HIGH);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<80){
 //seventy
  digitalWrite(DIG_ONE_A,HIGH);
  digitalWrite(DIG_ONE_B,HIGH);
  digitalWrite(DIG_ONE_C,HIGH);
  digitalWrite(DIG_ONE_D,LOW);
  }
  else if(CURRENTmag<90){
 //eighty
  digitalWrite(DIG_ONE_A,LOW);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,HIGH);
  }
  else if(CURRENTmag<100){
 //ninety
  digitalWrite(DIG_ONE_A,HIGH);
  digitalWrite(DIG_ONE_B,LOW);
  digitalWrite(DIG_ONE_C,LOW);
  digitalWrite(DIG_ONE_D,HIGH);
  }
  
/* 
   
the values below are for the SECOND digit of the 7 segment display, it works by using a modulus (the percentage symbol%)
a modulus pretty much divides the first value by the second, and returns the REMAINDER of the division, so 1 % 10 returns 1, 13 % 10 returns 3 and so on

*/
  
  if(CURRENTmag % 10 == 0){
 //zero
  digitalWrite(DIG_TWO_A,LOW);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,LOW);
  }
    if(CURRENTmag % 10 == 1){
  //one
  digitalWrite(DIG_TWO_A,HIGH);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 2){
  //two
  digitalWrite(DIG_TWO_A,LOW);
  digitalWrite(DIG_TWO_B,HIGH);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 3){
  //three
 digitalWrite(DIG_TWO_A,HIGH);
  digitalWrite(DIG_TWO_B,HIGH);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 4){
  //four
  digitalWrite(DIG_TWO_A,LOW);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,HIGH);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 5){
  //five
  digitalWrite(DIG_TWO_A,HIGH);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,HIGH);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 6){
  //six
  digitalWrite(DIG_TWO_A,LOW);
  digitalWrite(DIG_TWO_B,HIGH);
  digitalWrite(DIG_TWO_C,HIGH);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 7){
  //seven
  digitalWrite(DIG_TWO_A,HIGH);
  digitalWrite(DIG_TWO_B,HIGH);
  digitalWrite(DIG_TWO_C,HIGH);
  digitalWrite(DIG_TWO_D,LOW);
    }
    if(CURRENTmag % 10 == 8){
  //eight
  digitalWrite(DIG_TWO_A,LOW);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,HIGH);
    }
    if(CURRENTmag % 10 == 9){
  //nine
  digitalWrite(DIG_TWO_A,HIGH);
  digitalWrite(DIG_TWO_B,LOW);
  digitalWrite(DIG_TWO_C,LOW);
  digitalWrite(DIG_TWO_D,HIGH);
    }
}
