# Robot
This is the journal/code repository for a physical robot I am working on to test some of the AI software I do in a real system.
My intent is to use an Arduino computer for sensor/motor control, with a Raspberri Pi for high-level AI and vision.  Hardware is a simple 4-wheel platform purchased as a kit.

##  Parts
The following parts have been used for this robot

  * DFRobot 4WD Platform with encoders
  * DFRobot Romeo V 1.3 Arduino compatible controller with motor controllers
  * 0.1 μF ceramic capacitors (4)
  * 3x6x0.025 inch aluminum sheet (to make battery tray)
  * 6-32 1-1/2 inch bolts (4) 
  * 6-32 3/8 inch hex nuts (12)
  * Rechargable Lithium-Polymer battery (Turnigy 5000mAh 2S 40C Lipo Pack)
  * Bullet connector cable for battery (5.5mm Bullet to 4 x 4.0mm Multistar ESC Power Breakout Cable) \[Comment 2\]
  * 3/8 inch inner-diameter rubber grommets (2)
  * LMS303 Tilt compensated 3-axis Compass (DFRobot RB-Dfr-421)


##  Current Work
  * Wiring compass module to microcontroller
  * Writing compass calibration method up for this blog

##  Construction History/Notes
  * (5/7/16) Added capacitors and connection wires to motors (Do this before assembly!) \[Image 1\]
  * (5/9/16) Motors, encoders, and wheels added to base. \[Comment 1\]
  * (5/11/16) Tested motors with provided battery pack
  * (5/13/16) Added top plate to base.  Mounted Arduino board
  * (5/16/16) Constructed tray for rechargable battery  \[Comment 3\] \[Image 3\]
  * (5/17/16) Installed tray for rechargable battery (widening of slots needed to install bolts) \[Image 4\]
  * (5/19/16) Drilled holes in upper base for power/motor wires and installed grommets   \[Comment 4\] \[Image 5\]
  * (5/21/16) Connected wires, re-assembled base, tested motor  \[Code 1\]
  * (6/14/16) Connected wheel encoders to Romeo microcontroller (digital pins 2 and 3)
  * (7/6/16) Calibrated Wheel Encoders  \[Code 2\]   \[Comment 5\]
  * (7/13/16) Calibrated Compass from temporary wiring  \[Code 3\]
  
##  Comments
  1. - The instructions for putting on the wheel encoders doesn't place the sensor correctly.  There is plenty of hardware provided to adjust the spacing.  Play with it till it works.  \[Image 2\]
  2. - Rather than make my own bullet connector-to-wire connections, I bought a pre-made assembly and cut what I needed off of it.
  3. - I bent 1/4 inch sides on the long edge of the 3x6 inch aluminum, to make a 2-1/2x6 inch plate that was sturdier and had edges to keep the battery from sliding off.
  4. - Holes for grommets were 1/2" in diameter.
  5. - This took a while.  The interrupts for the encoders on the Arduino 'cross-talked' pulses between them.  Newer versions of the Romeo use a 32U4 chip, while this version uses a 328P chip.  The 32U4 chip has dedicated interrupts on pins 2 and 3, while the 328P can only use a block 'change' interrupt for all digital pins except 0 and 1.  Had to add code to check for pin state change in the interrupt handlers, as well as a debounce time.
  
##  Code
  1. - [Initial motor tests](Code/MotorTest.c).  Uses the 5 buttons to stop, go foreward, go backwards, turn left, and turn right
  2. - [Encoder calibration tests](Code/EncoderTest.c).  Uses the 5 buttons to clear eeprom, do a calibration run, check calibration, and put results on serial port
  3. - [Compass calibration tests](Code/CompassTest.c).  Displays heading, and can display raw data for determining calibration constants.  Use S2 button to write calibration constants (hard coded) into EEPROM
  
##  Images
  * Image1 - Capacitor and wires on motor:
![](Images/Image1.jpg?raw=true)
  * Image2 - Mounting hardware used on motor and encoder:
![](Images/Image2.jpg?raw=true)
  * Image3 - Battery tray:
![](Images/Image3.jpg?raw=true)
  * Image4 - Battery tray installed:
![](Images/Image4.jpg?raw=true)
  * Image5 - Wire holes and grommets:
![](Images/Image5.jpg?raw=true)


