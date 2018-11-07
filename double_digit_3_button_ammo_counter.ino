/*
Luca Cobeaga's (u/Band3rsnach) 

Stupidly Simple Open Source Ammo Counter Code


last updated 11/6/18


last edited by: 


feel free to edit code based on your hardware needs and magazine choices! and add your name to the edited by list
 
as this is open source I cant stop anyone from taking credit for my code, but I ask that you please give me credit if you do use this 

i tried to add comments to all code because im bad at this stuff, so if you are confused look at what i wrote to clear up confusion, or direct message me 
with any questions you cant find the answer too



this code is simple because i am NOT multiplexing the two 7 segment displays, and im using decoders to make my 10 pins off the 7 segment into 4 
thus this ammo counter has some specific hardware limit:

1. you MUST be using either TWO SEPARATE 7 segment displays or a double digit display that has 18 TOTAL pins. the two digits are controlled SEPARATELY
2. you MUST be using decoders that match your 7 segment displays, they are categorized by either common anode or common cathode. 
3. this code is designed for common anode(common positive pin), if you have something different you just need to update the HIGH and LOW sets at the 
   bottom of this code to make numbers, and plug the common into Vcc for common anode or Gnd for common cathode


ill call out what variables you need to change to edit the:

MAXmag - Maximum Magazine capacity, as in the largest magazine you own that will be shot with this ammo counter (mine is a 35 dart drum, so my MAXmag is 35)

BASEmag - BASE magazine size, as in the value that is displayed the moment the ammo counter is turned on (this doesn't really matter since you can change 
          this value at any moment and the code will remember the new value) 



oh and to note, this counter only supports up to 99 rounds in a magazine, if you want to go up to 999 (for like a proton pack or something) you need to add another digit
and another decoder, and add a digthree A through D



happy counting!
*/

//next 9 are names of the pinouts of the outbound signals (code applies voltages), so change based on where you plugged things in

int digtwoA = 1;
int digtwoB = 2;
int digtwoC = 3;
int digtwoD = 4;
int digoneA = 5;
int digoneB = 6;
int digoneC = 7;
int digoneD = 8;

//these are also named pins, but of inbound signals (code is checking for voltages), change based on where you plugged things in

int magSizeUp = 9;
int magSizeDown = 10;
int irbeam  = 11;
int insertMagSensor = 12;

//this variable is what we paste the IR beam results to, all it does is simplify writing, dont change 

int sensorState = 0;

//these are just variables I mentioned, change based on what I said above 

int BASEmag  = 17;
int MAXmag   = 35;
int CURRENTmag;

void setup() {

//this is just telling the code what each pin does, so all the variables above are called upon down here
//think like algebra 2 where you plug in the value for the variable
  
pinMode(digoneA,OUTPUT);
pinMode(digoneB,OUTPUT);
pinMode(digoneC,OUTPUT);
pinMode(digoneD,OUTPUT);
pinMode(digtwoA,OUTPUT);
pinMode(digtwoB,OUTPUT);
pinMode(digtwoC,OUTPUT);
pinMode(digtwoD,OUTPUT);
pinMode(magSizeUp,INPUT_PULLUP);        //for all 3 im turning on an internal resistor so when it sees the voltage pass through it doesnt burn the chip
pinMode(magSizeDown,INPUT_PULLUP);
pinMode(insertMagSensor,INPUT_PULLUP);
pinMode(irbeam,INPUT);

digitalWrite(irbeam, HIGH);    //turns on ir beam light
}

void loop() {
  sensorState = digitalRead(irbeam);                //sets output of irbeam to the variable sensorstate
  func(CURRENTmag);                                 //func is all the binary for the decoders saved and CURRENTmag is the CURRENT magazine size
  
  while(!digitalRead(insertMagSensor)==LOW){       //this keeps code within this loop until statement is true
    buttonValue();                                  //the function that allows the up and down buttons to change BASE magazine size
    func(BASEmag);                                  //displays the BASE magazine size 
    CURRENTmag=BASEmag;                             //sets the CURRENT magazine size to the BASE magazine size
  }
  
  if(digitalRead(insertMagSensor)==LOW&&sensorState==LOW){    //checks if the magazine is inserted AND if a dart passed the sensor
  CURRENTmag--;                                                //makes the CURRENT magazine size decrease by 1
  delay(100);                                                  //debounce, if this wasnt here some darts could actaully set off the breakbeam twice 
  }
}

void buttonValue(){                    //the function that allows the up and down buttons to change the BASE magazine size
  if(digitalRead(magSizeUp)==LOW){    //looks to see if you are pressing the up button
    delay(100);                        //debounce
    if (BASEmag<MAXmag){               //checks to see if the BASE magazine size is less than 35, if you have larger magazine size you change this and add code below
    BASEmag = ++BASEmag;               //increases BASE magazine size by 1
    }
    else {                             //pretty much just cycles back around so if a button breaks you can still change your BASE magazine size
      BASEmag = 0;                     //if you reach a value greater than maximum magazine size it cycles back to 0
    }
  }
  if(digitalRead(magSizeDown)==LOW){   //looks to see if you are pressing the down button
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
  digitalWrite(digoneA,LOW);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<20){
 //teens
  digitalWrite(digoneA,HIGH);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<30){
 //twenty
  digitalWrite(digoneA,LOW);
  digitalWrite(digoneB,HIGH);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<40){
 //thirty
  digitalWrite(digoneA,HIGH);
  digitalWrite(digoneB,HIGH);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<50){
 //fourty
  digitalWrite(digoneA,LOW);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,HIGH);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<60){
 //fifty
  digitalWrite(digoneA,HIGH);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,HIGH);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<70){
 //sixty
  digitalWrite(digoneA,LOW);
  digitalWrite(digoneB,HIGH);
  digitalWrite(digoneC,HIGH);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<80){
 //seventy
  digitalWrite(digoneA,HIGH);
  digitalWrite(digoneB,HIGH);
  digitalWrite(digoneC,HIGH);
  digitalWrite(digoneD,LOW);
  }
  else if(CURRENTmag<90){
 //eighty
  digitalWrite(digoneA,LOW);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,HIGH);
  }
  else if(CURRENTmag<100){
 //ninety
  digitalWrite(digoneA,HIGH);
  digitalWrite(digoneB,LOW);
  digitalWrite(digoneC,LOW);
  digitalWrite(digoneD,HIGH);
  }
  
/* 
   
the values below are for the SECOND digit of the 7 segment display, it works by using a modulus (the percentage symbol%)
a modulus pretty much divides the first value by the second, and returns the REMAINDER of the division, so 1 % 10 returns 1, 13 % 10 returns 3 and so on

*/
  
  if(CURRENTmag % 10 == 0){
 //zero
  digitalWrite(digtwoA,LOW);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,LOW);
  }
    if(CURRENTmag % 10 == 1){
  //one
  digitalWrite(digtwoA,HIGH);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 2){
  //two
  digitalWrite(digtwoA,LOW);
  digitalWrite(digtwoB,HIGH);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 3){
  //three
 digitalWrite(digtwoA,HIGH);
  digitalWrite(digtwoB,HIGH);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 4){
  //four
  digitalWrite(digtwoA,LOW);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,HIGH);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 5){
  //five
  digitalWrite(digtwoA,HIGH);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,HIGH);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 6){
  //six
  digitalWrite(digtwoA,LOW);
  digitalWrite(digtwoB,HIGH);
  digitalWrite(digtwoC,HIGH);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 7){
  //seven
  digitalWrite(digtwoA,HIGH);
  digitalWrite(digtwoB,HIGH);
  digitalWrite(digtwoC,HIGH);
  digitalWrite(digtwoD,LOW);
    }
    if(CURRENTmag % 10 == 8){
  //eight
  digitalWrite(digtwoA,LOW);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,HIGH);
    }
    if(CURRENTmag % 10 == 9){
  //nine
  digitalWrite(digtwoA,HIGH);
  digitalWrite(digtwoB,LOW);
  digitalWrite(digtwoC,LOW);
  digitalWrite(digtwoD,HIGH);
    }
}
