/**
 * Based on https://github.com/wdomski/SX1278
 * and https://github.com/wdomski/SX1278-example
 * 
 * Dmitriy Isaev or b4cksp4ce-aerospace program
 * issaev.dmitriy@gmail.com
 */


#include "main.h"
#include "cmsis_os.h"
#include "SX1278.h"

// starts from StartDefaultTask in freertos.c
// see task parameters in MX_FREERTOS_Init
#define MODE_Pin            GPIO_PIN_15
#define MODE_GPIO_Port      GPIOB

#define NSS_Pin             GPIO_PIN_4
#define NSS_GPIO_Port       GPIOA

#define LED_Pin             GPIO_PIN_13
#define LED_GPIO_Port       GPIOC

#define DIO0_Pin            GPIO_PIN_0
#define DIO0_GPIO_Port      GPIOA

#define RESET_Pin           GPIO_PIN_1
#define RESET_GPIO_Port     GPIOA


// UART tx 		PA2
// SPI MOSI 	PA7
// SPI MISO		PA6
// SPI CS		PA4
// SPI CLK 		PA5



extern SPI_HandleTypeDef hspi1;

SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

int ret;

char buffer[512];

uint8_t message_id = 0;
int message_length;

void loraTest() {

    printf("Entering LoRa test\r\n");

	bool master = HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin);

	if (master) {
		printf("Mode: Master\r\n");
	} else {
		printf("Mode: Slave\r\n");
	}

	//initialize LoRa module
	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi1;
	SX1278.hw = &SX1278_hw;

	while (1) {
		// change LORA param
		char* message = NULL;

		if(true) {
			message = "#S5AC3";
		} else {
			message = "#S5AC3-1x2x3x4x5x6x7x8x9x0y1y2y3y4y5y6";
		}

		if (master) {
			printf("conf...");
			SX1278_init(&SX1278,
				434000000,
				SX1278_POWER_20DBM,
				SX1278_LORA_SF_7,
				SX1278_LORA_BW_125KHZ,
				SX1278_LORA_CR_4_5,
				SX1278_LORA_CRC_EN,
				10
			);
			printf("done. ");


			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

			message_length = sprintf(buffer, "%c%s", message_id, message);
			ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
			printf("Entry: %d. id: %d, length: %d ", ret, message_id, message_length);

			ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t*) buffer,
					message_length, 2000);
			message_id++;

			printf("Transmission: %s.\r\n", ret ? "done":"timeout") ;
			osDelay(10);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

			osDelay(10);
		} else {
			printf("Slave ...\r\n");
			HAL_Delay(800);
			printf("Receiving package...\r\n");

			ret = SX1278_LoRaRxPacket(&SX1278);
			printf("Received: %s\r\n", ret ? "done":"timeout");
			if (ret > 0) {
				SX1278_read(&SX1278, (uint8_t*) buffer, ret);
				printf("Content (%d): %s\r\n", ret, buffer);
			}
			printf("Package received ...\r\n");
		}

		//change mode
		/*
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin)) {
			printf("Changing mode\r\n");
			master = ~master & 0x01;
			if (master == 1) {
				ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			} else {
				ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			}
			HAL_Delay(1000);
			while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin));
		}
		*/
	}
}



