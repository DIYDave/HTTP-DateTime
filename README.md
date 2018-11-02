# HTTP-DateTime
Library package for Arduino and other C ++ IDE. 
## HTML date time library for Arduino
Made and tested for ESP8266 & ESP32. Also usable for Arduino with ethernet shield.
- Parse the date and time from the HTTP header of the server response.
- User selectable time zone and European daylight saving time.
**Authors:**  *Dave Waldesbuehl***
## How does it work?
On every request to a web server, for example an [API request on Youtube](https://developers.google.com/youtube/v3/), on [Openweathermap](https://openweathermap.org/api), or a simple [load request for a HTML page](https://www.google.ch), first a header is sent back as an answer.
Normally, this header is not used except possibly to ask the status of the response. Here's an example of a header for a API request on a Youtube server. (First part only)
``` 
HTTP/1.1 200 OK
Expires: Sat, 22 Sep 2018 07:25:05 GMT
Date: Sat, 22 Sep 2018 07:25:05 GMT
Cache-Control: private, max-age=0, must-revalidate, no-transform
ETag: "XI7nbFXulYBIpL0ayR_gDh3eu1k/my59RHan7AsdbaUbFTij2u03Zek"
Vary: Origin
Vary: X-Origin
```
Mandatory part of this header is the current date and time of the answer. The information can be provided in some defined formats. This is [described here](https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1) The most common formats RFC1123 and RFC850/RFC1036 are supported in this library.
Since all servers of large companies are synchronized with a high-precision clock, this date/time can be used to synchronize the internal clock of an MCU. This works even without an external Real Time Clock (RTC) device with battery. Only with the clock frequency of the MCU. It is sufficient if the internal clock is synchronized once every hour or so. When starting an MCU, it only takes about one second for the date/time to be received. (After connecting to the Internet).

Reading / parsing the date and time out of the header is easy with the, unfortunately little known, command **_sscanf_**.
```C++
sscanf(input, "%10[^,], %d-%3s-%d %d:%d:%d", ....
```
You find a very good but not exhaustive description of **_sscanf_** here: http://docs.roxen.com/pike/7.0/tutorial/strings/sscanf.xml

As the time zone in the HTTP header is always GMT, the bigger part in this library is the consideration of the time zones and the Daylight saving time (DST) function.
## How to use the library in my project?
1. Download the package (zip) from here.
2. install the .zip in Arduino IDE. [Description here](https://www.arduino.cc/en/Guide/Libraries#toc4)
3. Open one of the examples that are installed. In IDE Menu _"File/Examples/HttpDateTime"_
4. Set your data for your WiFi Connection.
```C++
const char* SSID     = "XXXXXX";              // The SSID of your WiFi accesspoint
const char* PASSWORD = "XXXXXX";              // password for your WiFi accesspoint
```
5. Set the Constant for your timezone and Daylight saving:
```C++
const int TIMEZONE = 1;                       // +/- Timezone offset to GMT. e.g. 1 for MEZ, 0 for GMT
const bool USEDAYLIGHT = 1;                   // Use European daylight saving or not
```
In these onlie examples a simple HTML page request is made to get the HTTP header from a Google server. You can use it in this way if you dont need other data. If you make a request to an API service anyway, you do not need this part of the code. Then you just have to filter the header from the answer and pass it to the function of the library. The function will return TRUE if the parsing was successful and the _LocalTime struct_ will hold the current date/time.
```C++
void parseDateTime(String _header){           
  if (HttpDateTime.getDateTime(_header,LocalTime)){     // Parse date and time from header and calculate timezone and daylight saving
     setTime(LocalTime.hour,LocalTime.minute,LocalTime.second,LocalTime.day,LocalTime.month,LocalTime.year);
  }else{
    Serial.println("Error parsing header");
  }
}
```
## Examples
### Serial_Test.ino
This sketch is for test purpose only.
This can be used, for example, to test summer time switching. The user sends an excerpt of the header via the serial interface and the MCU sends back the edited date and time.


