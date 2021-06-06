  // Railway Speedometer using Grove 4 Digit Display 
  // Set sensors 100 CM apart
  // Choose Scale
   
   
   #include "TM1637.h"

   // Pins definitions for TM1637 and can be changed to other pins
   const int CLK = 6;
   const int DIO = 7;
   TM1637 tm1637(CLK, DIO);

   //user variables
   float distance = 100; //distance between sensors (cm)
   int scale = 76; //scale of model to report scale speed  (0O-Scale = 76)
   //int scale = 48;//scale of model to report scale speed  (O-Scale = 48)
   //int scale = 148;//scale of model to report scale speed  (N-Scale=148)
   //int scale = 87;//scale of model to report scale speed  (H0-Scale=87)
   int sensor1 = 4;   //sensor1 digital pin
   int sensor2 = 5;   //sensor2 digital pin
   int s1Open = 1;   //is START sensor open/uncovered? (goes to 0 if covered/triggered)
   int s2Open = 1;   //is END sensor open/uncovered? (goes to 0 if covered/triggered)

   float startmillis, finishmillis, elapsed, miles, hours, mph, scaleMPH;
   bool started1 = false;    // set to TRUE if START sensor has been triggered 
   bool started2 = false;    // set to TRUE if END sensor has been triggered 

   bool initialized = true;   //set to TRUE once all sensors are cleared for a period of time and ready to read
   float previousmillis;



void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(sensor1, INPUT);
   pinMode(sensor2, INPUT);
   tm1637.init();
   tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7; 
   tm1637.clearDisplay();
   
}

void loop() {
   //read respective sensor if not already triggered   
   if (!started1) {s1Open=digitalRead(sensor1);}
   if (!started2) {s2Open=digitalRead(sensor2);}
  
   // if neither sensor previously triggered and both sensors currently uncovered and several seconds have passed
   // determine if we can "reset" all variables and set the display to "8888"
   if ((!started1 && !started2) && (s1Open && s2Open) && ((millis() - previousmillis > 5000)))  {
      bool goodtogo = true;
      float starttesttime = millis();
   // assure no sensor triggers for several seconds before even attempting "reset"
      while ((millis() - starttesttime) < 5000) {
      if (!digitalRead(sensor1) || !digitalRead(sensor2)) {
         goodtogo = false;
         break;
      }
    }
 
   // neither sensor was previously triggered, nor triggered in last several seconds, so "reset"/initialize vars and display
   if (goodtogo) {
      initialized = true;
      started1 = false;
      started2 = false;
      previousmillis = millis(); 
      tm1637.displayStr("8888");
      delay(500);     
  }
 
 }
    
    // Sensor 1 is triggered, neither sensor was previously triggered, and we were in initialize mode
    if ((!s1Open) && (!started1 && !started2) && (initialized)) {
     started1 = true;
     initialized = false;
     startmillis = millis();
    }
    // Sensor 2 is triggered, neither sensor was previously triggered, and we were in initialize mode
    if ((!s2Open) && (!started1 && !started2) && (initialized)) {
     started2 = true;
     initialized = false;
     startmillis = millis();
    }
    //a first sensor (START) was previously triggered and now a second sensor (END) is triggered, calculate and display scale MPH 
    if (((started1) && (!s2Open)) || ((started2) && (!s1Open))) {
     finishmillis=millis();    
     elapsed = finishmillis-startmillis;//time between sensors 
    
     elapsed = elapsed / 1000;    //seconds    
     miles = distance / 160934;//miles there are 160934 cm in 1 mile
     hours = elapsed / 3600;   //hours
     mph = miles / hours;
     scaleMPH = mph * scale;     
     tm1637.displayNum(scaleMPH);// print scale MPH on display
    
    started1 = false;
    started2 = false;
  }





    
}
