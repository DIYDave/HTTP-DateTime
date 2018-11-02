/* Sample sketch to test the HttpDateTime library
Copyright (c) 2018 by Dave Waldesbuehl

For testing, send lines like this over a Serial connection:
Date: Sat, 22 Sep 2018 07:25:05 GMT        //RFC1123
Date: Saturday, 02-Jun-18 07:25:05 GMT     //RFC850
*/

#include <HttpDateTime.h>

// User settings
const int TIMEZONE = 1;             // +/- Timezone offset to GMT. e.g. 1 for MEZ, 0 for GMT
const bool USEDAYLIGHT = 1;         // Use European daylight saving or not
static char *weekday[7] = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "Sonntag"};     //Change to your language

//Variables
String rxText;
stHttpDT LocalTime;                 // Define variable with "stHttpDT" struct

HttpDateTime HttpDateTime(TIMEZONE,USEDAYLIGHT);          // Create class instance

void setup(void){ 
  Serial.begin(19200);
}

void loop(void) { 
  if (Serial.available() > 0){
    char rxChar = Serial.read();
    if (rxChar == '\n'){
      if (HttpDateTime.getDateTime(rxText,LocalTime)){
         printString();
      }else{
        Serial.println("Error parsing header");
      }
      rxText = "";
    }else{
      rxText += rxChar;
    }  
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

