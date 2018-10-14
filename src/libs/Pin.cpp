#include "Pin.h"
#include "utils.h"

// mbed libraries for hardware pwm
#include "PwmOut.h"
#include "InterruptIn.h"
#include "PinNames.h"
#include "port_api.h"

static GPIO_TypeDef* const gpios[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI};

Pin::Pin(){
    this->inverting= false;
    this->valid= false;
    this->pin= 16;
    this->port= nullptr;
}

// Make a new pin object from a string
Pin* Pin::from_string(std::string value){
    if(value == "nc") {
        this->valid= false;
        return this; // optimize the nc case
    }

    // cs is the current position in the string
    const char* cs = value.c_str();
    // cn is the position of the next char after the number we just read
    char* cn = NULL;
    valid= true;

    // grab first integer as port. pointer to first non-digit goes in cn
    this->port_number = strtol(cs, &cn, 10);
    // if cn > cs then strtol read at least one digit
    if ((cn > cs) && (port_number < (sizeof(gpios)/sizeof(gpios[0])))){
        // translate port index into something useful
        this->port = gpios[(unsigned int) this->port_number];
        // if the char after the first integer is a . then we should expect a pin index next
        if (*cn == '.'){
            // move pointer to first digit (hopefully) of pin index
            cs = ++cn;

            // grab pin index.
            this->pin = strtol(cs, &cn, 10);

            // if strtol read some numbers, cn will point to the first non-digit
            if ((cn > cs) && (pin < 16)){
                //this->port->FIOMASK &= ~(1 << this->pin); // stm32 doesn't have this feature

                // now check for modifiers:-
                // ! = invert pin
                // o = set pin to open drain
                // ^ = set pin to pull up
                // v = set pin to pull down
                // - = set pin to no pull up or down
                // @ = set pin to repeater mode
                for (;*cn;cn++) {
                    switch(*cn) {
                        case '!':
                            this->inverting = true;
                            break;
                        case 'o':
                            as_open_drain();
                            break;
                        case '^':
                            pull_up();
                            break;
                        case 'v':
                            pull_down();
                            break;
                        case '-':
                            pull_none();
                            break;
                        case '@':
                            as_repeater();
                            break;
                        default:
                            // skip any whitespace following the pin index
                            if (!is_whitespace(*cn))
                                return this;
                    }
                }
                return this;
            }
        }
    }

    // from_string failed. TODO: some sort of error
    valid= false;
    port_number = 0;
    port = gpios[0];
    pin = 16;
    inverting = false;
    return this;
}

// Configure this pin as OD
Pin* Pin::as_open_drain(){
    if (!this->valid) return this;

    gpios[this->port_number]->OTYPER |= (1 << this->pin);

    pull_none(); // no pull up by default
    return this;
}


// Configure this pin as a repeater
Pin* Pin::as_repeater(){
    if (!this->valid) return this;

    // stm32 doesn't support this mode

    return this;
}

// Configure this pin as no pullup or pulldown
Pin* Pin::pull_none(){
    if (!this->valid) return this;

	// Set the two bits for this pin as b00
    gpios[this->port_number]->PUPDR &= ~(0x3 << (2*this->pin));
	return this;
}

// Configure this pin as a pullup
Pin* Pin::pull_up(){
    if (!this->valid) return this;

    // Set the two bits for this pin as b01
    gpios[this->port_number]->PUPDR = (gpios[this->port_number]->PUPDR & ~(0x3 << (2*this->pin))) | (0x1 << (2*this->pin));
    return this;
}

// Configure this pin as a pulldown
Pin* Pin::pull_down(){
    if (!this->valid) return this;

    // Set the two bits for this pin as b10 
    gpios[this->port_number]->PUPDR = (gpios[this->port_number]->PUPDR & ~(0x3 << (2*this->pin))) | (0x2 << (2*this->pin));
    return this;
}

// If available on this pin, return mbed hardware pwm class for this pin
mbed::PwmOut* Pin::hardware_pwm()
{
    /* TODO STM32 PWM
    if (port_number == 1)
    {
        if (pin == 18) { return new mbed::PwmOut(P1_18); }
        if (pin == 20) { return new mbed::PwmOut(P1_20); }
        if (pin == 21) { return new mbed::PwmOut(P1_21); }
        if (pin == 23) { return new mbed::PwmOut(P1_23); }
        if (pin == 24) { return new mbed::PwmOut(P1_24); }
        if (pin == 26) { return new mbed::PwmOut(P1_26); }
    }
    else if (port_number == 2)
    {
        if (pin == 0) { return new mbed::PwmOut(P2_0); }
        if (pin == 1) { return new mbed::PwmOut(P2_1); }
        if (pin == 2) { return new mbed::PwmOut(P2_2); }
        if (pin == 3) { return new mbed::PwmOut(P2_3); }
        if (pin == 4) { return new mbed::PwmOut(P2_4); }
        if (pin == 5) { return new mbed::PwmOut(P2_5); }
    }
    else if (port_number == 3)
    {
        if (pin == 25) { return new mbed::PwmOut(P3_25); }
        if (pin == 26) { return new mbed::PwmOut(P3_26); }
    } */
    return nullptr;
}

mbed::InterruptIn* Pin::interrupt_pin()
{
    if(!this->valid) return nullptr;

    // set as input
    as_input();

    // TODO STM32 INT
    if (port_number == 0 || port_number == 2) {
        PinName pinname = port_pin((PortName)port_number, pin);
        return new mbed::InterruptIn(pinname);

    }else{
        this->valid= false;
        return nullptr;
    }
}
