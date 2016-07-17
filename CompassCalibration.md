# Compass Calibration

This file describes how to use the CompassTest sketch to set the compass calibration constants in the EEPROM image.


##  Get a data rose

The first step is to get a data rose.  Line the compass module up approximately magnetic north, and make sure it is level.  Install the CompassTest sketch, with the line that calls the "printValues(mag, accel)" function in the loop() function uncommented, and all the constants set to 0 (mag_x_offset, mag_y_offset, mag_x_multiplier, and mag_declination in the eeprom_values structure).  Run the sketch and open the Serial Monitor in the Arduino developer software (set it to a baud rate of 19200 if needed)
Write down the heading (if level, the tilt compensated and non-compensated readings should be very similar) and the magnetometer X and Y readings for each of the 8 octant directions - turning the compass module 45 degrees for each reading.  This should give you something like the following (I made these numbers up randomly, do not assume your numbers will look anything like the diagram values):
```
                       187 
                     (-24, 567)
                         |
         143 (0, 345)    |
                  \      |      / 243 (-80, 247)
                   \     |     /
                    \    |    /
                     \   |   /
                      \  |  /
                       \ | /
                        \|/
102----------------------*--------------------305
(56, 90)                /|\                   (-134, 108)
                       / | \
                      /  |  \
                     /   |   \
                    /    |    \
                   /     |     \ 
                  /      |      \
     52 (-83, 86)/       |       \  328 (-102, -187)
                         |
                         4
                      (-38, -420)
```
I labelled the rose lines with the following index numbers, where 1 is the reading from pointing north
```
8 1 2
 \|/
7-*-3
 /|\
6 5 4
```

##  Plot the data (optional)

I found it educational to plot the magnetometer values on a graph.  The result should be an ellipse.  The center of the elipse may not be near the 0,0 point, and the elongation from a circle can vary.  This offset and stretching factor are the constants needed to get accurate headings.  After calibration, the resulting values should be a circle centered at 0,0.

##  Calculate the constants

The center of the ellipse needs to be at 0,0.  So we will start by calculating the offsets for the X and Y axis.

Look at the values on the data rose and find the minimum and maximum X value.  They should most likely be at the readings taken for the 7 and the 3 index labels on the rose, but check the nearby others.  Average the minimum and maximum values to get the X offset.  Offset = (minimum X + maximum X) / 2, as an integer number.  Set this number into the initializer for the mag_x_offset value in the eeprom_values structure.

Perform the same operation on the Y values.  These will most likely come from the readings for rose positions 1 and 5.  Set this number into the initializer for the mag_y_offset value in the eeprom_values structure.

Next we need to turn the ellipse into a circle.  Find the range of the X and Y axis values.  The range is the maximum value minus the minimum value.  You averaged these values to calculate the offset, now subtract them (in the right order, the value that comes out must be positive).  The algorithm multiplies the X value by a scaling factor to make the circle, so the scaling factor we need is the (Y range) / (X range), as a floating point number.  Multiplying the X by this factor puts the reading into the same scale as the Y range.  Set this number into the initializer for the mag_x_multiplier value in the eeprom_values structure.

#  Look up the magnetic declination for your area

You can get the magnetic declination from a web site.  http://www.magnetic-declination.com is one posibility.  If the declination is not positive (east), make the value negative.  The value is a floating point value, so minutes in the reading must be converted to decimal.  i.e 14 degrees 40 minutes = 14 + 40/60 = 14.667.  Set this number into the initializer for the mag_declination value in the eeprom_values structure.

#  Set these values and test

Put the values calculated above into the sketch, comment out the call to "printValues(mag, accel)", and reload the sketch into the Arduino.  Verify now that the compass does a good job of pointing close to true north, and gives reasonable values for all other directions.

Once you have values that do the job correctly, press the S2 button to write these values into the EEPROM, for use by future sketches.


