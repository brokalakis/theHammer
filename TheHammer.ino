/*
Arduino game "Hammer" v0.1
1/31/2016
by Konstantinos@brokalakis.gr
*/

#include <Servo.h> 


 
Servo servo;  
//init section for servo
int servoPin = 9; 
int angle = 0;   // servo position in degrees 
int openAngle = 120;
int closeAngle =180;
int openDelay=0;
//init section for pins
const int startPin = 2;     // the number of the start button
const int losePin = 3;     // button where on hammer
const int ledPin =  13;      // the number of the LED pin
const int MAX_HITS = 3;
// variables will change:
int startButtonState = 0;         // variable for reading the pushbutton status
int loseButtonState = 0; //when player loses
//

//volatiles used from the interrupt handler
volatile int gameOn = 0;
volatile int hits = 0;
long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;


void setup() 
{ 

  //servo initialization
  
  servo.attach(servoPin); 
  //this is important because the LEGO structure was getting destroyed :)
  servo.write(openAngle);
   // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(startPin, INPUT);
  pinMode(losePin, INPUT);
  // attach interupt when hummer touches finger
  attachInterrupt(digitalPinToInterrupt(losePin), got_hammered, RISING);
} 
 
 //main loop here
void loop() 
{ 

  
  startButtonState = digitalRead(startPin);
  // if it is, the buttonState is HIGH:
  if (startButtonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    gameOn=1;// we are ready to play
    hits=0;//init hits
    delay(1000);//let player a bit loose
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }

//check if we are still playing and the hits are less or equel to 3
while (gameOn>0 && hits<=MAX_HITS)
{
  openDelay=random(700, 4000); //create a random delay for the user


servo.write(openAngle);
delay(openDelay);
servo.write(closeAngle);
//close delay 
delay(800);
}
//  servo.write(openAngle);
  

  if (hits>MAX_HITS)
  {
    dance();
    hits=0;
    gameOn=0;
  }

}

//the hammer dance when player loses
void dance()
{
  int i=0;
  int j=0;
  for (j=0;j<4;j++)
  {
  for (i=0;i<10;i++)
  {
  servo.write(openAngle+20);
  delay(30);
  servo.write(openAngle+10);
  delay(30);
  }
  delay(300);
  }
  servo.write(openAngle);
}

void got_hammered()
{
 // gameOn=0;
   if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    hits++;
  servo.write(openAngle);
    last_micros = micros();
  }

}


