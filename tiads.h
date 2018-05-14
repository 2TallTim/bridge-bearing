#ifndef TIADS
#define TIADS
//TI ADS130E08 Analog interface

#include <SPI.h>

class TiADS
{
public:

    TiADS(int cs, int drdy, void(*ifxn)() );
    ~TiADS();
    
    //SPI Commands and addresses
    enum CMDS{
        WAKEUP  = 0x02,
        STANDBY = 0x04,
        RESET   = 0x06,
        START   = 0x08,
        STOP    = 0x0A,

        RDATAC  = 0x10,
        SDATAC  = 0x11,
        RDATA   = 0x12,
        RREG    = 0x20,
        WREG    = 0x40
    };
    enum REGS{
        ID      = 0x00,
        CONFIG1 = 0x01,
        CONFIG2 = 0x02,
        CONFIG3 = 0x03,
        FAULT   = 0x04

    };
    enum FLAG // High byte is register, low byte is bit number
    {
        CLK_EN     = 0x0105,
        INT_TEST   = 0x0204,
        TEST_AMP   = 0x0202,
        TEST_FREQ1 = 0x0201,
        TEST_FREQ0 = 0x0200,
        PD_REFBUF  = 0x0307,
        VREF_4V    = 0x0305,
        OPAMP_REF  = 0x0303,
        PD_OPAMP   = 0x0302,
        COMP_TH2   = 0x0407,
        COMP_TH1   = 0x0406,
        COMP_TH0   = 0x0405,
        CH1SET     = 0x0500,
        CH2SET     = 0x0600,
        CH3SET     = 0x0700,
        CH4SET     = 0x0800,
        CH5SET     = 0x0900,
        CH6SET     = 0x0A00,
        CH7SET     = 0x0B00,
        CH8SET     = 0x0C00
    };

    enum CHFLAGS {
        PD    = 0x0080,
        GAIN  = 0x0070,
        MUX   = 0x0007
    };

//SPI Commands

    void readRegister(byte addr, byte *result);
    void writeRegister(byte addr, byte data);

    bool readFlag(TiADS::FLAG f);
    void writeFlag(TiADS::FLAG f, bool v);

//State

    void standby();
    void wakeup();
    void reset();
    
    void startup();
    void readInterrupt();

//Conversion

    void startContinuousConversion();
    void stopContinuousConversion();

    void readData();

    int channelResult( int i );

    void waitConversion();

private:

    int csPin;
    int drdyPin;
    
    void startTrans();
    void endTrans();

    int chResult[8];
    volatile byte statusResult[3];
    
    bool conversion_running;
    void (*interrupt_fxn)();

    void resultReadback();

    volatile bool convwaiting;
};


#endif