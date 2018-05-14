
//ADC
#include "tiads.h"
//WiFi
#include <WiFi101.h>
//RTC
#include "timekeeper.h"
//SD
#include "sdbuf.h"

#define DEBUG

//RTC Vars
//RTC_PCF8523 rtc;

//SD Vars

//ADC Vars
TiADS *adc1;
TiADS *adc2;

//Interrupt function forward declarations

void interrupt1();
void interrupt2();

//CS lines
const int SD_CS = 10; //SD chip select
const int ADS1_CS = 11;
const int ADS2_CS = 12;
String formatDate(DateTime t);

// INITIALIZERS

int initWIFI(){
    WiFi.setPins(8, 7, 4, 2);
    Serial.println("\nInitializing WiFi...");
  
    auto s = WiFi.status();
    if(s == WL_NO_SHIELD){
        Serial.println("WiFi not present.");
        return -1;
    }else{
        Serial.print("WiFi present with status ");
        Serial.println(s);
    }
    
    // int num = WiFi.scanNetworks();
    // Serial.println(num);
    
    // Serial.println(WiFi.SSID(0));
}

//DEBUG, STRING, AND PRINTING

void hexepoch(DateTime t, char* buf){
    long epoch = t.unixtime();
    sprintf(buf,"%08X",epoch);
}

void printhex(byte b){
    if(b<16){
        Serial.print("0");
    }
    Serial.print(b,HEX);
}

//MAIN LOOP

void printChannel(int i, TiADS *t){
    Serial.print(t->channelResult(i));
}

void setup() {
    Serial.begin(115200);
    


    adc1 = new TiADS( ADS1_CS, 16, &interrupt1 );
    adc2 = new TiADS( ADS2_CS, 17, &interrupt2 );

    pinMode(16, INPUT);
    

    #ifdef DEBUG
    while( !Serial ){} //Wait for usb serial connection to debug
    #endif
    
    SDBufferingWriter::init(SD_CS);

    initWIFI();

    Timekeeper::init(20000);

    Serial.println("Starting up...");
    adc1->startup();
    Serial.println("Startup completed");
    //Set clock output
    adc1->writeFlag(TiADS::FLAG::CLK_EN, true);
    //adc2->startup();


    //Set square test
    //adc1->writeRegister(TiADS::REGS::CONFIG2, 0x71);

    for(byte i = 5; i < 13; ++i){
        //adc1->writeRegister(i,0x90);//Shutdown channels
        adc1->writeRegister(i,0x10);
    }
    //adc1->writeRegister(8,0x10); //Enable ch4
    
}

void logData(){
    //String now = hexepoch(rtc.now());
    DateTime dt = Timekeeper::now();
    char now[10];
    hexepoch(dt,now);
    SDBufferingWriter::println(now);
}

void loop() {
    #ifdef DEBUG
    if(Serial.available()){
        String cmd = Serial.readString();
        Serial.println(cmd);
        if(cmd == String("regs\n")){
            for (int i = 0; i < 13; ++i)
            {
                byte r;
                adc1->readRegister(i,&r);
                printhex(i);
                Serial.print(": ");
                for (int j = 7; j >= 0; --j)
                {
                    Serial.print((r>>j)&1);
                }
                Serial.print("\n");
            }

            for (int i = 18; i < 21; ++i)
            {
                byte r;
                adc1->readRegister(i,&r);
                printhex(i);
                Serial.print(": ");
                for (int j = 7; j >= 0; --j)
                {
                    Serial.print((r>>j)&1);
                }
                Serial.print("\n");
            }
        }
        if(cmd == String("sensor\n")){
            adc1->readData();
            for(int i = 0; i<8; i++){
                // printChannel(i, adc1);
                Serial.print(", ");
                char buf[9];
                sprintf(buf, "%8d", adc1->channelResult(i));
                Serial.print(buf);
            }
            Serial.println("");
        }
        if(cmd == String("write\n")){
            for(byte i = 5; i < 13; ++i){
                //adc1->writeRegister(i,0x90);//Shutdown channels
                adc1->writeRegister(i,0x10);
            }
        }
        if(cmd == String("sdtest\n")){
            SDBufferingWriter::open("log.txt",O_WRITE | O_CREAT);
            Serial.println("Buffered write test");
            int t = micros();
            for (int i = 0; i < 1000; ++i)
            {
                logData();
            }
            SDBufferingWriter::flush();
            Serial.print("Total time: ");
            Serial.println(micros()-t);


            Serial.println("Unbuffered test");
            t = micros();
            for (int i = 0; i < 1000; ++i)
            {
                logData();
                SDBufferingWriter::flush();
            }
            Serial.print("Total time: ");
            Serial.println(micros()-t);
            SDBufferingWriter::close();
        }
        if(cmd == String("timetest\n")){
            Timekeeper::setTimeout(1000);
            for (int i = 0; i < 100; ++i)
            {
                Serial.println(Timekeeper::formatDate(Timekeeper::now()));
                delay(100);
            }
            Timekeeper::setTimeout(20000);
        }
    }
#endif
}

void interrupt1(){
    Serial.println("Interrupt on 1");
    adc1->readInterrupt();
}

void interrupt2(){
    Serial.println("Interrupt on 2");
    adc2->readInterrupt();
}