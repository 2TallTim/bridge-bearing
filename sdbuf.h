#ifndef SDBUF
#define SDBUF

#include <SD.h>

#define BUFSIZE 4096

class SDBufferingWriter
{
public:
    SDBufferingWriter();
    ~SDBufferingWriter();
    static int init(int sdcs);

    static void open(const char* fname,byte mode);

    static void close();

    static void flush();

    static int print(const char* s);

    static int println(const char* s);

private:

    static char buffer[BUFSIZE];
    static size_t nchars;
    static File curfile;

};

#endif //Whole file