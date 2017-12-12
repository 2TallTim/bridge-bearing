#ifndef ADS130E08
#define ADS130E08
//TI ADS130E08 Analog interface

#include <SPI.h>

class ads130e08
{
public:
    ads130e08();
    ~ads130e08();
    void setCS(int cs);
private:
    int csPin;
};

#endif
