#ifndef TWILIB_H_
#define TWILIB_H_

#define TWI_FREQ 100000 //frequenza SCL TWI
#define TWI_STATUS	(TWSR & 0xF8) //prende dal 3° al 7° bit dello stato

//buffer globali di trasmissione e ricezione
#define TRANSMIT_BUFLEN 20
#define RECEIVE_BUFLEN 20
uint8_t Transmit_Buffer[TRANSMIT_BUFLEN];
volatile uint8_t Receive_Buffer[RECEIVE_BUFLEN];     //volatile-->può cambiare ad ogni istante
volatile int TB_Index;                               //indicizza il buffer di trasmissione--> può cambiare ad ogni istante
int RB_Index;                                        //indicizza il buffer di ricezione
int transmit_len;                                    //Lunghezza della trasmissione (minore di TRANSMIT_BUFLEN)
int receive_len;                                     //Lunghezza della ricezione (minore di RECEIVE_BUFLEN)

//modalità operative TWI
typedef enum {
	Ready,
	Initializing,
	Repeated_Start,
	Master_Transmitter,
	Master_Receiver,
	Slave_Transmitter,
	Slave_Reciever
} TWI_mode;

//struttura dati info TWI

typedef struct TWI_info{
	TWI_mode mode;
	uint8_t error_code;
	uint8_t repeated_start;
}TWI_info;

//macros per il controllo

#define TWI_Send_Start() (TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE))

/*
 * TWI_Send_Start() [TWCR=10100101]:
 *  [7] TWINT=1 si è finito di lavorare in background e ci si aspetta un'interrupt
 *  [6] TWEA=0 disabilitato il controllo per l'ACK
 *  [5] TWSTA=1 si vuole trasmettere una condizione di START
 *  [4] TWSTO=0 NON si vuole trasmettere una condizione di STOP
 *  [3] TWWC=0 non è avvenuto un tentativo illegale di scrittura (write collision)
 *  [2] TWEN=1 abilita TWI, che prende il controllo dei pins SCL ed SDA
 *  [1] 0 bit inutilizzato
 *  [0] TWIE=1 abilita le interruzioni e la loro gestione passando per l'interrupt vector
*/

#define TWI_Send_Stop() (TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE))

/*
 * TWI_Send_Stop() [TWCR=10010101]:
 *  [7] TWINT=1 si è finito di lavorare in background e ci si aspetta un'interrupt
 *  [6] TWEA=0 disabilitato il controllo per l'ACK
 *  [5] TWSTA=0 NON si vuole trasmettere una condizione di START
 *  [4] TWSTO=1 si vuole trasmettere una condizione di STOP
 *  [3] TWWC=0 non è avvenuto un tentativo illegale di scrittura (write collision)
 *  [2] TWEN=1 abilita TWI, che prende il controllo dei pins SCL ed SDA
 *  [1] 0 bit inutilizzato
 *  [0] TWIE=1 abilita le interruzioni e la loro gestione passando per l'interrupt vector
*/

#define TWI_Send_Transmit() (TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE))

/*
 * TWI_Send_Transmit() [TWCR=10000101]:
 *  [7] TWINT=1 si è finito di lavorare in background e ci si aspetta un'interrupt
 *  [6] TWEA=0 disabilitato il controllo per l'ACK
 *  [5] TWSTA=0 NON si vuole trasmettere una condizione di START
 *  [4] TWSTO=0 NON vuole trasmettere una condizione di STOP
 *  [3] TWWC=0 non è avvenuto un tentativo illegale di scrittura (write collision)
 *  [2] TWEN=1 abilita TWI, che prende il controllo dei pins SCL ed SDA
 *  [1] 0 bit inutilizzato
 *  [0] TWIE=1 abilita le interruzioni e la loro gestione passando per l'interrupt vector
*/

#define TWI_Send_ACK()  (TWCR = (1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE))

/*
 * TWI_Send_ACK() [TWCR=11000101]:
 *  [7] TWINT=1 si è finito di lavorare in background e ci si aspetta un'interrupt
 *  [6] TWEA=1 abilitato il controllo per l'ACK
 *  [5] TWSTA=0 NON si vuole trasmettere una condizione di START
 *  [4] TWSTO=0 NON vuole trasmettere una condizione di STOP
 *  [3] TWWC=0 non è avvenuto un tentativo illegale di scrittura (write collision)
 *  [2] TWEN=1 abilita TWI, che prende il controllo dei pins SCL ed SDA
 *  [1] 0 bit inutilizzato
 *  [0] TWIE=1 abilita le interruzioni e la loro gestione passando per l'interrupt vector
*/

#define TWI_Send_NACK() (TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE))

/*
 * TWI_Send_NACK() [TWCR=10000101]:
 *  [7] TWINT=1 si è finito di lavorare in background e ci si aspetta un'interrupt
 *  [6] TWEA=0 disabilitato il controllo per l'ACK
 *  [5] TWSTA=0 NON si vuole trasmettere una condizione di START
 *  [4] TWSTO=0 NON vuole trasmettere una condizione di STOP
 *  [3] TWWC=0 non è avvenuto un tentativo illegale di scrittura (write collision)
 *  [2] TWEN=1 abilita TWI, che prende il controllo dei pins SCL ed SDA
 *  [1] 0 bit inutilizzato
 *  [0] TWIE=1 abilita le interruzioni e la loro gestione passando per l'interrupt vector
*/

//dichiarazione delle funzioni
uint8_t is_TWI_ready(void);
void TWI_Init(void);
uint8_t TWI_Transmit_Data(void *const TR_data, uint8_t data_len, uint8_t repeated_start);
uint8_t TWI_Read_Data(uint8_t TWI_addr, uint8_t bytes_to_read, uint8_t repeated_start);

#endif

