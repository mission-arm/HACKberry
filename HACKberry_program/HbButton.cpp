#include "Arduino.h"
#include "HbButton.h"


HbButton::HbButton() {
    this->pin = pin;
    this->isDigital = isDigital;
    this->isPullup = isPullup;
    this->isInit = false;

    if (isDigital) {
        if (isPullup) {
            pinMode(pin, INPUT_PULLUP);
        } else {
            pinMode(pin, INPUT);
        }
    } else {
    }
}

boolean HbButton::attach(int pin, boolean isDigital, boolean isPullup) {
    this->pin = pin;
    this->isDigital = isDigital;
    this->isPullup = isPullup;
    this->isInit = false;

    if (isDigital) {
        if (isPullup) {
            pinMode(pin, INPUT_PULLUP);
        } else {
            pinMode(pin, INPUT);
        }
    } else {
    }

    return true;
}

boolean HbButton::read() {
    if (!this->isInit) return LOW;

    int count = 0;
    if (isDigital) {
        for(int i=0; i<HBBUTTON_MAX_COUNT; i++) {
            if (digitalRead(this->pin) == HIGH) count++;
        }
    } else {
        for(int i=0; i<HBBUTTON_MAX_COUNT; i++) {
            if (analogRead(this->pin) > 512) count++;
        }
    }

    return 2*count > HBBUTTON_MAX_COUNT ? HIGH : LOW;
}
