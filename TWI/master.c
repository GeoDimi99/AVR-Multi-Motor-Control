#include <avr/io.h>
#include "TWI_lib.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "../avr_common/uart.h"
#include "../avr_common/uart1.h"

#define velocity_len 10
 
int main(void){
	UART_init();
	printf_init();
	sei();                                      //abilita le interruzioni globali
	TWI_Init();                                 //inizializza TWI
	
	//uint8_t buf[TRANSMIT_BUFLEN];
	uint8_t input[TRANSMIT_BUFLEN];
	uint8_t output[TRANSMIT_BUFLEN];	
	uint8_t current_velocity[velocity_len]= "piano";         //modificare e mettere lungezza generica
	uint8_t desired_velocity[velocity_len]= "forte";        //modificare e mettere lungezza generica
	uint8_t send_velocity[velocity_len + 1];
	uint8_t swap[velocity_len];
	
	uint8_t sample[] = {0x00, 's', 'a', 'm', 'p', 'l', 'e', '\0'};     //sample in broadcast
	uint8_t get[] = {(0x01 << 1) | 0x00, 'g', 'e', 't','\0'};          //get in unicast
	uint8_t set[] = {(0x01 << 1) | 0x00, 's', 'e', 't','\0'};          //set in unicast
	uint8_t apply[] = {0x00, 'a', 'p', 'p', 'l', 'y', '\0'};           //apply in broadcast
	send_velocity[0]= (0x01 << 1) | 0x00;

	do{
		        
		do { TWI_Transmit_Data(sample, 8, 0); } while(TWI_info.error_code != TWI_SUCCESS);        //invio comando "sample"
		                
		do { TWI_Transmit_Data(get, 5, 0); } while(TWI_info.error_code != TWI_SUCCESS);           //invio comando "get"
		
		do { TWI_Read_Data(0x01, velocity_len, 0); } while (TWI_info.error_code != TWI_SUCCESS);  //leggo la velocità corrente //modificare e mettere lungezza generica
		
		strcpy(swap, current_velocity); 
		
		strcpy(current_velocity, Receive_Buffer); 
		
				              
		do { TWI_Transmit_Data(set, 5, 0); } while (TWI_info.error_code != TWI_SUCCESS);       //invio il comando "set"
		
		strcpy(send_velocity + 1, desired_velocity);
		
		do {TWI_Transmit_Data(send_velocity, velocity_len+1, 0);} while (TWI_info.error_code != TWI_SUCCESS);       //invio la velocità desiderata
	
		do {TWI_Transmit_Data(apply, 7, 0);} while (TWI_info.error_code != TWI_SUCCESS);         //invio il comando "apply"
		
		strcpy(current_velocity, desired_velocity);
		
		printf("%s\n", current_velocity);
		
		strcpy(desired_velocity, swap);
			
		//UART_putString( strcat(strcat("current velocity: ", current_velocity), "\n")); 
		
		//UART_getString(input);
		
		//strcpy(desired_velocity, input); 
		
	}while(1); 
	
	return 0;

}
