#include <DMD32.h> 
#include "fonts/SystemFont5x7.h"
//#include <Fonts/FreeSerif9pt7b.h>

#include "fonts/Arial_Black_16_ISO_8859_1.h"
#include <WiFi.h>
#include <HTTPClient.h>


#define FONT_LENGTH      4
#define FONT_FIXED_WIDTH    8
#define FONT_HEIGHT     8


const char* ssid = "Home";
const char* password =  "Pass4321";

//Your Domain name with URL path or IP address with path
//String serverName = "https://w-access.hidayahsmart.solutions/rand";
//http://q.bacomaviation.com/q.html
String serverName = "http://q.bacomaviation.com/q.html";

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;


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

void setup(void)
{
   Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
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
   //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName ;
//      Serial.println(serverPath);
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
     
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
//        Serial.print("HTTP Response code: ");
//        Serial.println(httpResponseCode);

        String payload = http.getString();
        Serial.println(payload);
        char buf[5];
        payload.toCharArray(buf,5);
        dmd.clearScreen( true );
   dmd.selectFont(SystemFont5x7);
   // Français, Österreich, Magyarország
//   display.setCursor(5,6)
   dmd.drawString( 1, 0, "COU:8", 5, GRAPHICS_NORMAL );
//   dmd.drawString( 0, 5, "___", 3, GRAPHICS_NORMAL );
   dmd.drawString( 4, 9, buf, strlen(buf), GRAPHICS_NORMAL );
  
   
   //dmd.drawMarquee(buf,strlen(buf),(32*DISPLAYS_ACROSS)-1,0);


      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  

   
}
