#ifndef EGG_EXIT_H
#define EGG_EXIT_H

#include <Arduino.h>

class EggExit {
public:
    
    // crack
    static void crackExitOn(byte data);
    static void crackExitOff(byte data);
    
    // badExit
    static void badExitOn(byte data);
    static void badExitOff(byte data);

    // badExit-single
    static void badExitOnOne(byte data, int count);
    static void badExitOffOne();
};

#endif
