#include <Adafruit_LiquidCrystal.h>




#include <Servo.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Sensor pins
int sensorPower = 4;
int sensorPin = A3;
int ir = A2;
int b=3;
Servo servoDustbin;// Define our Servo for lid opening
Servo servoRotate;// Define our Servo for dustbin rotation
Servo ServoWaste; // Define our Servo for Waste Managemet
int trigpin = 10; // Trigger of Ultrasonic
int echopin = 11; // echo of Ultrasonic
int distance; 
float duration;
float cm;
int command; 
int left =49; // bluetooth 1
int right =50;  // Bluetooth 2

//Bluetooth TX pin goes to Arduino Digital 0 (RX)
//Bluetooth RX pin goes to Arduino Digital 1 (TX)



void setup() {
  pinMode(sensorPower, OUTPUT);
  pinMode(b,OUTPUT);
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
  Serial.begin(9600);
  pinMode(ir, INPUT);
  ServoWaste.attach(8); //servo on digital pin 8
  servoDustbin.attach(9); // servo on digital pin 9
  servoRotate.attach(6);// servo on digital pin 6
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  servoDustbin.write(180);
  servoRotate.write(145);

  // oled intialize

     // initialize OLED display with address 0x3C for 128x64
if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println(F("SSD1306 allocation failed"));
while (true);
  }

        // wait for initializing
oled.clearDisplay(); // clear display

oled.setTextSize(2);          // text size
oled.setTextColor(WHITE);     // text color


  
}

void loop() {

      // Code for Smart dustbin
ServoWaste.write(67);
digitalWrite(trigpin, LOW);
delay(2);  
digitalWrite(trigpin, HIGH);
delayMicroseconds(10);  
digitalWrite(trigpin, LOW);  
duration = pulseIn(echopin, HIGH);  
cm = (duration/58.82);
distance = cm;

if(distance<10)
  {
servoDustbin.write(0);// Turn Servo back to center position (90 degrees)

delay(3000); 
  }
else{
servoDustbin.write(180);

delay(50);
  }
  
// code for waste maageent
delay(50);
 
  
  int detection = analogRead(ir);
  Serial.println(detection);
  delay(500);
  // Make servo go to 0 degrees 
    

  if(detection < 100)
    { 
      tone(b,500);
      delay(1000);
      noTone(b);
      delay(2000);
      delay(3000);
      int result = readSensor();
      
  
      //get the reading from the function below and print it
      Serial.print("Analog output: ");
      Serial.println(result);
      

      wasteDetected();
      delay(1000);
      if (result < 980) {

      ServoWaste.write(20); 
      delay(2000); 
      organic();
      Serial.println("Organic");
      
      }else {
        delay(1000);
      ServoWaste.write(160); 
       delay(2000);
      Serial.println("Non-Organic");
      nonorganic();
      }
      
     }
      else
      {
        Serial.println("Nothing");
        teamName();
      }

// code for dustbin roraation below

if(Serial.available() > 0){ 
    command = Serial.read(); 
     
      
    Serial.println(command);
    if(command == left){
      servoDustbin.write(0);
      delay(1000);
      servoRotate.write(145);
      delay(1000);
      servoRotate.write(125);
      delay(1000);
      servoRotate.write(110);
      delay(1000);
      servoRotate.write(100);
      delay(1000);
      servoRotate.write(90);
      delay(1000);
      servoRotate.write(75);
      delay(1000);
      servoRotate.write(60);
      delay(1000);
      servoRotate.write(45);
      delay(1000);
      servoRotate.write(30);
      delay(1000);
      servoRotate.write(15);
      delay(1000);
      
      
      
      
      
    }else if(command == right){
      
      servoRotate.write(15);
      delay(1000);
      servoRotate.write(30);
      delay(1000);
      servoRotate.write(45);
      delay(1000);
      servoRotate.write(60);
      delay(1000);
      servoRotate.write(75);
      delay(1000);
      servoRotate.write(90);
      delay(1000);
      servoRotate.write(100);
      delay(1000);
      servoRotate.write(110);
      delay(1000);
      servoRotate.write(125);
      delay(1000);
      servoRotate.write(145);
      delay(1000);
      servoDustbin.write(180);
     delay(1000);
    }else{}
    
   
  }

  
    
  
}

//  This function returns the analog soil moisture measurement
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}

void wasteDetected(){

      oled.setCursor(0, 0);
      oled.println("Detected");
      oled.display();
      oled.clearDisplay();
  }

void organic(){

oled.setCursor(0, 0);
oled.println("Organic"); // text to display
oled.display();
oled.clearDisplay();
  }  

void nonorganic(){

oled.setCursor(0, 0);
oled.println("Non-Organic"); // text to display
oled.display();
oled.clearDisplay();
  }  

  void teamName(){

oled.setCursor(0, 0);
oled.println("Complain recieved"); // text to display
oled.display();
oled.clearDisplay();
  } 