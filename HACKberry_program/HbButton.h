#ifndef __HBBUTTON_H__
#define __HBBUTTON_H__

#define HBBUTTON_MAX_COUNT 10

#include "Arduino.h"

class HbButton
{
private:
    int pin;
    boolean isDigital;
    boolean isPullup;
    boolean isInit;
public:
    HbButton(int pin, boolean isDigital, boolean isPullup);
    HbButton();
    boolean attach(int pin, boolean isDigital, boolean isPullup);
    boolean read();
};

#endif
