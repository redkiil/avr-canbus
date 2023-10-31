#include "spi.h"
#include <avr/io.h>
#include "mcp2515_defs.h"

void spi_select(){
    SPI_PORT &= ~(1<<SPI_SS);
}
void spi_deselect(){
    SPI_PORT |= (1<<SPI_SS);
}
void spi_masterinit()
{
    SPI_DDR |= (1<<SPI_MOSI) | (1<<SPI_CLOCK) | (1<<SPI_SS);
    SPI_PORT |= (1<<SPI_SS);
    SPCR = (1<<SPE) | (1<<MSTR);
}
uint8_t spi_write(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}
void spi_writeregister(uint8_t reg, uint8_t data)
{
    spi_select();
    spi_write(CAN_WRITE);
    spi_write(reg);
    spi_write(data);
    spi_deselect();
}
void spi_writeregisters(uint8_t reg, const uint8_t values[], const uint8_t n)
{
    spi_select();
    spi_write(CAN_WRITE);
    spi_write(reg);
    for (uint8_t i=0; i<n; i++) {
        spi_write(values[i]);
    }
    spi_deselect();
}
uint8_t spi_readregister(uint8_t reg)
{
    uint8_t d = 0xFF;
    spi_select();
    spi_write(CAN_READ);
    spi_write(reg);
    d = spi_write(0xFF);
    spi_deselect();
    return d;
}
void spi_readregisters(uint8_t reg, uint8_t values[],uint8_t size)
{
    spi_select();
    spi_write(CAN_READ);
    spi_write(reg);
    for(uint8_t i = 0; i < size; i++){
        values[i] = spi_write(0xFF);
    }
    spi_deselect();
}
void spi_bitmodify(uint8_t reg, uint8_t mask, uint8_t data)
{
    spi_select();
    spi_write(CAN_BIT_MODIFY);
    spi_write(reg);
    spi_write(mask);
    spi_write(data);
    spi_deselect();
}
uint8_t spi_readstatus()
{
    spi_select();
    spi_write(CAN_RD_STATUS);
    uint8_t rd = spi_write(0x00);
    spi_deselect();
    return rd;
}