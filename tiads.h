#ifndef TIADS
#define TIADS
//TI ADS130E08 Analog interface

#include <SPI.h>

class TiADS
{
public:
    TiADS();
    ~TiADS();
    void setCS(int cs);

    //SPI Command interface
    char readRegister(char addr);
    void writeRegister(char data, char addr);
    
    //SPI Commands
    enum CMDS{
        WAKEUP  = 0x02;
        STANDBY = 0x04;
        RESET   = 0x06;
        START   = 0x08;
        STOP    = 0x0A;

        RDATAC  = 0x10;
        SDATAC  = 0x11;
        RDATA   = 0x12;
        RREG    = 0x2000;
        WREG    = 0x4000;
    };
    enum REGS // High byte is register, low byte is mask
    {
        CLK_EN     = 0x0120,
        INT_TEST   = 0x0210,
        TEST_AMP   = 0x0204,
        TEST_FREQ1 = 0x0202,
        TEST_FREQ0 = 0x0201,
        PD_REFBUF  = 0x0380,
        VREF_4V    = 0x0320,
        OPAMP_REF  = 0x0308,
        PD_OPAMP   = 0x0304,
        COMP_TH2   = 0x0480,
        COMP_TH1   = 0x0440,
        COMP_TH0   = 0x0420,
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

    bool readFlag(REGS f);
    void writeFlag(REGS f, bool v);

private:
    int csPin;
};

#endif


/*
To set bits high: OR with mask bit
ex:
  0101010 |
  0000100 =>
  0101110


*/


// 1000 0000 = 0x80   BIT 7
// 0100 0000 = 0x40   BIT 6
// 0010 0000 = 0x20   BIT 5
// 0001 0000 = 0x10   BIT 4
// 0000 1000 = 0x08   BIT 3
// 0000 0100 = 0x04   BIT 2
// 0000 0010 = 0x02   BIT 1
// 0000 0001 = 0x01   BIT 0
