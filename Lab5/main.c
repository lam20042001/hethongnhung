#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define TIMER0_ID  9
#define TIMER1_ID 29

TimerHandle_t xTimer0, xTimer1;

const uint32_t ulMaxExpiryCountBeforeStoppingTIMER0 = 15;
const uint32_t ulMaxExpiryCountBeforeStoppingTIMER1 =  5;

uint32_t ulCount0, ulCount1;

void vTimerCallback(TimerHandle_t xTimer) {
    uint32_t xTimerID;
    xTimerID  = (uint32_t)pvTimerGetTimerID(xTimer);
    if (xTimerID == TIMER0_ID) {
        ulCount0++;
        if (ulCount0 >= ulMaxExpiryCountBeforeStoppingTIMER0) {
            xTimerStop(xTimer, 0);
        }

        printf("%dms\t- TIMER0: ahihi\n", pdTICKS_TO_MS(xTaskGetTickCount()));
        fflush(stdout);
    } else if (xTimerID == TIMER1_ID) {
        ulCount1++;
        if (ulCount1 >= ulMaxExpiryCountBeforeStoppingTIMER1) {
            xTimerStop(xTimer, 0);
        }

        printf("%dms\t- TIMER1: ihaha\n", pdTICKS_TO_MS(xTaskGetTickCount()));
        fflush(stdout);
    } else {
        printf("Handle...\n");
    }
}


void app_main(void)
{
    xTimer0 = xTimerCreate("Timer0", pdMS_TO_TICKS(2000), pdTRUE, (void *)TIMER0_ID, vTimerCallback);
    xTimer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(3000), pdTRUE, (void *)TIMER1_ID, vTimerCallback);
    if (xTimerStart(xTimer0, 0) != pdPASS) {
        printf("Failed to start TIMER0...");
    }
    if (xTimerStart(xTimer1, 0) != pdPASS) {
        printf("Failed to start TIMER1...");
    }
}