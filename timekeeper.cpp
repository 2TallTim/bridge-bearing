#include "timekeeper.h"

uint32_t Timekeeper::lastSync;
DateTime Timekeeper::lastSync_DT;
uint32_t Timekeeper::timeout;
RTC_PCF8523 Timekeeper::rtc;

String Timekeeper::formatDate(DateTime t){
    char buf[20];
    sprintf(buf,"%04u-%02u-%02u %02u:%02u:%02u",
        t.year(),
        t.month(),
        t.day(),
        t.hour(),
        t.minute(),
        t.second());
    return String(buf);
}
void Timekeeper::init(uint32_t _timeout){
    if(!rtc.begin()){
        Serial.println("Couldn't find RTC");
    }else{
        DateTime now = rtc.now();
        Serial.print("RTC initialized at ");

        Serial.println(formatDate(now));
    }

    timeout = _timeout;
    forceSync();
}

DateTime Timekeeper::now(){
    if(millis() < lastSync || // Handle roll-over
       millis() > lastSync + timeout){
        forceSync();
    }
    return lastSync_DT + TimeSpan((millis() - lastSync) / 1000);
}

void Timekeeper::setTimeout(uint32_t millis){
    timeout = millis;
}

void Timekeeper::forceSync(){
    Serial.println("Synchronizing now.");
    DateTime now = rtc.now();
    lastSync = millis();
    lastSync_DT = now;
}