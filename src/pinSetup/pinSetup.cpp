#include <Arduino.h>
#include "config/Define.hpp"
#include "pinSetup/pinSetup.h"

void setupPins() {
    pinMode(B_Encoder,           INPUT);
    pinMode(inputRESET,     INPUT);
    SerialUSB.println("1");
    
    pinMode(NEAR_SENSOR1,   INPUT);
    pinMode(NEAR_SENSOR2,   INPUT);
    pinMode(NEAR_SENSOR3,   INPUT);
    pinMode(NEAR_SENSOR4,   INPUT);
    pinMode(NEAR_SENSOR5,   INPUT);
    pinMode(NEAR_SENSOR6,   INPUT);
    pinMode(NEAR_SENSOR7,   INPUT);
    pinMode(NEAR_SENSOR8,   INPUT);
    pinMode(LED_EXCLK,   INPUT);


    pinMode(TRIGGER1,       OUTPUT);
    pinMode(TRIGGER2,       OUTPUT);
    pinMode(TRIGGER3,       OUTPUT);
    pinMode(TRIGGER4,       OUTPUT);

    pinMode(BAD_EXIT1,      OUTPUT);
    pinMode(BAD_EXIT2,      OUTPUT);
    pinMode(BAD_EXIT3,      OUTPUT);
    pinMode(BAD_EXIT4,      OUTPUT);
    pinMode(BAD_EXIT5,      OUTPUT);
    pinMode(BAD_EXIT6,      OUTPUT);
    pinMode(BAD_EXIT7,      OUTPUT);
    pinMode(BAD_EXIT8,      OUTPUT);

    pinMode(CRACK_EXIT1,    OUTPUT);
    pinMode(CRACK_EXIT2,    OUTPUT);
    pinMode(CRACK_EXIT3,    OUTPUT);
    pinMode(CRACK_EXIT4,    OUTPUT);
    pinMode(CRACK_EXIT5,    OUTPUT);
    pinMode(CRACK_EXIT6,    OUTPUT);
    pinMode(CRACK_EXIT7,    OUTPUT);
    pinMode(CRACK_EXIT8,    OUTPUT);

    // pinMode(PLC1 , OUTPUT);
    // pinMode(PLC2 , OUTPUT);
    // pinMode(PLC3 , OUTPUT);
    // pinMode(PLC4 , OUTPUT);



    pinMode(TEST_PIN,       OUTPUT);

    
    pinMode(LED_OE0,    OUTPUT);
    pinMode(LED_OE1,    OUTPUT);



}

void setup_init(){
    // digitalWrite(EXTLED1,   LOW);   
     
    digitalWrite(TRIGGER1,  LOW);
    digitalWrite(TRIGGER2,  LOW); 
    digitalWrite(TRIGGER3,  LOW); 
    digitalWrite(TRIGGER4,  LOW); 
    // digitalWrite(TRIGGER5,  LOW);
    // digitalWrite(TRIGGER6,  LOW); 
    // digitalWrite(TRIGGER7,  LOW); 
    // digitalWrite(TRIGGER8,  LOW); 

    digitalWrite(LED_OE0,  LOW); 
    digitalWrite(LED_OE1,  LOW);

}