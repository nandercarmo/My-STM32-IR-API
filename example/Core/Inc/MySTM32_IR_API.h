/*
 *  MySTM32_IR_API.h
 *  Universidade Federal de Minas Gerais
 *  Created on: Jan 2021
 *  Author: Nander Carmo, Marcos Ribeiro 
 *  Version: 1.0
 *  License: MIT
 *
 *  Esta API foi desenvolvida como trabalho da disciplina 
 *  de Programação de Sistemas Embarcados da UFMG 
 *  – Prof. Ricardo de Oliveira Duarte – Departamento de
 *  Engenharia Eletrônica
 */

#ifndef MY_STM32_IR_API_H
#define MY_STM32_IR_API_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32g4xx_hal.h"
#include "stm32g4xx_it.h"

#include "MySTM32_LG_Commands.h"

// Defines
#define TIMER_MULTIPLIER 10
#define DELTA_TIME 200
#define ALLOW_REPEAT_COMMAND 0

// Functions definitions
void My_IR_Init(TIM_HandleTypeDef * htim, GPIO_TypeDef * group, uint16_t pin);
void My_IR_Listening();
void My_IR_StartCommandReceived();
void My_IR_SameCommandReceived();
void My_IR_IdentifyPulse();
void My_IR_SetBitReceived(uint32_t bit);
void My_IR_GetIrCommandInHex(char * hexCommand);
void My_IR_GetIrCommandText(char * command);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim);

// Variable Definitions
TIM_HandleTypeDef * My_IR_Timer;
uint16_t My_IR_Pin;
GPIO_TypeDef * My_IR_PinPort;
uint8_t My_IR_IsReceiving;
uint8_t My_IR_ReceivedNewCommand;
uint32_t My_IR_Command;
uint8_t My_IR_BitsReceived;
uint32_t My_IR_ElapsedTimeHigh;
uint32_t My_IR_ElapsedTimeLow;
uint8_t My_IR_ReadValue;

#endif
