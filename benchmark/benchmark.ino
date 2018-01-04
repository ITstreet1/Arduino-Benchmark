//
// Pi_2
//
// Steve Curd
// December 2012
//
// This program approximates pi utilizing the Newton's approximation.  It quickly
// converges on the first 5-6 digits of precision, but converges verrrry slowly
// after that.  For example, it takes over a million iterations to get to 7-8
// significant digits.

//
// I wrote this to evaluate the performance difference between the 8-bit Arduino Mega,
// and the 32-bit Arduino Due.
// 
#include <LCD5110_Graph.h>

#define ITERATIONS 100000L    // number of iterations
#define FLASH 1000           // blink LED every 1000 iterations

int progress = 1;

LCD5110 myGLCD(8,9,10,11,12);
extern unsigned char SmallFont[];
extern unsigned char TinyFont[];
extern uint8_t pi[];

char piArray[15];
char timeArray[15];
int percent = 0;

void setup() {

 myGLCD.InitLCD();
 myGLCD.setContrast(50);
 myGLCD.clrScr();
 myGLCD.drawBitmap(14, 0, pi, 84, 48);
 myGLCD.update();  
 delay(2000);
 pinMode(13, OUTPUT);        // set the LED up to blink every 1000 iterations
 Serial.begin(57600);
 myGLCD.clrScr();
 myGLCD.setFont(SmallFont);
 myGLCD.print("PI:",0,0);
 myGLCD.print("TIME:",0,20);
 printBar();
 myGLCD.update();
 
}

void loop() {
 
 unsigned long start, time;
 unsigned long niter=ITERATIONS;
 int LEDcounter = 0;
 boolean alternate = false;
 unsigned long i, count=0;
 float x = 1.0;
 float temp, pi=1.0;

 Serial.print("Beginning ");
 Serial.print(niter);
 Serial.println(" iterations...");
 Serial.println();
 
 start = millis();  
 for ( i = 2; i < niter; i++) {
   x *= -1.0;
   pi += x / (2.0f*(float)i-1.0f);
   if (LEDcounter++ > FLASH) {
     LEDcounter = 0;
     progress++;
     percent = progress*100/(ITERATIONS/FLASH);

     fillBar(percent);
     
     if (alternate) {
       digitalWrite(13, HIGH);
       alternate = false;
     } else {
       digitalWrite(13, LOW);
       alternate = true;
     }
     temp = 40000000.0 * pi;
   }
 }
 time = millis() - start;
 
 pi = pi * 4.0;
 
 itoa(time,timeArray,10);
 convertToString(pi);
 myGLCD.print(piArray,18,0);
 myGLCD.print(timeArray,30,20);
 myGLCD.print("ms",60,20);
 myGLCD.update();
 
 Serial.print("# of trials = ");
 Serial.println(niter);
 Serial.print("Estimate of pi = ");
 Serial.println(pi, 10);
 Serial.print("Time: "); Serial.print(time); Serial.println(" ms");
 
 delay(100000);
}

void printBar()
{  
  myGLCD.drawRect(2, 35, 81, 40);
  myGLCD.update(); 
}

void fillBar(int percent)
{
    percent = map(percent,0,100,2,81);     
    myGLCD.drawLine(2, 36, percent, 36);
    myGLCD.drawLine(2, 37, percent, 37);
    myGLCD.drawLine(2, 38, percent, 38);
    myGLCD.drawLine(2, 39, percent, 39);
    myGLCD.update(); 
}

void convertToString(float number)
{
   dtostrf(number, 10, 7, piArray);
}



