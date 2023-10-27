#define F_CPU 16000000UL
#include "canmodule.h"
#include "mcp2515_defs.h"
#include "spi.h"
#include <util/delay.h>
#include <string.h>

uint8_t dummy;

void can_init(){
    spi_select();
    spi_write(0xC0);
    spi_deselect();
    _delay_ms(10);
    
    uint8_t zeros[14];
    memset(zeros, 0,sizeof(zeros));
    spi_writeregisters(TXB0CTRL, zeros, 14);
    spi_writeregisters(TXB1CTRL, zeros, 14);
    spi_writeregisters(TXB2CTRL, zeros, 14);
    
    
    spi_writeregister(RXB0CTRL, 0);
    spi_writeregister(RXB1CTRL, 0);

    spi_writeregister(CANINTE, RX0IF | RX1IF | ERRIF | MERRF);
    
    
    //
    spi_bitmodify(RXB0CTRL,0x67,0x04);
    spi_bitmodify(RXB1CTRL,0x67,0x01);
    //
    
    
    spi_writeregister(CNF1,MCP_8MHz_125kBPS_CFG1);

    //Set CNF2
    spi_writeregister(CNF2, MCP_8MHz_125kBPS_CFG2);

    //Set CNF3
    spi_writeregister(CNF3, MCP_8MHz_125kBPS_CFG3);
    
    spi_writeregister(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);
  

    _delay_ms(100);
  //Verify device entered Normal mode
    dummy = spi_readregister(CANSTAT);
    if (OPMODE_NORMAL != (dummy && 0xE0))
        spi_writeregister(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);
    _delay_ms(100);
}
uint8_t can_send(canframe can_frame){

    uint8_t ext = (can_frame.id & CAN_EFF_FLAG);
    uint8_t rtr = (can_frame.id & CAN_RTR_FLAG);
    uint32_t id = (can_frame.id & (ext ? CAN_EFF_MASK : CAN_SFF_MASK));
    uint8_t data[13];
    can_prepare_id(data, ext, id);
    data[MCP_DLC] = rtr ? (can_frame.dlc | RTR_MASK) : can_frame.dlc;
    
    memcpy(&data[MCP_DATA], can_frame.can_data, can_frame.dlc);
    
    spi_writeregisters(TXB0SIDH, data,5 + can_frame.dlc);
    
    spi_bitmodify(TXB0CTRL, TXB_TXREQ, TXB_TXREQ);
    uint8_t ctrl = spi_readregister(TXB0CTRL);
    if ((ctrl & (TXB_ABTF | TXB_MLOA | TXB_TXERR)) != 0) {
        return 1;
    }
    return 0;
}
void can_prepare_id(uint8_t *candata, const uint8_t ext, const uint32_t id)
{
    uint16_t canid = (uint16_t)(id & 0x0FFFF);

    if (ext) {
        candata[MCP_EID0] = (uint8_t) (canid & 0xFF);
        candata[MCP_EID8] = (uint8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        candata[MCP_SIDL] = (uint8_t) (canid & 0x03);//0x01
        candata[MCP_SIDL] += (uint8_t) ((canid & 0x1C) << 3);
        candata[MCP_SIDL] |= TXB_EXIDE_MASK;
        candata[MCP_SIDH] = (uint8_t) (canid >> 5);
    } else {
        candata[MCP_SIDH] = (uint8_t) (canid >> 3);//0000 0111 
        candata[MCP_SIDL] = (uint8_t) ((canid & 0x07 ) << 5);//10000000
        candata[MCP_EID0] = 0;
        candata[MCP_EID8] = 0;
    }
}
uint8_t can_read_message(uint8_t rxsdh,canframe *can_frame)
{
    uint8_t tbufdata[5];
    spi_readregisters(rxsdh, tbufdata, 5);
    uint32_t id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & TXB_EXIDE_MASK) ==  TXB_EXIDE_MASK ) {
        id = (id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        id = (id<<8) + tbufdata[MCP_EID8];
        id = (id<<8) + tbufdata[MCP_EID0];
        id |= CAN_EFF_FLAG;
    }
    uint8_t dlc = (tbufdata[MCP_DLC] & DLC_MASK);
    if (dlc > CAN_MAX_DLEN) {
        return ERROR_FAIL;
    }
    uint8_t ctrlreg = (rxsdh ^ RXB0SIDH);
    uint8_t ctrl = spi_readregister((ctrlreg ? RXB1CTRL : RXB0CTRL));
    uint8_t data = (ctrlreg ? RXB1D0 : RXB0D0);
    uint8_t interupt = (ctrlreg ? RX1IF : RX0IF);
    if (ctrl & RXBnCTRL_RTR) {
        id |= CAN_RTR_FLAG;
    }

    can_frame->id = id;
    can_frame->dlc = dlc;

    spi_readregisters(data, can_frame->can_data, dlc);

    spi_bitmodify(CANINTF, interupt, 0);
    return ERROR_OK;
}
uint8_t can_read(canframe *can_frame)
{
    uint8_t st = spi_readstatus();
    if(st & STAT_RX0IF){
        can_read_message(RXB0SIDH, can_frame);
    }else if(st & STAT_RX1IF){
        can_read_message(RXB1SIDH, can_frame);
    }
    return 0;
}