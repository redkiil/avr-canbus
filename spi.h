/* 
 * File:   spi.h
 * Author: Augusto
 *
 * Created on October 23, 2023, 12:46 AM
 */

#ifndef SPI_H
#define	SPI_H

#include <avr/io.h>

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_MOSI PB3
#define SPI_MISO PB4
#define SPI_CLOCK PB5
#define SPI_SS PB2

void spi_deselect();
void spi_select();
void spi_bitmodify(uint8_t reg, uint8_t mask, uint8_t data);
uint8_t spi_readregister(uint8_t reg);
void spi_readregisters(uint8_t reg, uint8_t values[],uint8_t size);
void spi_writeregisters(uint8_t reg, const uint8_t values[], const uint8_t n);
void spi_writeregister(uint8_t reg, uint8_t data);
uint8_t spi_write(uint8_t data);
void spi_masterinit();
uint8_t spi_readstatus();


#endif