#include "Grbl.h"
#include "I2SOut.h"

String pinName(uint8_t pin) {
    if (pin == UNDEFINED_PIN) {
        return "None";
    }
    if (pin < I2S_OUT_PIN_BASE) {
        return String("GPIO(") + pin + ")";
    } else {
#ifdef USE_EXT_IO_PIN_LIMIT
        if (pin >= EXT_IN_PIN_BASE) {
            return String("EXT_IO(") + (pin - EXT_IN_PIN_BASE) + ")";
        }
#endif
        return String("I2SO(") + (pin - I2S_OUT_PIN_BASE) + ")";
    }
}

// Even if USE_I2S_OUT is not defined, it is necessary to
// override the following functions, instead of allowing
// the weak aliases in the library to apply, because of
// the UNDEFINED_PIN check.  That UNDEFINED_PIN behavior
// cleans up other code by eliminating ifdefs and checks.
void IRAM_ATTR digitalWrite(uint8_t pin, uint8_t val) {
    if (pin == UNDEFINED_PIN) {
        return;
    }
    if (pin < I2S_OUT_PIN_BASE) {
        __digitalWrite(pin, val);
        return;
    }

#ifdef USE_EXT_IO_PIN_LIMIT
    if (pin >= EXT_IN_PIN_BASE) {
        return;
    }
#endif

#ifdef USE_I2S_OUT
    i2s_out_write(pin - I2S_OUT_PIN_BASE, val);
#endif
}

void IRAM_ATTR pinMode(uint8_t pin, uint8_t mode) {
    if (pin == UNDEFINED_PIN) {
        return;
    }
    if (pin < I2S_OUT_PIN_BASE) {
        __pinMode(pin, mode);
    }
    // I2S out pins cannot be configured, hence there
    // is nothing to do here for them.
}

int IRAM_ATTR digitalRead(uint8_t pin) {
    if (pin == UNDEFINED_PIN) {
        return 0;
    }

    if (pin < I2S_OUT_PIN_BASE) {
        return __digitalRead(pin);
    }

#ifdef USE_EXT_IO_PIN_LIMIT
    if (pin >= EXT_IN_PIN_BASE) {
        extern uint8_t getExtIOStatus(uint8_t pin_in);
        return getExtIOStatus(pin);
    }
#endif

#ifdef USE_I2S_OUT
    return i2s_out_read(pin - I2S_OUT_PIN_BASE);
#else
    return 0;
#endif
}
