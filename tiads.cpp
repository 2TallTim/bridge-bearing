#include "tiads.h"


TiADS::TiADS(){
    SPI.
}
TiADS::~TiADS(){}



void TiADS::setCS(int cs){
    csPin = cs;
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
}

char TiADS::readRegister(REGS reg){
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
    digitalWrite(cs, LOW);
    char ret = SPI.transfer16((CMDS::RREG | reg)<<8);
    digitalWrite(cs, HIGH);
    SPI.endTransaction();


    return 0xFF;
}

void TiADS::writeRegister(char data, char addr){
    return;
}

bool TiADS::readFlag(REGS f){
    return false;
}
void TiADS::writeFlag(REGS f, bool v){
    return;
}