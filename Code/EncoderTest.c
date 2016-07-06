#include "EEPROM.h"

//Standard PWM DC control - Swap if needed for your robot
#define M1FORWARD HIGH
#define M1BACKWARD LOW
#define M2FORWARD LOW
#define M2BACKWARD HIGH

#define LEFT 0
#define RIGHT 1

const int M1Speed = 5;    //  M1 Speed Control
const int M2Speed = 6;    //  M2 Speed Control
const int M1Dir = 4;      //  M1 Direction Control
const int M2Dir = 7;      //  M1 Direction Control


const float test_distance = 2.0;      //  2 meter test distance
 
volatile byte last_encoder_state[2] = {0,0};
volatile long encoder[2] = {0,0};
volatile unsigned long encoder_time[2] = {0,0};

struct eeprom {
  float encoder_counts_per_meter;
  short num_trials;
} eeprom_values;

void setup() {
  //  Set the motor control pins as outputs
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  //  Attach interupts to the wheel incoders
  attachInterrupt(digitalPinToInterrupt(2), LeftEncoder, RISING);    //init the interrupt mode for the digital pin 2
  attachInterrupt(digitalPinToInterrupt(3), RightEncoder, RISING);   //init the interrupt mode for the digital pin 3
  
  pinMode(13, OUTPUT);  //we'll use the debug LED to indicate a motor is on

  //  Read the eeprom settings
  read_eeprom();
  
 Serial.begin(19200);                            //!!init the Serial port to print the debugdata
}

void loop() {
  float newEntry, newValue;
//  static unsigned long timer = 0;                //!!print manager timer
  
  //  Get a button press
  int button;
  button = getButton();
  delay(50);      // wait for debounce time
  int secondButton = getButton();
  if (secondButton == button) {
    switch (button) {
      case 0:           //  Button 1 - erase the EEPROM storage
        eeprom_values.encoder_counts_per_meter = 100.0;
        eeprom_values.num_trials = 1;
        write_eeprom();
        break;
      case 1:           //  Button 2 - start a trial
        //  Wait for the button to be released
        while (button == 1) {
          button = getButton();
        }
        //  Delay 10 ms for debounce
        delay(10);
        //  Clear the counts
        encoder[LEFT] = encoder[RIGHT] = 0;
        //  Start the motor - lower speed
        forward(140);
        //  Watch for end button press (without debounce)
        button = -1;
        while (button != 1) {
          button = getButton();
        }
        //  Record the values before they change
        newEntry = (float)(encoder[LEFT] + encoder[RIGHT]) * 0.5 / test_distance;  //  Average and divide by test distance
        //  Stop the motors
        stopMotors();
        //  Update the values in the eeprom
        newValue = eeprom_values.encoder_counts_per_meter * (float)eeprom_values.num_trials;
        newValue += newEntry;
        eeprom_values.num_trials++;
        newValue /= (float)eeprom_values.num_trials;
        eeprom_values.encoder_counts_per_meter = newValue;
        write_eeprom();
        //  Wait for the button to be released before continuing
        while (button == 1) {
          button = getButton();
        }
        delay(10);
        button = getButton();
        while (button == 1) {
          button = getButton();
        }
        break;
      case 2:         //  Button 3, go 1 meter forward
        //  Clear the counts
        encoder[LEFT] = encoder[RIGHT] = 0;
        //  Start the motor - medium speed
        forward(200);
        //  Wait for the average count to reach the meter value
        newEntry = (float)(encoder[LEFT] + encoder[RIGHT]) * 0.5;  //  Average
        while (newEntry < eeprom_values.encoder_counts_per_meter) {
          if ((encoder[LEFT] % 5) == 0) {
            if ((encoder[LEFT] % 10) == 0) {
              digitalWrite(13, LOW);
            }
            else {
              digitalWrite(13, HIGH);
            }
          }
          newEntry = (float)(encoder[LEFT] + encoder[RIGHT]) * 0.5;  //  Average
        }
        //  Stop the motors
        stopMotors();
        break;
      case 3:
        encoder[LEFT] = encoder[RIGHT] = 0;
        while (true) {
          Serial.print(encoder[LEFT]);
          Serial.print(", ");
          Serial.print(encoder[RIGHT]);
          Serial.println(" counts");          
          delay(500);
        }
        break;
      case 4:         //  Button 5, Write the latest value to the serial port
        Serial.print("After ");
        Serial.print(eeprom_values.num_trials);
        Serial.print(" trials, the result is ");
        Serial.print(eeprom_values.encoder_counts_per_meter);
        Serial.println(" counts/meter");
        break;
    }
  }
//
//  //  Debug output
//  if(millis() - timer > 1000){                   
//    Serial.print("Coder value: ");
//    Serial.print(encoder[LEFT]);
//    Serial.print("[Left Wheel] ");
//    Serial.print(encoder[RIGHT]);
//    Serial.println("[Right Wheel]");
//    timer = millis();
//  }
}

// Get and convert ADC value to button number
int  adc_key_val[5] ={ 30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int getButton()
{   
  int input = analogRead(7);    // read the value from the sensor  
  int k;
  for (k = 0; k < NUM_KEYS; k++) {
    if (input < adc_key_val[k])  {  
      return k;  
    }
  }
  if (k >= NUM_KEYS) k = -1;     // No valid key pressed
  return k;
}

void stopMotors()
{
  digitalWrite(M1Speed, 0);   
  digitalWrite(M2Speed, 0);      
}

void forward(char speed)
{
  analogWrite(M1Speed, speed); 
  digitalWrite(M1Dir, M1FORWARD);  
  analogWrite(M2Speed, speed);      
  digitalWrite(M2Dir, M2FORWARD);  
}

void backward(char speed)
{
  analogWrite(M1Speed, speed); 
  digitalWrite(M1Dir, M1BACKWARD);  
  analogWrite(M2Speed, speed);      
  digitalWrite(M2Dir, M2BACKWARD);  
}

//  These next two functions assume M1 is motor on the right.  Reverse Forward/Backward if needed
void left(char speed)   //  Speed should be below 128
{
  analogWrite(M1Speed, speed); 
  digitalWrite(M1Dir, M1FORWARD);  
  analogWrite(M2Speed, speed);      
  digitalWrite(M2Dir, M2BACKWARD);  
}

void right(char speed)   //  Speed should be below 128
{
  analogWrite(M1Speed, speed); 
  digitalWrite(M1Dir, M1BACKWARD);  
  analogWrite(M2Speed, speed);      
  digitalWrite(M2Dir, M2FORWARD);  
}


 
void LeftEncoder()
{
  //  Make sure this pin changed state
  byte current_state = (digitalRead(2) == 1);
  if (current_state == last_encoder_state[LEFT]) return;
  last_encoder_state[LEFT] = current_state;
  
  //  Ignore if it has not been at least 5 milliseconds since the last one
  unsigned long current_time = millis();
  if ((current_time - encoder_time[LEFT]) < 5) return;
  encoder_time[LEFT] = current_time;
  
  //  Count the left wheel encoder interrupts
  encoder[LEFT]++;  
}
 
 
void RightEncoder()
{
  //  Make sure this pin changed state
  byte current_state = (digitalRead(3) == 1);
  if (current_state == last_encoder_state[RIGHT]) return;
  last_encoder_state[RIGHT] = current_state;
  
  //  Ignore if it has not been at least 5 milliseconds since the last one
  unsigned long current_time = millis();
  if ((current_time - encoder_time[RIGHT]) < 5) return;
  encoder_time[RIGHT] = current_time;
  
  //  Count the right wheel encoder interrupts
  encoder[RIGHT]++;
}
 
void read_eeprom()
{
  char *ptr = (char *)&eeprom_values;
  for (int i = 0; i < sizeof(eeprom); i++) {
    *ptr = EEPROM.read(i);
    ptr++;
  }
}
 
void write_eeprom()
{
  char *ptr = (char *)&eeprom_values;
  for (int i = 0; i < sizeof(eeprom); i++) {
    EEPROM.write(i, *ptr);
    ptr++;
  }
}

