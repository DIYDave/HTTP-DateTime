/* 
- Read date & time from server response HTTP header
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

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <Arduino.h>

#ifndef __HTTPDATETIME_H__
#define __HTTPDATETIME_H__

struct stHttpDT{
  int dayOfWeek;
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;
};

class HttpDateTime {
  public:
    HttpDateTime(int Offset, bool dls);
    bool getDateTime(String _header, stHttpDT &HttpDT);

  private:
    boolean selectMonth(String _smonth, stHttpDT &HttpDT);
    boolean selectDayOfWeek(String _sdayOfWeek, stHttpDT &HttpDT);
    boolean summertime(stHttpDT &HttpDT, int tzHours);
    void checkDT(stHttpDT &HttpDT);
    int _offset, _dlsoffset;   
    bool _dls;
};

#endif
