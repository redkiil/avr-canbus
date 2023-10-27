/* 
 * File:   canmodule.h
 * Author: Augusto
 *
 * Created on October 23, 2023, 12:46 AM
 */

#ifndef CANMODULE_H
#define	CANMODULE_H

#include <avr/io.h>

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000UL /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000UL /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000UL /* error message frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFUL /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFUL /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFUL /* omit EFF, RTR, ERR flags */

#define CAN_125kbps 0x07       //
#define MCP_8MHz_125kBPS_CFG1 (0x01)
#define MCP_8MHz_125kBPS_CFG2 (0xB1)
#define MCP_8MHz_125kBPS_CFG3 (0x85)

#define TXB_ABTF 0x40
#define TXB_MLOA 0x20
#define TXB_TXERR 0x10
#define TXB_TXREQ 0x08
#define TXB_TXIE 0x04
#define TXB_TXP 0x03

#define STAT_RX0IF (1 << 0)
#define STAT_RX1IF (1 << 1)

#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN 8

#define TXB_EXIDE_MASK 0x08
#define DLC_MASK 0x0F
#define RTR_MASK 0x40
#define RXBnCTRL_RTR 0x08

#define ERROR_OK 0
#define ERROR_FAIL 1

static const uint8_t MCP_SIDH = 0;
static const uint8_t MCP_SIDL = 1;
static const uint8_t MCP_EID8 = 2;
static const uint8_t MCP_EID0 = 3;
static const uint8_t MCP_DLC  = 4;
static const uint8_t MCP_DATA = 5;



struct can_f
{
    uint32_t id;
    uint8_t dlc;
    uint8_t can_data[8];
};

typedef struct can_f canframe;

void can_init();
uint8_t can_send(canframe can_frame);
void can_prepare_id(uint8_t *candata, const uint8_t ext, const uint32_t id);
uint8_t can_read_message(uint8_t rxsdh, canframe *can_frame);
uint8_t can_read(canframe *can_frame);
#endif