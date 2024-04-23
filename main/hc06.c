#include "hc06.h"

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

char hc_05_06_check_version() {
	char version[5];
    char str[32];
    int i = 0;
    uart_puts(HC06_UART_ID, "AT+VERSION");
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';

    if (strstr(str, "linvorV1.8") > 0){
		version = "HC-06"
        return version;
	}
    else if (strstr(str, "VERSION:3.0-20170609") > 0){
		version = "HC-05"
        return version;
	}
	else
		return false
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

bool hc_05_06_init(char name[], char pin[]) {
	char version[5];
    hc06_set_at_mode(1);
	
    printf("check connection\n");
    while (hc06_check_connection() == false) {
        printf("not connected\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("Connected \n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	
	
	printf("check version\n");
	version = hc_05_06_check_version();
    if (hc_05_06_check_version() == false) {
        printf("unrecognized version\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
	else{
		printf("VERSION: %c",version);
	}
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	
	
	
    printf("set name\n");
    while (hc06_set_name(name) == false) {
        printf("set name failed\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }	
    printf("name ok\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	
	

    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("set pin\n");
    while (hc06_set_pin(pin) == false) {
        printf("set pin failed\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("pin ok\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
    hc06_set_at_mode(0);
}
