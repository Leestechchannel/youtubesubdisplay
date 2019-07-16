/***********************************************************
 *  This code wil rRead YouTube Channel statistics from YouTube using Google API   *
 *  you can find full build instructions on my youtube channel https://www.youtube.com/channel/UCKGYYd0gWAjstw0ohLYRIeQ *
 ***********************************************************/

#include <YoutubeApi.h>       
#include <ESP8266WiFi.h>      
#include <WiFiClientSecure.h> 
#include <TM1637Display.h>    
#include <ArduinoJson.h>      

// ********** Fill in the following with your own detalis ********** //
char ssid[]     = "YOUR_WiFi_NETWORK-NAME-HERE";             // name of your WiFi network
char password[] = "YOUR-WiFi PASSWORD HERE";                 // your WiFi password
#define API_KEY "YOUR_GOOGLE_API_KEY_HERE"                   // your google apps API 
#define CHANNEL_ID "YOUR_YOUTUBE_CHSNNEL_ID_HERE"            // your youtube channel ID
// ------------------------------------- //

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 120000; // time between api requests
unsigned long counter = 0;       // Counter that increments every 10 milliseconds

// declare pins
#define CLK1 D5
#define DIO1 D4
TM1637Display display1(CLK1, DIO1);
long subRight   = 0;

void setup() {

  Serial.begin(115200);

  // Clear display
  display1.setBrightness(0x0f);
  display1.showNumberDec(0, false, 4, 0);

  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {

  counter = counter + 10;
  if (counter > api_mtbs)  {      
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);

      // Display the subscribers count
      subRight = api.channelStats.subscriberCount % 10000;               
      display1.showNumberDec(subRight, false, 4, 0);
    }
    counter = 0; // Reset counter after displaying the data
  }
  delay(10);
}
