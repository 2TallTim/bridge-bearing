#include "sdbuf.h"


/******************/
/* Initialization */
/******************/

char SDBufferingWriter::buffer[BUFSIZE];
size_t SDBufferingWriter::nchars;
File SDBufferingWriter::curfile;

int SDBufferingWriter::init(int sdcs){
    if( !SD.begin(sdcs) ){
        Serial.println("Card init failed.");
        return -1;
    }

    nchars = 0;

    return 1;
}

void SDBufferingWriter::open(const char* fname, byte mode){
    if(curfile){
        flush();
        curfile.close();
    }
    curfile = SD.open(fname, mode);
    if(!curfile){
        Serial.println("ERROR OPENING FILE");
    }
}

void SDBufferingWriter::close(){
    if(curfile){
        flush();
        curfile.close();
    }
}

void SDBufferingWriter::flush(){
    curfile.write(buffer, nchars);
    curfile.flush();
    nchars = 0;
}

/********************/
/* Printing Methods */
/********************/

int SDBufferingWriter::print(const char* s){
    size_t len = strlen(s);
    if(len + nchars > BUFSIZE){
        flush();
    }
    memcpy(buffer+nchars, s, len);
    nchars += len;
    return len;
}

int SDBufferingWriter::println(const char* s){
    size_t len = strlen(s);
    char buf[len+4];
    char nl[2];
    nl[0] = '\n';
    nl[1] = '\0';

    strcat(buf,nl);
    int ret = print(buf);
    return ret;
}