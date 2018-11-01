# HTTP-DateTime
Library package for Arduino and other C ++ IDE. Made and tested for ESP8266. With small changes also usable with Arduino or ESP32.
- Read the date and time from the HTTP header of the server response.
- User-selectable time zone and European daylight saving time.

## How does it work?
On every request to a server, first a header is sent back as an answer. A request may be for example an API request on Youtube, on Openweathermap, or load request for a HTML page.

Normally, this header is not used except possibly to ask the status of the request. Here's an example of a header for a API request on a Youtube server. 
``` 
HTTP/1.1 200 OK
Expires: Sat, 22 Sep 2018 07:25:05 GMT
Date: Sat, 22 Sep 2018 07:25:05 GMT
Cache-Control: private, max-age=0, must-revalidate, no-transform
ETag: "XI7nbFXulYBIpL0ayR_gDh3eu1k/my59RHan7AsdbaUbFTij2u03Zek"
Vary: Origin
Vary: X-Origin
Content-Type: application/json; charset=UTF-8
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
X-XSS-Protection: 1; mode=block
Content-Length: 506
Server: GSE
Alt-Svc: quic=":443"; ma=2592000; v="44,43,39,35"
```
Mandatory part of this header is the date and time of the answer. This is described here: https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1 The information can be provided in two defined formats. The formats RFC1123 and RFC850 are supported in this library.

Since all servers of large companies are synchronized with a high-precision clock, this time can be used to synchronize the internal clock of an MCU. This works even without an external Real Time Clock (RTC) device with battery only with the clock frequency of the MCU. It is sufficient if the internal clock is synchronized once every hour. When starting an MCU, it only takes about 1-2 seconds for the clock to be received. (After connecting to the Internet).

## Examples
### Serial_Test.ino
This sketch is for test purpose only.
This can be used, for example, to test summer time switching. The user sends an excerpt of the header via the serial interface and the MCU sends back the edited date and time.
