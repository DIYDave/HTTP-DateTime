/* Sample sketch to demonstrate the HttpDateTime library function
 * Makes GET request to server and extracting date / time from received header.
 * Adds timezone and daylight saving if necessary 
 * Print the result over serial
Copyright (c) 2018 by Dave Waldesbuehl
*/

#include <ESP8266WiFi.h>
#include <HttpDateTime.h>					  // https://github.com/DIYDave

// WiFi settings
const char* SSID     = "XXXXXX";              // The SSID of your WiFi accesspoint
const char* PASSWORD = "XXXXXX";       		  // password for your WiFi accesspoint

// Http settings
const char SERVER[30] = "www.google.ch";	  // Every host is possible
String COMMAND ="https://www.google.ch/";     // Every site or API request is possible
const int SSL_PORT = 443;                     // Use port 80 for non secure connection

// Time settings
const int TIMEZONE = 1;                       // +/- Timezone offset to GMT. e.g. 1 for MEZ, 0 for GMT
const bool USEDAYLIGHT = 1;                   // Use European daylight saving or not
static char *weekday[7] = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "Sonntag"};     //Change to your language

unsigned long lastRequestTime = 10000;        // Last server request. INTERVAL time for instant requeat after start
unsigned long INTERVAL =  10000;              // Intervall time for request 
const unsigned long TIMEOUT = 2500;           // Timeout for header recieve

stHttpDT LocalTime;                           // Define variable with "stHttpDT" struct
HttpDateTime HttpDateTime(TIMEZONE,USEDAYLIGHT);          // Create class instance

WiFiClientSecure client;                      // SSL Client connection to server. use "WiFiClient client;" for port 80

void setup(void) 
{ 
  Serial.begin(19200);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) 
{  
  if (millis() - lastRequestTime > INTERVAL) {
    parseDateTime(HttpRequest());                       // Make request to server an give the header to the parser
    lastRequestTime = millis(); 
  }
}  


/////////////////////////////////// Online Function ///////////////////////////////////////////////////////
String HttpRequest() {                                      // makes a HTTP request
  unsigned long timeNow;
  bool HeaderComplete = false;
  bool currentLineIsBlank = true;
  String header="";
  client.stop();                                          // Close any connection before send a new request.  This will free the socket on the WiFi
  if (client.connect(SERVER, SSL_PORT)) {                 // if there's a successful connection:  
    client.println("GET " + COMMAND + " HTTP/1.1");
    client.print("HOST: ");
    client.println(SERVER);
    client.println();
    timeNow=millis();
    while (millis() - timeNow < TIMEOUT) {
      while (client.available()) {
        char c = client.read();
        if(!HeaderComplete){
          if (currentLineIsBlank && c == '\n') {
            HeaderComplete = true;
          }else{
            header = header + c;
          }
        } 
        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }                     
      }
      if (HeaderComplete) break;                        
    }  
  }else {
    Serial.println("Connection failed");
  }
  if(client.connected()) {   
    client.stop();
  } 
  return header; 
}
/////////////////////////////////// End Of Online Function ///////////////////////////////////////////////////////

void parseDateTime(String _header){           
  // Serial.println(_header);
  if (HttpDateTime.getDateTime(_header,LocalTime)){     // Parse date and time from header and calculate timezone and daylight saving
     printString();
  }else{
    Serial.println("Error parsing header");
  }
}

void printString(){
  char dtString[30];      
  sprintf(dtString,"%s, %02u.%02u.%04u %02u:%02u:%02u", 
                          weekday[LocalTime.dayOfWeek], 
                                  LocalTime.day, 
                                  LocalTime.month, 
                                  LocalTime.year, 
                                  LocalTime.hour, 
                                  LocalTime.minute, 
                                  LocalTime.second);
  Serial.println(dtString);
}

