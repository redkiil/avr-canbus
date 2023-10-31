
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "spi.h"
#include "canmodule.h"
#include "UART.h"

canframe canMsg1;


int main(void){
    
    DDRC |= PC4;
    
    spi_masterinit();
    _delay_ms(100);
    can_init();
    _delay_ms(100);
    UART_config();
    _delay_ms(100);
    while(1){
        
        if(can_read(&canMsg1) == ERROR_OK){
            unsigned char play[64];
            sprintf((char*)play, "%lx | %d | %x %x %x %x %x %x %x %x\n", canMsg1.id, canMsg1.dlc, canMsg1.can_data[0], canMsg1.can_data[1], canMsg1.can_data[2], canMsg1.can_data[3], canMsg1.can_data[4], canMsg1.can_data[5], canMsg1.can_data[6], canMsg1.can_data[7]);
            UART_sendString(play);
        }
        canMsg1.id  = 0x98FF1380;
        canMsg1.dlc = 8;
        canMsg1.can_data[0] = 0x8E;
        canMsg1.can_data[1] = 0x87;
        canMsg1.can_data[2] = 0x32;
        canMsg1.can_data[3] = 0xFA;
        canMsg1.can_data[4] = 0x26;
        canMsg1.can_data[5] = 0x8E;
        canMsg1.can_data[6] = 0xBE;
        canMsg1.can_data[7] = 0x86;
        can_send(canMsg1);
    }   
}
