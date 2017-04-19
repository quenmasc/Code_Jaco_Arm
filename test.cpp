#include "mySerial.h"
#include <iostream>
using namespace std;



int  main(void)
{

    mySerial serial("/dev/ttyAMA0",115200);

    // One Byte At the time
     serial.Send(128);
     serial.Send(132);

    // An array of byte
    unsigned char  dataArray[] = { 142,0};
    serial.Send(dataArray,sizeof(dataArray));

    // Or a string
    serial.Send("this is it\r\n");

    return 0;
}
