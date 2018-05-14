#include "tiads.h"


TiADS::TiADS(int cs, int drdy, void(*ifxn)() ){
    csPin = cs;
    drdyPin = drdy;

    pinMode(csPin, OUTPUT);
    digitalWrite(cs, HIGH);

    pinMode(drdyPin, INPUT_PULLUP);
    this->interrupt_fxn = ifxn;
}

TiADS::~TiADS(){}

//#####################//
// Registers and Flags //
//#####################//

void TiADS::readRegister(byte addr,byte *result){
    startTrans();

    SPI.transfer(CMDS::RREG | addr);
    SPI.transfer(0x00);
    *result = SPI.transfer(0x00);
    
    endTrans();
}

void TiADS::writeRegister(byte addr, byte data){
    startTrans();
    
    SPI.transfer(CMDS::WREG | addr);
    SPI.transfer(0x00);
    SPI.transfer(data);
    
    endTrans();
}

bool TiADS::readFlag(FLAG f){
    byte b;
    readRegister(highByte(f), &b);
    return bitRead(b,lowByte(f));
}
void TiADS::writeFlag(FLAG f, bool v){
    byte reg;
    readRegister(highByte(f), &reg);
    bitWrite(reg,lowByte(f),v);
    writeRegister(highByte(f), reg);
}

void TiADS::standby(){
    startTrans();
    SPI.transfer(CMDS::STANDBY);
    endTrans();
}

void TiADS::wakeup(){
    startTrans();
    SPI.transfer(CMDS::WAKEUP);
    endTrans();
}

void TiADS::reset(){
    startTrans();
    SPI.transfer16(CMDS::RESET);
    endTrans();
}

void TiADS::startTrans(){
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
    digitalWrite(csPin, LOW);
}

void TiADS::endTrans(){
    SPI.endTransaction();
    digitalWrite(csPin, HIGH);
}

//#################//
// Data Conversion //
//#################//

void TiADS::startContinuousConversion(){
    startTrans();
    SPI.transfer(CMDS::RDATAC); //Set read data continuous mode
    SPI.transfer(CMDS::START);
    endTrans();
    conversion_running = true;
    attachInterrupt(drdyPin,interrupt_fxn,LOW);
}

void TiADS::stopContinuousConversion(){
    startTrans();
    SPI.transfer(CMDS::SDATAC);
    endTrans();
    conversion_running = false;
    detachInterrupt(drdyPin);
}

void TiADS::readData(){
    //Non-continuous reading
    int NSAMPLES = 10;
    startTrans();
    SPI.transfer(CMDS::START);
    while(digitalRead(drdyPin)){}
    SPI.transfer(CMDS::RDATA);
    

    //Status result
    for(int i = 0; i < 3; ++i){
        statusResult[i] = SPI.transfer(0x00);
    }
    delayMicroseconds(10);

    //Channel result (2 bytes)
    for(int i = 0; i < 8; ++i){
        chResult[i] = (int16_t)SPI.transfer16(0x0000);
    }

    for(int i = 1; i < NSAMPLES; i++){
        while(digitalRead(drdyPin)){}
        
        SPI.transfer(CMDS::RDATA);
        
        for(int i = 0; i < 3; ++i){
            SPI.transfer(0x00);//We don't care about status regs
        }
        delayMicroseconds(10);

        for(int i = 0; i < 8; ++i){
            chResult[i] += (int16_t)SPI.transfer16(0x0000);
        }        
    }
    endTrans();
    for(int i = 0; i<8; ++i){
        //chResult[i] /= NSAMPLES;
    }


}

void TiADS::startup(){
    reset();
    delay(1000);
    stopContinuousConversion();
    delay(10);
    writeRegister(REGS::CONFIG1,0x01);
    writeRegister(REGS::CONFIG2,0x60);
    for (int i = 5; i < 13; ++i)
    {
        writeRegister(i,0x11); //Set input short for noise measurement
    }
    Serial.println("Regs Written");
    startTrans();
    SPI.transfer(CMDS::START);
    delay(10);
    SPI.transfer(CMDS::RDATAC);
    Serial.println("cmds sent");
    int state = 1;
    while(state!=LOW){
        state = digitalRead(drdyPin);
    }
    Serial.println("statechg");
    
    for(int i = 0; i < 19; ++i){
        SPI.transfer(0x00);
    }
    delay(1);
    SPI.transfer(CMDS::SDATAC);
    endTrans();
    
}

void TiADS::waitConversion(){
    convwaiting = true;
    while(convwaiting){}
}

void TiADS::readInterrupt(){
    startTrans();

    for(int i = 0; i < 3; ++i){
        statusResult[i] = SPI.transfer(0x00);
    }
    for(int i = 0; i < 8; ++i){
        chResult[i] = (int16_t)SPI.transfer16(0x00);
    }
    endTrans();
    convwaiting = false;
}

int TiADS::channelResult( int i ){
    return chResult[i];
}
