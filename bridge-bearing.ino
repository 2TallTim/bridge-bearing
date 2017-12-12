//ADC
#include "ads130e08.h"
//WiFi
#include <WiFi101.h>
//RTC
#include <Wire.h>
#include "RTClib.h"
//SD
#include <SD.h>

#define DEBUG

//RTC Vars
RTC_PCF8523 rtc;

//SD Vars
Sd2Card card;
SdVolume volume;
SdFile root;

//ADC Vars
ads130e08 adc1;

const int SD_CS = 10; //SD chip select


void setup() {
    Serial.begin(115200);

    #ifdef DEBUG
    while(!Serial){} //Wait for usb serial connection to debug
    #endif


    Serial.println("Initializing SD Card...");
    if(!card.init(SPI_HALF_SPEED, SD_CS)){
        Serial.println("Connection issue. Check card is present.");
        return;
    }else{
        Serial.println("Card detected");
    }

    if(!volume.init(card)){
        Serial.println("Could not find FAT16/FAT32 partition \n Format card before use.");
        return;
    }else{
        Serial.println("Volume initialized");
        Serial.print("Volume found: FAT");
        Serial.print(volume.fatType(), DEC);
        Serial.print(" of size ");
        uint32_t volumesize;
        volumesize = volume.blocksPerCluster();
        volumesize *= volume.clusterCount();
        volumesize /= 2048; //MBytes
        Serial.print(volumesize);
        Serial.println(" MB");
    }

    Serial.println("\nInitializing WiFi...");

    WiFi.setPins(8, 7, 4, 2);

    if(WiFi.status() == WL_NO_SHIELD){
        Serial.println("WiFi not present.");
    }else{
        Serial.print("WiFi present with status ");
        Serial.println(WiFi.status());
    }

    ads130e08 adc1 = ads130e08();

    Serial.println("\nInitializing RTC...");

    if(! rtc.begin()){
        Serial.println("Couldn't find RTC");
        return;
    }else{
        Serial.print("RTC initialized at ");
        Serial.println(formatDate(rtc.now()));
    }

}

String formatDate(DateTime t){
    String s = "";
    s += String(t.year(), DEC);
    s += "/";
    s += String(t.month(), DEC);
    s += "/";
    s += String(t.day(), DEC);
    s += " ";
    s += String(t.hour(), DEC);
    s += ":";
    s += String(t.minute(), DEC);
    s += ":";
    s += String(t.second(), DEC);

    return s;
}

void loop() {
  // put your main code here, to run repeatedly:

}
