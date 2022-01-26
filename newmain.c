#include <xc.h>
#include "delays.h"

unsigned char j;

void main(void) {
    /*TRISB = 0b11110000; //RB5 to RB3 are connected to On/Off switches
    TRISD = 0b00000000; //RD7 to RD0 are connected to LEDs
    
    // LDR, light decrease resistance --> light, 5v
    while (1){
        if (PORTBbits.RB3 == 1)  {
            PORTD = 0xFF; //light up all the LEDs on the PIC18 board
            delay_ms(1000);
        } else {
            PORTD = 0x00;
            delay_ms(1000);
        }
    }
    return;*/
    
    TRISD = 0b00000000; // make PORT D an output port
while (1)
 {
 // the PORTD bits: --In3 In4 En2 In2 In1 En1
 PORTD= 0b00101011; // turns both motors
 delay_ms(1000);
 PORTDbits.RD5 = 0; PORTDbits.RD4 = 1; // reverses Motor 2
 delay_ms(1000);
 PORTDbits.RD0 = 0; // stops Motor1
 delay_ms(1000);
 PORTDbits.RD3 = 0; // stops Motor2
  delay_ms(1000);

}
    
    
}


