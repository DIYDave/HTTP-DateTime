# HTTP-DateTime
Library package for Arduino and other C ++ IDE. Made and tested for ESP8266. With small changes also usable with Arduino or ESP32.
- Read the date and time from the HTTP header of the server response.
- User-selectable time zone and European daylight saving time.

## How does it work?
On every request to a server, first a header is sent back as an answer. A request may be for example an API request on Youtube, on Openweathermap, or load request for a HTML page.

Normally, this header is not used except possibly to ask the status of the request. Here's an example of a header for a API request on a Youtube server. ```
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
