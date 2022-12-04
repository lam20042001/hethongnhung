#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define QUEUE_SIZE 10

#define TOTAL_TASK  3
#define TASKA_ID    0
#define TASKB_ID    1
#define TASKC_ID    2
#define DUMB_ID     9

struct Request {
    int  taskID;
    char data[20];
};

QueueHandle_t xQueue;
bool isRead[TOTAL_TASK];

int genTaskID() {
    int tmp = rand() % 10;
    if (tmp > 8) 
        return DUMB_ID;
    else 
        return rand() % TOTAL_TASK;
}

void clearFlag() {
    for (int i = 0; i < TOTAL_TASK; ++i) {
        isRead[i] = false;
    }
}

bool isAllRead() {
    bool result = true;
    for (int i = 0; i < TOTAL_TASK; ++i) {
        result = result && isRead[i];
    }
    return result;
}

void printQueueInfo(QueueHandle_t xQueue) {
    printf("-----------------\n");
    printf("QUEUE INFORMATION\n");
    printf("Number of messages in queue   : %d\n", uxQueueMessagesWaiting(xQueue));
    printf("Number of free spaces in queue: %d\n", uxQueueSpacesAvailable(xQueue));
    printf("-----------------\n");
}

void ReceptionTask(void *pvParameter)
{
    struct Request xRequest;
    while(1)
	{
        xRequest.taskID = genTaskID();
        if(xQueueSendToBack(xQueue, &xRequest, (TickType_t)10) != pdPASS) {
            printf("ReceptionTask: Send failed!!!\n");
        } else {
            printf("ReceptionTask: Send ID %d\n",xRequest.taskID);
        }
        // printQueueInfo(xQueue);
        vTaskDelay(1000 / portTICK_RATE_MS);
	}
}   

void FunctionalTaskA(void *pvParameter)
{
    struct Request xRequest;
    while(1)
	{
        if(xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS) {
            printf("FunctionalTaskA: Receive failed!!!\n");
        } else {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASKA_ID) {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearFlag();
                // HANDLE
                printf("FunctionalTaskA: Handle\n");
            } else {
                isRead[TASKA_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
	}
}   

void FunctionalTaskB(void *pvParameter)
{
    struct Request xRequest;
    while(1)
	{
        if(xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS) {
            printf("FunctionalTaskB: Receive failed!!!\n");
        } else {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASKB_ID) {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearFlag();
                // HANDLE
                printf("FunctionalTaskB: Handle\n");
            } else {
                isRead[TASKB_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
	}
}  

void FunctionalTaskC(void *pvParameter)
{
    struct Request xRequest;
    while(1)
	{
        if(xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS) {
            printf("FunctionalTaskC: Receive failed!!!\n");
        } else {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASKC_ID) {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearFlag();
                // HANDLE
                printf("FunctionalTaskC: Handle\n");
            } else {
                isRead[TASKC_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
	}
}  

void ErrorHandlerTask(void *pvParameter)
{
    struct Request xRequest;
    while(1)
	{
        if(xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS) {
            printf("ErrorHandlerTask: Receive failed!!!\n");
        } else {
            // printf("Receive ID: %d\n",xRequest.taskID);
            if (isAllRead()) {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                // HANDLE
                printf("ErrorHandlerTask: Handle\n");
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
	}
}   


void app_main(void)
{
    xQueue = xQueueCreate(QUEUE_SIZE, sizeof(struct Request *));
    clearFlag();

    xTaskCreate(&ReceptionTask, "ReceptionTask", 2048, NULL, 1, NULL);
    xTaskCreate(&FunctionalTaskA, "FunctionalTaskA", 2048, NULL, 1, NULL);
    xTaskCreate(&FunctionalTaskB, "FunctionalTaskB", 2048, NULL, 1, NULL);
    xTaskCreate(&FunctionalTaskC, "FunctionalTaskC", 2048, NULL, 1, NULL);
    xTaskCreate(&ErrorHandlerTask, "ErrorHandlerTask", 2048, NULL, 1, NULL);
}