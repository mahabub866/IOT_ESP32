#include <DMD32.h>
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_black_16.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

   //Timer setup
  //create a hardware timer  of ESP32
  hw_timer_t * timer = NULL;

void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{

// return the clock speed of the CPU
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  
  // Use 1st timer of 4 
  // devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer
  timer = timerBegin(0, cpuClock, true);
  // Attach triggerScan function to our timer 
  timerAttachInterrupt(timer, &triggerScan, true);
  // Set alarm to call triggerScan function  
  // Repeat the alarm (third parameter) 
  timerAlarmWrite(timer, 300, true);

  // Start an alarm 
  timerAlarmEnable(timer);

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)

}

void loop(void)
{
   byte b;
   
   // 10 x 14 font clock, including demo of OR and NOR modes for pixels so that the flashing colon can be overlayed
   dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16);
   dmd.drawMarquee("Scrolling Text",14,(32*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+100) < millis()) {
       ret=dmd.stepMarquee(-1,0);
       timer=millis();
     }
   }
}
