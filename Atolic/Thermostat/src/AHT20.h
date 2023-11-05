/*
 * AHT20.h
 *
 *  Created on: 3 но€б. 2023 г.
 *      Author: denlo
 */

#ifndef AHT20_H_
#define AHT20_H_

#include "constant.h"

#define AHT_ADDRESS 0x38
#define AHT_CMD_CONVERT_TEMPERATURE 0xAC

uint8_t AHT_state;
#define AHT_START_CONVERSATION 0
#define AHT_READ_CONVERSATION 1

void I2C_for_AHT_init();
void I2C_AHT_SendByte(uint8_t addr, uint8_t byte);
void I2C_AHT_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size);
void I2C_Get_Data(uint8_t addr, uint8_t *data, uint8_t size);
void AHT_measure_temperature();

#endif /* AHT20_H_ */
