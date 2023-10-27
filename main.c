
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "canmodule.h"

int main(void){
    
    DDRC |= PC4;
    
    spi_masterinit();
    _delay_ms(100);
    can_init();
    
    while(1){
        
        
    }   
}
