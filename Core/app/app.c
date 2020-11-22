#include "main.h"
#include "cmsis_os.h"

// starts from StartDefaultTask in freertos.c
// see task parameters in MX_FREERTOS_Init
void app() {
    while(1) {
        HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
        osDelay(100);
        HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);

        osDelay(400);
    }
}