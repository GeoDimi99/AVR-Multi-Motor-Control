
// digit_init: inizializzazione di pin di output (pin 2 e 3).
void digit_init(void);

// digit_write: scrittura del pin:
// - pin_mask : pin di output
// - pin_value: valore 0 o 1 da stampare 
void digit_write(uint8_t pin_mask ,uint8_t pin_value);
