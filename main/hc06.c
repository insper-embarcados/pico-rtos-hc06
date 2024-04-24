#include "hc06.h"

bool hc06_check_connection() {
    vTaskDelay(pdMS_TO_TICKS(1000));
    char str[32];
    int i = 0;
    uart_puts(HC06_UART_ID, "AT\r\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    uart_puts(HC06_UART_ID, "AT\r\n");
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';
    
    if (strstr(str, "OK") > 0)
        return true;

    vTaskDelay(pdMS_TO_TICKS(1000));
        
    char str2[32];
    int j = 0;
    uart_puts(HC06_UART_ID, "AT");
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str2[j++] = uart_getc(HC06_UART_ID);
    }
    str2[j] = '\0';
    //for(int k=0;k<=j;k++) printf("%c",str2[k]);
    //printf("\n\n\n");
    if (strstr(str2, "OK") > 0)
        return true;

    else
        return false;
}

char* hc_05_06_check_version() {
    vTaskDelay(pdMS_TO_TICKS(1000));
    char str[32];
    int i = 0;
    uart_puts(HC06_UART_ID, "AT+VERSION");
    
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[i++] = uart_getc(HC06_UART_ID);
    }
    str[i] = '\0';
    //for(int j=0;j<=i;j++) printf("%c",str[j]);



    //printf("\n\n\n");
    
    if (strstr(str, "OKlinvorV1.8") != NULL) {
        //printf("HC-06");
        return "HC-06";
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
    char str2[32];
    int j = 0;
    uart_puts(HC06_UART_ID, "AT+VERSION\r\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    uart_puts(HC06_UART_ID, "AT+VERSION\r\n");
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[j++] = uart_getc(HC06_UART_ID);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (uart_is_readable_within_us(HC06_UART_ID, 1000)) {
        str[j++] = uart_getc(HC06_UART_ID);
    }
    str[j] = '\0';


    if (strstr(str, "VERSION:3.0-20170609") != NULL) {
        //printf("HC-05");
        return "HC-05";
    }
    else {
        return NULL;
    }
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
	
    hc06_set_at_mode(1);
	
    printf("check connection\n");
    while (hc06_check_connection() == false) {
        printf("not connected\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("Connected \n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	
	
	printf("check version\n");
	char* version = hc_05_06_check_version();

    if (version != NULL) {
        printf("VERSION>>> %s\n", version);
    } else {
        printf("unrecognized version\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
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