# HTTP-DateTime
Library package for Arduino and other C ++ IDE
- Read the date and time from the HTTP header of the server response.
- User-selectable time zone and European daylight saving time.

Made and tested for ESP8266. With small changes also with Arduino or ESP32 usable.

## How does it work?
On every request to a server, first a header is sent back as an answer. A request may be for example an API request on Youtube, on Openweathermap, or load request for a HTML page.

Normally, this header is not used except possibly to ask the status of the request. Here's an example of a header for a pageview on a Google server.
