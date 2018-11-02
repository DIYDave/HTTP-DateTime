/* 
- extract date & time from server responsed HTML header
- Manage timezone & european daylight saving
- Format RFC1123 and RFC850 are supported
Base: https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1
Example:
Date: Sat, 22 Sep 2018 07:25:05 GMT      //RFC1123
Date: Saturday, 02-Jun-18 07:25:05 GMT   //RFC850

Copyright (c) 2018 by Dave Waldesbuehl

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
*/

#include "HttpDateTime.h"

HttpDateTime::HttpDateTime(int offset, bool dls){
 _offset = offset;
 _dls = dls;
}

bool HttpDateTime::getDateTime(String _header, stHttpDT &HttpDT){
  int _date = _header.indexOf("Date:");
  int _GMT = _header.indexOf("GMT",_date);
  char _smonth[4], _sdayOfWeek[11];

  if (_date < 0 || _GMT < 0){                       // No valid HTML header or incorrect format
    return false;
  }else{
    String stemp = _header.substring(_date+6,_GMT-1);
	int _dash = stemp.indexOf('-',_date);
    char input[stemp.length()+1];
    stemp.toCharArray(input,stemp.length()+1);

    if (_dash > 0){                                 // RFC850
      sscanf(input, "%10[^,], %d-%3s-%d %d:%d:%d", _sdayOfWeek, &HttpDT.day, _smonth, &HttpDT.year, &HttpDT.hour, &HttpDT.minute, &HttpDT.second);
      HttpDT.year = HttpDT.year + 2000;
    }else if (_dash < 0){                           // RFC1123{
      sscanf(input, "%4[^,], %d %3s %d %d:%d:%d", _sdayOfWeek, &HttpDT.day, _smonth, &HttpDT.year, &HttpDT.hour, &HttpDT.minute, &HttpDT.second);      
    }
	
    if (!selectDayOfWeek(_sdayOfWeek, HttpDT)) return false;    // Week not recognized
    if (!selectMonth(_smonth, HttpDT)) return false;            // Month not recognized

    if (_dls == 1 && summertime(HttpDT, _offset)){                // Daylight saving 
      _dlsoffset = 1;       
    }else _dlsoffset = 0; 
   
    HttpDT.hour = HttpDT.hour + _dlsoffset + _offset;
    checkDT(HttpDT);                    // Check if nessessary to change somthing after add offsets
  }  
   return true;                         // All operations are OK
}

void HttpDateTime::checkDT(stHttpDT &HttpDT){
// In case of positive timezone offset 
  if (HttpDT.hour > 23){
    HttpDT.day++;
    HttpDT.dayOfWeek++;
    if (HttpDT.dayOfWeek > 6) HttpDT.dayOfWeek = 0;
    HttpDT.hour -= 24;
  }
  
  switch (HttpDT.day) {           // switch to increase month
    case 29:
      if (HttpDT.month == 2 && HttpDT.year % 4 !=0){   // Kein Schaltjahr = 28 Tage
        HttpDT.month = 3;
        HttpDT.day = 1;
      }          
    break;     
    case 30:
      if (HttpDT.month == 2 && HttpDT.year % 4 ==0){   // Schaltjahr = 29 Tage
        HttpDT.month = 3;
        HttpDT.day = 1;
      } 
    break;
    case 31:
      if (HttpDT.month == 4 || HttpDT.month == 6 || HttpDT.month == 9 || HttpDT.month == 11){   // 30 tage
        HttpDT.month++;
        HttpDT.day = 1;     
      }
    break;    
    case 32:
      if (HttpDT.month == 1 || HttpDT.month == 3 || HttpDT.month == 5 || HttpDT.month == 7 || HttpDT.month == 8 || HttpDT.month == 10){   // 31 tage
        HttpDT.month++;
        HttpDT.day = 1;   
      }else {           
        if (HttpDT.month == 12){
          HttpDT.month = 1;
          HttpDT.day = 1; 
          HttpDT.year++;
        }                 
      }
    break;
  }

// In case of negative timezone offset 
  if (HttpDT.hour < 0){
    HttpDT.day--;
    HttpDT.dayOfWeek--;
    if (HttpDT.dayOfWeek < 0) HttpDT.dayOfWeek = 6;
    HttpDT.hour += 24;
  }
  if (HttpDT.day < 1){
    HttpDT.month--;
    if (HttpDT.month < 1){
      HttpDT.month = 12;    
      HttpDT.year--;           
    }
    switch (HttpDT.month){            // switch for change day backward
      case 1: HttpDT.day = 31; break;
      case 2:
        if (HttpDT.year % 4 !=0) HttpDT.day = 28;   //Kein Schaltjahr
        if (HttpDT.year % 4 ==0) HttpDT.day = 29;   // Schaltjahr
      break;
      case 3: HttpDT.day = 31; break;   
      case 4: HttpDT.day = 30; break;     
      case 5: HttpDT.day = 31; break;  
      case 6: HttpDT.day = 30; break;  
      case 7: HttpDT.day = 31; break;  
      case 8: HttpDT.day = 31; break; 
      case 9: HttpDT.day = 30; break;  
      case 10: HttpDT.day = 31; break;  
      case 11: HttpDT.day = 30; break;  
      case 12: HttpDT.day = 31; break;                                    
    }
  }  
}

boolean HttpDateTime::selectMonth(String _smonth, stHttpDT &HttpDT){
  if (_smonth == "Jan"){ HttpDT.month = 1; return true;
  }else if (_smonth == "Feb"){ HttpDT.month = 2; return true;
  }else if (_smonth == "Mar"){ HttpDT.month = 3; return true;
  }else if (_smonth == "Apr"){ HttpDT.month = 4; return true;
  }else if (_smonth == "May"){ HttpDT.month = 5; return true;
  }else if (_smonth == "Jun"){ HttpDT.month = 6; return true;
  }else if (_smonth == "Jul"){ HttpDT.month = 7; return true;
  }else if (_smonth == "Aug"){ HttpDT.month = 8; return true;
  }else if (_smonth == "Sep"){ HttpDT.month = 9; return true;
  }else if (_smonth == "Oct"){ HttpDT.month = 10; return true;
  }else if (_smonth == "Nov"){ HttpDT.month = 11; return true;
  }else if (_smonth == "Dec"){ HttpDT.month = 12; return true;
  }else return false; 
}

boolean HttpDateTime::selectDayOfWeek(String _sdayOfWeek, stHttpDT &HttpDT){
  if (_sdayOfWeek == "Mon" || _sdayOfWeek == "Monday"){ HttpDT.dayOfWeek = 0; return true;
  }else if (_sdayOfWeek == "Tue" || _sdayOfWeek == "Tuesday"){ HttpDT.dayOfWeek = 1;; return true;
  }else if (_sdayOfWeek == "Wed" || _sdayOfWeek == "Wednesday"){ HttpDT.dayOfWeek = 2;; return true;
  }else if (_sdayOfWeek == "Thu" || _sdayOfWeek == "Thursday"){ HttpDT.dayOfWeek = 3;; return true;
  }else if (_sdayOfWeek == "Fri" || _sdayOfWeek == "Friday"){ HttpDT.dayOfWeek = 4;; return true;
  }else if (_sdayOfWeek == "Sat" || _sdayOfWeek == "Saturday"){ HttpDT.dayOfWeek = 5;; return true;
  }else if (_sdayOfWeek == "Sun" || _sdayOfWeek == "Sunday"){ HttpDT.dayOfWeek = 6;; return true;
  }else return false; 
}

boolean HttpDateTime::summertime(stHttpDT &HttpDT, int tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
// Geändert von "1 + tzHours" auf "2 - tzHours"
{
 if (HttpDT.month<3 || HttpDT.month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
 if (HttpDT.month>3 && HttpDT.month<10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
 if (HttpDT.month==3 && (HttpDT.hour + 24 * HttpDT.day)>=(2 - tzHours + 24*(31 - (5 * HttpDT.year /4 + 4) % 7)) || HttpDT.month==10 && (HttpDT.hour + 24 * HttpDT.day)<(2 - tzHours + 24*(31 - (5 * HttpDT.year /4 + 1) % 7)))
   return true;
 else
   return false;
}
