# HTTP-DateTime
Library package for Arduino and other C ++ IDE. 

Made and tested for ESP8266. With small changes also usable with Arduino or ESP32.
- Read the date and time from the HTTP header of the server response.
- User-selectable time zone and European daylight saving time.

## How does it work?
On every request to a web server, first a header is sent back as an answer. A request may be for example an [API request on Youtube](https://developers.google.com/youtube/v3/), on [Openweathermap](https://openweathermap.org/api), or a simple [load request for a HTML page](https://www.google.ch).

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
Mandatory part of this header is the date and time of the answer. The information can be provided in some defined formats. This is [described here](https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1) The most common formats RFC1123 and RFC850/RFC1036 are supported in this library.

Since all servers of large companies are synchronized with a high-precision clock, this date/time can be used to synchronize the internal clock of an MCU. This works even without an external Real Time Clock (RTC) device with battery only with the clock frequency of the MCU. It is sufficient if the internal clock is synchronized once every hour or so. When starting an MCU, it only takes about 1-2 seconds for the clock to be received. (After connecting to the Internet).

Reading / parsing the date and time out of the header is easy with the, unfortunately little known, command **_sscanf_**.
```
sscanf(input, "%10[^,], %d-%3s-%d %d:%d:%d", ....
```
A very good but not exhaustive description of **_sscanf_** here: http://docs.roxen.com/pike/7.0/tutorial/strings/sscanf.xml

As the time zone in the HTTP header is always GMT, the bigger part in this library is the consideration of the time zones and the Daylight saving time (DST) function.

## How to use the library in my project?
1. Download the package (zip) from here.
2. install the .zip in Arduino IDE. [Description here](https://www.arduino.cc/en/Guide/Libraries#toc4)
3. Include the headerfile: #include <HttpDateTime.h>
4. Define a local variable with a structure: 
```
stHttpDT lokalTime;     // Define variable with "stHttpDT" struct 
```
⋅⋅⋅ This variable "lokalTime" holds now the date/time from the HTTP header.
5. Set the Constant for your timezone and Daylight saving:
```
const int TIMEZONE = 1;                       // +/- Timezone offset to GMT. e.g. 1 for MEZ, 0 for GMT
const bool USEDAYLIGHT = 1;                   // Use European daylight saving or not 
```
6. Create instance of the HttpDateTime Class: HttpDateTime HttpDateTime(TIMEZONE,USEDAYLIGHT);          // Create class instance

## Examples
### Serial_Test.ino
⋅⋅⋅This sketch is for test purpose only.
⋅⋅⋅This can be used, for example, to test summer time switching. The user sends an excerpt of the header via the serial interface and ⋅⋅⋅the MCU sends back the edited date and time.
