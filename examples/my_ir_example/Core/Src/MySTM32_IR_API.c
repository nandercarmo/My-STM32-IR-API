/*
 *  MySTM32_IR_API.c
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

#include "MySTM32_IR_API.h"

void My_IR_Init(TIM_HandleTypeDef * htim, UART_HandleTypeDef * huart, GPIO_TypeDef * group, uint16_t pin) {
	
	My_IR_Timer = htim;
	My_IR_Uart = huart;
	My_IR_PinPort = group;
	My_IR_Pin = pin;

	My_IR_ReadValue = 0;
	My_IR_ReceivedNewCommand = 0;
	My_IR_IsReceiving = 0;

	HAL_TIM_Base_Start_IT(My_IR_Timer);
}

void My_IR_Listening() {
	
	uint8_t newBit = HAL_GPIO_ReadPin(My_IR_PinPort, My_IR_Pin) == GPIO_PIN_SET ? 0 : 1;
	
	if (newBit != My_IR_ReadValue) {

		if (newBit == 1) {
			
			if (My_IR_IsReceiving) {

				My_IR_ElapsedTimeLow = __HAL_TIM_GET_COUNTER(My_IR_Timer) * TIMER_MULTIPLIER;
				__HAL_TIM_SET_COUNTER(My_IR_Timer, 0);

				My_IR_IdentifyPulse();
				
			} else {
				
				__HAL_TIM_SET_COUNTER(My_IR_Timer, 0);
				My_IR_IsReceiving = 1;
				bits = 0;
			}

		} else {
			
			My_IR_ElapsedTimeHigh = __HAL_TIM_GET_COUNTER(My_IR_Timer) * TIMER_MULTIPLIER;
			__HAL_TIM_SET_COUNTER(My_IR_Timer, 0);
		}

		My_IR_ReadValue = newBit;
	}
}

void My_IR_IdentifyPulse() {

	if (My_IR_ElapsedTimeHigh >= 6800 - DELTA_TIME && My_IR_ElapsedTimeHigh <= 6800 + DELTA_TIME && My_IR_ElapsedTimeLow >= 4500 - DELTA_TIME && My_IR_ElapsedTimeLow <= 4500 + DELTA_TIME) My_IR_StartCommandReceived();
	else if (My_IR_ElapsedTimeHigh >= 6800 - DELTA_TIME && My_IR_ElapsedTimeHigh <= 6800 + DELTA_TIME && My_IR_ElapsedTimeLow >= 2200 - DELTA_TIME && My_IR_ElapsedTimeLow <= 2200 + DELTA_TIME) My_IR_SameCommandReceived();
	else if (My_IR_ElapsedTimeHigh >= 500 - DELTA_TIME && My_IR_ElapsedTimeHigh <= 500 + DELTA_TIME && My_IR_ElapsedTimeLow >= 600 - DELTA_TIME && My_IR_ElapsedTimeLow <= 600 + DELTA_TIME) My_IR_SetBitReceived(0);
	else if (My_IR_ElapsedTimeHigh >= 500 - DELTA_TIME && My_IR_ElapsedTimeHigh <= 500 + DELTA_TIME && My_IR_ElapsedTimeLow >= 1700 - DELTA_TIME && My_IR_ElapsedTimeLow <= 1700 + DELTA_TIME) My_IR_SetBitReceived(1);
}

void My_IR_StartCommandReceived() {
	
	My_IR_BitsReceived = 0;
	My_IR_Command = 0;
	My_IR_ReceivedNewCommand = 0;
}

void My_IR_SameCommandReceived() {
	
	My_IR_ReceivedNewCommand = 1;
	My_IR_IsReceiving = 0;
}

void My_IR_SetBitReceived(uint32_t bit) {
	
	if (My_IR_IsReceiving) {
		
		bit <<= My_IR_BitsReceived; // Desloca o bit recebido até a posição correta que ele deve ser inserido
		My_IR_Command |= bit; // Adiciona o bit a My_IR_Command
		My_IR_BitsReceived++; // Incrementa a quantidade de bits recebidos
	}
	
	if (My_IR_BitsReceived == 32) {
		
		My_IR_IsReceiving = 0;
		My_IR_ReceivedNewCommand = 1;
	}
}

void My_IR_GetIrCommandInHex(char * hexCommand) {
	
	sprintf(hexCommand, "0x%08x", (unsigned int) My_IR_Command);
}

void My_IR_GetIrCommandText(char * command) {

	switch(My_IR_Command) {

		case COMMAND_ON_OFF: sprintf(command, "COMMAND_ON_OFF"); break;
		case COMMAND_CHANNEL_1: sprintf(command, "COMMAND_CHANNEL_1"); break;
		case COMMAND_CHANNEL_2: sprintf(command, "COMMAND_CHANNEL_2"); break;
		case COMMAND_CHANNEL_3: sprintf(command, "COMMAND_CHANNEL_3"); break;
		case COMMAND_CHANNEL_4: sprintf(command, "COMMAND_CHANNEL_4"); break;
		case COMMAND_CHANNEL_5: sprintf(command, "COMMAND_CHANNEL_5"); break;
		case COMMAND_CHANNEL_6: sprintf(command, "COMMAND_CHANNEL_6"); break;
		case COMMAND_CHANNEL_7: sprintf(command, "COMMAND_CHANNEL_7"); break;
		case COMMAND_CHANNEL_8: sprintf(command, "COMMAND_CHANNEL_8"); break;
		case COMMAND_CHANNEL_9: sprintf(command, "COMMAND_CHANNEL_9"); break;
		case COMMAND_CHANNEL_ADD: sprintf(command, "COMMAND_CHANNEL_ADD"); break;
		case COMMAND_CHANNEL_SUB: sprintf(command, "COMMAND_CHANNEL_SUB"); break;
		case COMMAND_VOL_ADD: sprintf(command, "COMMAND_VOL_ADD"); break;
		case COMMAND_VOL_SUB: sprintf(command, "COMMAND_VOL_SUB"); break;
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) {
	
	if (htim == My_IR_Timer) {
		
//		char message[] = "x\n";
//		HAL_UART_Transmit(My_IR_Uart, (uint8_t *) message, strlen(message), 1000);

		if (My_IR_IsReceiving) My_IR_IsReceiving = 0;
		else __NOP();
	}
}
