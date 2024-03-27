/*
 * LED blink with FreeRTOS
 */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <string.h>

#include "pico/stdlib.h"
#include <stdio.h>

#define HC06_UART_ID uart1
#define HC06_BAUD_RATE 9600
#define HC06_TX_PIN 4
#define HC06_RX_PIN 5
#define HC06_PIN 6

bool hc06_check_connection();
bool hc06_set_name(char name[]);
bool hc06_set_pin(char pin[]);
bool hc06_set_at_mode(int on);
bool hc06_init(char name[], char pin[]);

bool hc06_check_connection() {
    char str[32];
    int i = 0;
    uart_puts(HC06_UART_ID, "AT");
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';

    if (strstr(str, "OK") > 0)
        return true;
    else
        return false;
}

bool hc06_set_name(char name[]) {
    char str[32];
    int i = 0;

    sprintf(str, "AT+NAME%s", name);
    uart_puts(HC06_UART_ID, str);
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';

    if (strstr(str, "OK") > 0)
        return true;
    else
        return false;
}

bool hc06_set_pin(char pin[]) {
    char str[32];
    int i = 0;

    sprintf(str, "AT+PIN%s", pin);
    uart_puts(HC06_UART_ID, str);
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';

    if (strstr(str, "OK") > 0)
        return true;
    else
        return false;
}

bool hc06_set_at_mode(int on){
    gpio_put(HC06_PIN, on);
}

bool hc06_init(char name[], char pin[]) {
    hc06_set_at_mode(1);
    printf("check connection\n");
    while (hc06_check_connection() == false) {
        printf("not connected\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("Connected \n");

    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("set name\n");
    while (hc06_set_name(name) == false) {
        printf("set name failed\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("name ok\n");

    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("set pin\n");
    while (hc06_set_pin(pin) == false) {
        printf("set pin failed\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("pin ok\n");
    hc06_set_at_mode(0);
}

void hc06_task(void *p) {
    uart_init(HC06_UART_ID, HC06_BAUD_RATE);
    gpio_set_function(HC06_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(HC06_RX_PIN, GPIO_FUNC_UART);
    hc06_init("aps2_legal", "1234");

    int i = 0;
    while (true) {
        uart_puts(HC06_UART_ID, "OLAAA ");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() {
    stdio_init_all();

    printf("Start bluetooth task\n");

    xTaskCreate(hc06_task, "UART_Task 1", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
