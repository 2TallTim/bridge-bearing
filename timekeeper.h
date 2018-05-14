#ifndef TIMEKEEPER
#define TIMEKEEPER

#include <Wire.h>
#include "RTClib.h"

class Timekeeper
{
public:
    Timekeeper();
    ~Timekeeper();
    static String formatDate(DateTime t);
    static DateTime now();
    static void setTimeout(uint32_t millis);
    static void forceSync();
    static void init(uint32_t _timeout);
private:
    static RTC_PCF8523 rtc;
    static DateTime lastSync_DT;
    static uint32_t lastSync; //millis() at last sync
    static uint32_t timeout;  //resync every n millis
};

#endif