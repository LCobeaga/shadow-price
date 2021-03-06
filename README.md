Shadow Price
=====

###### Shadow Price is the culmination of many years of CADing, which I had to learn along the way, and Coding, most of which I was taught by @mochoy. It uses brushless motors for the flywheels, a solenoid pusher, and has an integrated IR beam based ammo counter. The shell is based off of the stock weapon model for an auto rifle in the video game Destiny, and my personal blaster is painted like Shadow Price. 

### The STLs for all components are not inhenretly in the correct priting orientation, however there are large flat areas on all parts so use your best judgement when printing and print on these faces

### All minor changes to this document will be dated and placed at the bottom, major changes will be noted at the top here

### Note: This version uses tighter tolerances on all print surfaces. You will likely have to file the removeable components to get them to slide past each other.

## Parts List

For this Blaster you will need:

### Hardware
- 18 Regular Nerf screws (10mm long)
- 10 Small Nerf screws (5mm long)
- two small washers that will not slide over the main screw heads (or switch two of the small screws for something like the washer screws that are used on Stryfe mag releases)
- A small extension spring for the trigger
- A small compression spring for the magazine release (pen spring works)
- Optional: 
  - An 8-32 or M5 machine screw and nut (to hold up butt plate)

### Ammo Counter Electronics
- An Arduino Pro Mini 5V/16MHz 
- A small solderable breadboard
- two .56" 7 segment displays 
- two resistors (based on the color of your display) 
- two 7 segment decoders (make sure they are the same type as your displays)
- two 5mm breadboard compatible momentary push buttons
- A 3mm [IR break beam](https://www.adafruit.com/product/2167)
- A small [limit switch](https://www.allelectronics.com/item/sms-371/spdt-mini-snap-action-switch-w/sim-roller-lever/1.html)
- 22 gauge wire
- Heat shrink 
- Optional: 
  - Multiple colors of wire (to differentiate each button)
  - Bread board jumpers (for creating protoboard)
  - Crimp connectors (for power and ground from ESC-BEC)

__Firing System Electronics__
- An Arduino Pro Mini 5v/16MHz 
- A 10K Ohm [potentiometer](https://www.amazon.com/PoiLee-Potentiometer-Linear-WH148-Shaft/dp/B07BW8JBC8/ref=zg_bs_306810011_15?_encoding=UTF8&psc=1&refRID=9QH8NRF41JYXV1WAPK84)
- A medium sized [limit switch](https://www.allelectronics.com/item/sms-219/spdt-mini-snap-action-switch-w/roller-lever/1.html)
- A male battery connector of your prefrence 
- Two ESCs with BEC that are rated up to 3s and have a high enough amperage for your motors
- Two 2204-2300Kv brushless motors (currently working on adding support for 1806 sized motors as well)
- A 35mm stroke 12v electric solenoid 
- A 1N540x Rectifier Diode 
- A IRLB3034PBF MOSFET 
- A 10K Ohm resistor 
- 22 gauge wire
- 16 gauge wire
- heat shrink 
- Optional: 
  - 5mm bullet connectors (for removeable ESCs and easy polarity switching)
  - Crimp connectors (for power from ESC-BEC and to keep small control cables organized)
  - Suild's [MOSFET board 2](https://suild.com/shop/0) (as a replacement for all the MOSFET components)
  
## Software

### Firing Code

To Start, you will need to download the [JC Button Library](https://github.com/JChristensen/JC_Button) by Jack Christensen. 
If you dont know how to download a library to your Arduino IDE [this is a good guide](https://www.digikey.com/en/maker/blogs/2018/how-to-install-arduino-libraries) that I used, just scroll about half way down and there are two mehtods to download the library. 
There are only two areas in the code you should ever realistically need to change, the pinout and revSequence. 
The pinout is simple:
```
#define TRIGGER_PIN 9
#define MOTOR_PIN 10
#define ACTUATOR_PIN 11
#define POT_PIN 1
#define FIRE_MODE_PIN 7
```
- TRIGGER_PIN is for the trigger microswitch.
- MOTOR_PIN is for ***both*** of the signal wires from the ESCs, so make sure you splice the wires together at some point.
- ACTUATOR_PIN is for the signal wire coming off the solenoid MOSFET, in the case of the MOSFET board 2 from Suild it would be a wire coming off the gate pin.
- POT_PIN is for the 10K Ohm potentiometer signal pin, the wire that comes from the middle tab.
- FIRE_MODE_PIN is for an unutilized two position switch that would change between full auto and three round burst. While the code is written for this, there is no mounting geometry in the 3D prints, so you would have to download the .step file in the prints folder and model it in yourself.

The other area of code you might need to change is:
```
void revSequence(){
  flywheelESC.writeMicroseconds(1860);                
  delay(map(analogRead(POT_PIN), 0, 1023, 150, 50));    
  flywheelESC.writeMicroseconds(map(map(analogRead(POT_PIN), 0, 1023, 0, 100), 0, 100, 1250, 1650));
  delay(70);
}
```
In this function you might have to change the delays based on your motors' spin up time. In the first delay, the only values you might need to change are the 150 and 50, which determine how long you need the motors to accelerate for. 
Effectively, the motors accelerate the greatest when an immediate command to full speed is sent. 
What this line of code is doing is using that concept to put the motors at max acceleration for longer when the potentiometer is set to a low speed and accelerate for less time when at the optimal speed. 
Without this line of code the spin up times for all motor speeds would be significantly greater, so change these values only if your first dart fires significantly harder than the rest of your darts when holding the trigger.

The second delay at the end of the function is less complicated, and the only value you might need to change is the 70. 
Once the motor has been told the desired speed (from the previous line of code) it needs a moment to reach it, so the delay is just waiting for the motor to reach that speed, so if your blaster constantly jams when firing the first shot increase this delay, and if you feel like the first dart takes an eternity to fire decrease the delay. 
If you are going to play with this number I suggest you increase or degrease by units of 5 until you reach the minimum delay where your blaster wont jam. 

If you use a different ESC than the 12A AFRO that I use, the min and max speed in microseconds (1860) might be different, and since you will use different motors than the 2204-2300KV Turnigy Multistar Elites (because they are discontinued) the minimum firing and optimal speeds in microseconds (1250 & 1650) will be different. 

To find the max and min speeds in microseconds you will have to refer to the instruction manual that is associated with your ESC, or just test values until you find the threshold value where the motors begin to spin (the minimum microseconds) and find the threshold value where the motor speed does not seem to contimue to increase. 
Once you find these values just ctrl+F in the arduino IDE and switch out every 1060 & 1064 for your new minimum value and switch out every 1860 & 1864 for yor new maximum value (including in the setup code). 
Make sure all of these test you conduct inside the assembled blaster, since it is dependent on the solenoid firing 

To find the minimum firing and optimal speeds you will have to rely solely on testing. 
For minimum firing value, increase from the minimum value in incriments of 10 until you can fire 10 shots full auto without jamming. 
For optimal value, degrease from max in incriments of 10 until the FPS of the dart begins to drop, and you'll need a chronograph for this test (or just look at range with measuring tape, but that will take longer and require more walking). 
These tests can be conducted outside of the blaster, with you manually feeding the darts into the flywheel cage, but it's better if you conduct them inside the blaster since the results will be more realistic. 

### Ammo Counter Code

I will preface this section by saying that this ammo counter is not exlusive to Shadow Price, if you can find some other low current 5v power source to plug it into it will work completely independent. 
The amount it can count up to is also expandable, so if you want to make an ammo counter for your backpack fed blaster feel free to use this design, good luck with counting how many balls you put in at the beginning though. 

Just like before the pinout is relatively simple
```
#define DIG_ONE_A 2
#define DIG_ONE_B 3
#define DIG_ONE_C 4
#define DIG_ONE_D 5
#define DIG_TWO_A 6
#define DIG_TWO_B 7
#define DIG_TWO_C 8
#define DIG_TWO_D 9

#define IR_BEAM 14
#define MAG_SIZE_UP 10
#define MAG_SIZE_DOWN 11
#define MAG_SENSOR 12
```
- DIG_ONE_A-B and DIG_TWO_A-B are the A-B signal pins on the 7 segment decoders, ONE and TWO being for the tens and ones digits, respectively. 
- IR_BEAM is for the signal pin of the IR break beam sensor. 
- MAG_SIZE_UP is for the button that increases the magazine value. 
- MAG_SIZE_DOWN is for the button that decreases the magazine value. 
- MAG_SENSOR is for the limit switch that goes inside the magazine well. 

If you are using this ammo counter for Shadow Price or any other under-100-dart-capacity blaster the only other variables you might change are:
```
int BASEmag  = 17;
int MAXmag   = 35;
```
- BASEmag is the value the ammo counter will show on boot up.
You'll be able to change this value using the up and down buttons, so it doesn't really matter what this value is as long as it's less than the MAXmag value; I like putting it as the capacity of my first magazine in my loadout. 
- MAXmag is the greatest value the ammo counter will display, and the value at which it switches back to 0 if you keep hitting the MAG_SIZE_UP button. 
Make this value the capacity of your largest magazine you would put into the blaster; In my case the largest magazine I own is the 35 round drum. 

Now, if you want to expand this ammo counter to three digits (999 rounds max) or four digits (9999 rounds max) its pretty simple. 
1. Repeat the code in ```void func()``` that displays the ones position for every added digit (I suggest you put it at the end of the function to keep the code organized). 
2. Replace the variable in the ```digitalWrite();``` on your repeated section with the new pinout for your extra digit(s). 
3. Replace the conditions in the DIG_ONE display code with your new highest values (e.g. replace the 10,20,30, etc. with 100,200,300, etc)
4. In decending order, update the conditions for the new digits (e.g. replace the %10 == 1,2,3 etc with %100 == 10,20,30 for digit two, and then leave the conditions alone for the last digit). 
5. Update the BASEmag and MAXmag to values that better reflect your new ammo capacity.

If this didnt make sense feel free ask via reddit (u/Band3rsnach) and I can help, but none of this last bit is required for Shadow Price. 

## Printing and Assembly

### Recomended Printing Orientation

To save time in generating these images I've placed multiple parts on a single print bed. 
I am NOT suggesting you print multiple parts at once unless you are confident in your printer's ability. 
As a general rule, internal features will not need any support, and external features will require supports (if you want the blaster to look pretty). 
A few parts (tail, stock panel left, trigger well left) have extrememly thin surfaces that are for bridging or to reduce supports. 
If you are confident in your printer's ability to bridge use them, if not the surfaces should not cause any problems with supports. 

<img width="466" alt="barrel 1" src="https://user-images.githubusercontent.com/44830532/69666588-5698e200-1041-11ea-8b4b-11aa179b1e64.PNG">

Depicted: 3X Barrel Tip, Barrel Front, Barrel Middle, Barrel End, 2X Key, Lock

<img width="377" alt="barrel 3" src="https://user-images.githubusercontent.com/44830532/64136950-ef54f400-cda9-11e9-9bcf-49d8834d395c.PNG">

Depicted: Battery Tray

<img width="463" alt="body 1" src="https://user-images.githubusercontent.com/44830532/66980348-706c0000-f065-11e9-8fea-166e1a55f9d8.PNG">

Depicted: Handle, Knuckle Duster, Left & Right Grill, Bottom Dovetail, Pusher Cover, Solenoid Cover, Top Dovetail, Trigger

<img width="455" alt="body 2" src="https://user-images.githubusercontent.com/44830532/69671853-b300ff00-104b-11ea-80d4-78751e6f9596.PNG">

Depicted: Solenoid Mount, Tail, Tail Dovetail, Trigger Well Left & Right

The thin surface circled in red on Trigger Well Left is a built in support, and the Tail has a thin surface for bridging inside it. 

<img width="287" alt="functional" src="https://user-images.githubusercontent.com/44830532/64136969-0a276880-cdaa-11e9-8e26-0675ee1e7bbb.PNG">

Depicted: Flywheel Smooth, High Crush Flywheel Cage

<img width="408" alt="magwell 1" src="https://user-images.githubusercontent.com/44830532/64136985-18758480-cdaa-11e9-8ad2-c27afabfdfd7.PNG">

Depicted : Alignment Bar, Archway, Battery Tray Latch, Bottom Dovetail, Cage Mount, Flywheel Cover, Mag Release Lever, Mag Tooth, Magazine Release, Picatiny Left & Right, Top Dovetail

<img width="436" alt="magwell 2" src="https://user-images.githubusercontent.com/44830532/69670173-27d23a00-1048-11ea-980b-c6f1707ec580.PNG">

Depicted: Magwell, Insert

<img width="452" alt="stock" src="https://user-images.githubusercontent.com/44830532/64137000-2b885480-cdaa-11e9-9db3-c1f82d2cca94.PNG">

Depicted: 2X Button, 4X Slot, Button Mount, Buttstock, Fin, Left & Right Panel, Stock Main

The part circled in red is the Left Panel, and uses a thin surface for bridging in two areas near each button. 

### Assembly

Now, if like me you are impatient and like to assmeble as you print, I suggest you begin by printing all the parts in the Body and Magwell folders. 
These two folders will make up about 60-70% of your time building Shadow Price since they have a relatively complex assembly that keeps the blaster robust and secure. 
I **highly** suggest you use some form of solvent weld for this blaster instead of an epoxy or superglue, as there are some areas that can take a lot of stress during a fall, and its better to be too secure rather than not. 
For PLA I suggest Weldon 16, for PetG or similar Pet filaments I suggest Weldon 3 or 4 (4 is just more viscous, same formula), and for ABS obviously any form of acetone.
> One cool thing with Weldon 3 or 4 is that it also bonds styrene plastic, so if any part warps (like the Flywheel Cover) you can glue on styrene and it should be super easy to shape and fill gaps. Another good thing about specifically Weldon 3 is it is water thin, so you can use a small spray bottle head (make sure it doesnt disolve in the solvent!) or a brush to cover areas that seem to be cracking. This bonds the layers together like annealing and will make the area stronger (if you can, do this along the top angle of the Cage Mount). You can probably tell at this point that I printed mine in PetG. 

Ok, suggestions over, lets start assembling:

1. Screw into place and glue down Tail Dovetail into tail
<img width="396" alt="step -1" src="https://user-images.githubusercontent.com/44830532/69674255-f316b080-1050-11ea-9358-b441e2743280.PNG">

2. Screw into place and glue Insert into Cage Mount

<img width="387" alt="step -2" src="https://user-images.githubusercontent.com/44830532/69675212-fe6adb80-1052-11ea-9ed8-5aef25b8e92f.PNG">

3. Glue the Cage Mount, Archway, Solenoid Mount, and Tail together in a line. We will call this group the "upper reciever"
<img width="550" alt="step 1" src="https://user-images.githubusercontent.com/44830532/64640513-fccc3880-d3be-11e9-922f-c31bfcf743db.PNG">

4. Glue and snap the Magwell and Battery Tray Latch together by aligning the circular cutout and snapping in the triangular piece. If you are using a water thin solvent snap the parts together then put a bit of solvent in between the parts.
<img width="534" alt="step 2" src="https://user-images.githubusercontent.com/44830532/64640539-0e154500-d3bf-11e9-822c-1c777592b023.PNG">

5. Glue the Alignment Tab into the Magwell. It should be a tight fit.
<img width="592" alt="step 3" src="https://user-images.githubusercontent.com/44830532/64640706-64828380-d3bf-11e9-8fba-1a3e43772bfe.PNG">

6. Place the Mag Release lever into the Mag tooth and slot both into the back of the Magwell. Make sure the the cutout part of the lever faces away from the magwell, the part of the tooth that sticks out is towards the bottom of the magwell, and that **no glue** gets on any of these parts. 
<img width="362" alt="step 4" src="https://user-images.githubusercontent.com/44830532/64640758-811ebb80-d3bf-11e9-8cef-b5f9df81105d.PNG">
<img width="159" alt="step 4-2" src="https://user-images.githubusercontent.com/44830532/64641143-6436b800-d3c0-11e9-9a29-64f65b38f6ea.PNG">

These next two steps need to be done pretty quickly if you are not using a water thin solvent, so prepare everything before hand. 
If you are using a water thin solvent, just dont put any solvent in until after all the pieces are locked in place. 

7. Glue and slot Trigger Well Right into the back of the Magwell. This will lock in the magazine lever and tooth so it is critical you align those before gluing this piece on. This collection of pieces we will call the "lower reciever".
<img width="483" alt="step 5" src="https://user-images.githubusercontent.com/44830532/64641184-7b75a580-d3c0-11e9-84bd-b136e77d047a.PNG">

8. Glue, slot, and screw the upper and lower reciever together. Make sure the Alignment Tab is pushed into the back and top of the slot in the Archway, the spike on the front of the Cage Mount is flush with the cut out in the Battery Tray Latch and the screw is in place, the L shaped extrusion is in the corner of the Trigger Well Right, and that the entire top surface of the lower reciever is flush with the bottom of the upper.
<img width="621" alt="step 6" src="https://user-images.githubusercontent.com/44830532/64641197-84667700-d3c0-11e9-9ab8-0187ca47143c.PNG">

From here on the rest of the parts are mostly cosmetic, however they make up a lot of the length and weight of the blaster.
Make sure they are securely glued and mounted in place or you might get cracks in the shell after painting.

9. Glue and slot the Handle into the bottom of the Trigger Well Right. It should be a secure fit, just make sure you push it as far up as possible. 
<img width="393" alt="step 7" src="https://user-images.githubusercontent.com/44830532/64641255-a3650900-d3c0-11e9-91f6-7eb62bb183f3.PNG">

10. Place the small extension spring on to the Magazine Release pin, and insert the pair into the Knuckle Duster. Make sure the spring sits flat on the inside of the Knuckle Duster. I also suggest you paint this component before you put it in since there is no way to get it out. (Optional: add a bit of craft foam to the front of the Magazine Release to make it more comfortable to hit with your middle finger)
<img width="418" alt="step 8" src="https://user-images.githubusercontent.com/44830532/64641273-ad870780-d3c0-11e9-8405-e2780649ae30.PNG">

11. Slot the Knuckle Duster assembly into the bottom of the Trigger Well Right and the bottom of the Handle, and align the Magazine Release with the Mag Release Lever, making sure that the system is free to move.
<img width="272" alt="step 9" src="https://user-images.githubusercontent.com/44830532/64641405-fb037480-d3c0-11e9-816a-5a559ccd4f3b.PNG">

12. Screw the Knuckle Duster into the bottom of the Handle and glue the top of it into the bottom of the Trigger Well Right and the Magwell. Be careful not to get glue on the release system. 
<img width="375" alt="step 10" src="https://user-images.githubusercontent.com/44830532/64641427-035baf80-d3c1-11e9-90ee-019f8cb5ace8.PNG">

13. Glue and slot the Stock onto the back of the Tail. Be sure to get good adhesion both inside and on the two flush surfaces. 
<img width="516" alt="step 11" src="https://user-images.githubusercontent.com/44830532/64641740-a8768800-d3c1-11e9-8a10-477382c7d5c7.PNG">

14. Glue and slot the four Slots into the back of the Stock. Be sure to press them in firmly or the buttplate might be wobbly.
<img width="278" alt="step 12" src="https://user-images.githubusercontent.com/44830532/64641844-d956bd00-d3c1-11e9-8a7d-278cb4f22f36.PNG">

15. Glue and slot the Right Panel onto the right side of the Stock. Make sure to get good adhesion between the front of the panel and the Tail.
<img width="242" alt="step 13" src="https://user-images.githubusercontent.com/44830532/64641866-e5427f00-d3c1-11e9-91f9-414cd8e10ce9.PNG">

16. Moving back to the front of the blaster, glue and slot the Top & Bottom Dovetails to the Cage Mount and Battery Tray Latch. Make sure they are flush with the exterior of the blaster. 
<img width="272" alt="step 14" src="https://user-images.githubusercontent.com/44830532/64641962-19b63b00-d3c2-11e9-9ec8-40a7ccb40f56.PNG">

17. Glue and slot Barrel Middle and Barrel Back into place 

<img width="459" alt="step 14b" src="https://user-images.githubusercontent.com/44830532/69675790-4f2f0400-1054-11ea-8580-8b58a79d3513.PNG">

18. Glue, Slot, and Screw the Barrel assembly into place. 
Make sure it is flush against the Cage Mount both in the front and on the sides.
<img width="295" alt="step 15" src="https://user-images.githubusercontent.com/44830532/64642004-381c3680-d3c2-11e9-9c39-b4b2ad3dd3c9.PNG">

The next two steps are for one of the removeable covers, make sure not to glue it into the rest of the blaster.

19. Glue and slot the top and bottom dovetails into the Solenoid Cover
<img width="348" alt="step 17" src="https://user-images.githubusercontent.com/44830532/64642079-6437b780-d3c2-11e9-892f-894e0289772d.PNG">

20. Glue and slot the Trigger Well Left into the bottom of the Solenoid cover. (Optional: glue the two at a slight inward angle so that when screwing into the blaster the panel will always fill the break at the top)
<img width="393" alt="step 18" src="https://user-images.githubusercontent.com/44830532/64642099-6e59b600-d3c2-11e9-8664-fb669392a395.PNG">

21. Screw Picatiny Left & Right into the Flywheel Cover and Cage Mount, respectively.
<img width="293" alt="step 19a" src="https://user-images.githubusercontent.com/44830532/64642268-cf818980-d3c2-11e9-87bc-433de8a83b32.PNG">
<img width="284" alt="step 19b" src="https://user-images.githubusercontent.com/44830532/64642621-a1e91000-d3c3-11e9-82e6-764d26e40b9f.PNG">

22. Slot and Glue Grill left & right into the solenoid panel assembly and Solenoid Mount, respectively.
<img width="307" alt="step 20a" src="https://user-images.githubusercontent.com/44830532/64642647-add4d200-d3c3-11e9-9535-a8a71d35c4bb.PNG">
<img width="310" alt="step 20b" src="https://user-images.githubusercontent.com/44830532/64642658-b62d0d00-d3c3-11e9-8450-ef680c857333.PNG">

23. Slot and Glue the three Barrel Tips together.
<img width="342" alt="step 21" src="https://user-images.githubusercontent.com/44830532/64642676-bf1dde80-d3c3-11e9-9c3f-e82e4f229f25.PNG">

Now other than removeable components (Which are screw in) the blaster is fully assembled. 
I suggest that you dont glue the barrel tip assembly into the Barrrel End because its prone to breaking, and make sure that at no point do you glue the Flywheel Cover, Barrel Front, Buttstock, and the Solenoid Cover/Trigger Well Right assembly to the rest of the blaster or you will be unable to insert the internals. 

## Wiring

The blaster can be broken up into two circuits controlled by each of the Arduino Pro Minis and powered by each BEC output. 

This is the diagram for the ammo counter
![Ammo_Counter](https://user-images.githubusercontent.com/44830532/65849465-7e005680-e2ff-11e9-8b77-181c737970da.png)

And this for the firing system
![Firing_System](https://user-images.githubusercontent.com/44830532/72862203-47b95f00-3c81-11ea-8799-fa9791adb93e.png)

# Updates 

## 10/15/19
2Xkey file was added into the barrel folder. Two of them should be printed along the longest dimention and glued together. Will requre a little less sanding than the regular key, but it will still be a tight fit.

Swapped Left Panel and Stock Main in the stock folder for new versions that make use of a screw and bridging characteristics. No changes to printing orientation. Added another screw in the parts list.

## 10/16/19
Made changes to a majority of body components to improve aesthetics and to remove remnant geometry from older versions. 

Major changes include a block to better attach the Handle to the Trigger Well Right, combining the Solenoid Mount and Top Picatiny to prevent cracking in both parts (as observed in my personal print), and adding a 1 deg angle to the connection between the Trigger Well Left and Solenoid Cover to close shell gaps.

Began to strenghten area around mag release by increasing contact between the Magwell and Knuckle Duster, however no changes have been made to the Magwell so currently the blaster is non printable. 

Updated printing orientation photos and removed assembly step 16 pertaining to the Top Picatiny. 

## 11/24/19
Broke down Barrel Main into two components, Barrel Back and Barrel Middle.

Replaced Barrel end with Barrel Front, now with the new attachment lugs and optimised for bridging. 

## 11/25/19
Filled in gaps between Tail, Solenoid Mount, Solenoid Cover, Trigger Well Right, Magwell, Cage Mount, Flywheel Cover and Archway.

Cut out space in Magwell for the new Knuckle duster, now making the blaster printable again. 

Cut a hole in the top of Cage Mount.

- Created two new parts, Tail Dovetail and Insert.
- Tail Dovetial holds Solenoid Cover from the back and hopefully reduce flexing on Solenoid Mount.
- Insert goes inside of Cage Mount and will hopefully reduce the cracking that occurs in that area.
- Parts list updated to reflect four screws that hold the pieces in place.

## 11/26/19
Increased Insert tolerances. 

Added fillets to the picatiny on Solenoid Mount to reduce the need for supports.

Updated printing and assembly photos to reflect geometry changes. 

NOTE: Since many changes had no effect on assembly, not all photos were switched out, so go by description and use the photos as refrence. 

## 12/11/19
Multiple minor geometry changes as I print another shadow price and issues arise. 

- Pushed back dovetail and increased angle on Handle.
- New female side of dovetail on Trigger Well Left.
- Added screwport between Trigger Well Right and Left.
- Added reinfocement bar to Solenoid Cover to prevent cracking on small upper area.
- Removed extraneous walls in Archway to help with aethetics.
- Minor wall adjustments on Solenoid Mount so it meshes better with Archway and requires less support.

Updated Parts list with extra screw.

## 12/14/19
Redesigned all dovetails connecting the removeable handle cover from the blaster.

- Added new part, Tail Decoration, to prevent the need for a large support tower.
- Added a Dovetail to the side and back of Magwell and Archway. 
- New locations for two of the screws, moved from Trigger Well Left onto Solenoid Cover.
- Removed Top and Bottom Dovetails as they are now extraneous parts.

Created new set of combined parts for larger printers (150mm+ in X or Y).

- Added new parts Trigger Well Left Combined, Trigger Well Right Combined, Handle Combined.
- Moved geometry constraining the magazine release system onto the trigger well pair, should now allow for removeable components.

No changes to assembly order but some steps need to be removed due to dovetail changes.

## 12/16/19
Reinforced Magwell and surrounding parts

 - Added bridge support to Magwell dovetail.
 - Thickened wall on Magwell under the cage.
 - Made the anchor for Battery Tray Latch larger.
 - Removed extra features on inside of Cage Mount, added chamfers to reduce supports and increase strength, added extra screwport at the top of the Back Barrel connection, thickened walls and added bridge supports. 

Minor changes to Solenoid Mount and Cover to reduce required support.

Removed the original trigger well files as the new dovetail geometry was not reflected in their design and made them redundant.
The Knuckle Duster does not reflect the changes on the Trigger Well Combined files, so for now only use Handle Combined.

Cleaned up extraneous surfaces on stock main.

## 2/12/20
Dropped height of Battery Tray so top is planar, makes printing easier.

Increased wall length of Barrel Middle and Front to match Battery Tray.


