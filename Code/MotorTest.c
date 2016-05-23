//Standard PWM DC control - Swap if needed for your robot
#define M1FORWARD HIGH
#define M1BACKWARD LOW
#define M2FORWARD LOW
#define M2BACKWARD HIGH
int M1Speed = 5;    //  M1 Speed Control
int M2Speed = 6;    //  M2 Speed Control
int M1Dir = 4;      //  M1 Direction Control
int M2Dir = 7;      //  M1 Direction Control

void setup() {
  //  Set the motor control pins as outputs
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  pinMode(13, OUTPUT);  //we'll use the debug LED to indicate a motor is on
}

void loop() {
  //  Get a button press
  int button;
  button = getButton();
  delay(50);      // wait for debounce time
  int secondButton = getButton();
  if (secondButton == button) {
    switch (button) {
      case 0:
        stop();
        digitalWrite(13, LOW);
        break;
      case 1:
        forward(255);
        digitalWrite(13, HIGH);
        break;
      case 2:
        backward(255);
        digitalWrite(13, HIGH);
        break;
      case 3:
        left(150);
        digitalWrite(13, HIGH);
        break;
      case 4:
        right(150);
        digitalWrite(13, HIGH);
        break;
    }
  }
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

void stop()
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



