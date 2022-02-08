#include <xc.h>
#include "delays.h"

unsigned char j;

void main(void) {
    TRISB = 0b11111111;     // RB5 to RB3 are connected to On/Off switches
    TRISD = 0b00000000;     // RD7 to RD0 are connected to LEDs
    INTCONbits.GIE = 1;     // Enable global interrupt
    INTCONbits.INT0IE = 1;  // Enable int for RB0
    
    while(1) {
        if (isDark() && isWet())
            comeback();
    }
    
    return;
    
    
}
/* This function turns off LEDs at PORTD if the LDR detects light (use phone flashlight)
 * When LDR reads 0, it means it is bright (there is light)
 * When LDR reads 1, it means it is dim
 */
int isDark(){              
        if (PORTBbits.RB3 == 1) // 1
            return 1;
    return 0;
}

/* RB5 is for Moisture sensor
    * When moisture sensor is dry, it reads 0
    * When moisture sensor is wet, it reads 1
    * PORTBbits.RB5 
 */
int isWet(){         //This code lights up all LEDs at PORTD if the moisture sensor reads 1 (wet)
        if(PORTBbits.RB5 == 1)
            return 1;
    return 0;
}

int comeback(){
    while (1){
            PORTD= 0b00101011; // turns both motors
            delay_ms(5000);
            PORTDbits.RD5 = 0; PORTDbits.RD4 = 1; // reverses Motor 2
            delay_ms(5000);
            PORTDbits.RD0 = 0; // stops Motor1
            delay_ms(5000);
            PORTDbits.RD3 = 0; // stops Motor2
            delay_ms(5000);
    }
}


void interrupt overrideButton_isr(void) {
    INTCONbits.INT0IF = 0; //clear flag
    PORTDbits.RD5 = 0; //
    PORTDbits.RD4 = 1;
    PORTDbits.RD1 = 0; 
    PORTDbits.RD2 = 1;
    delay_ms(5000);
    PORTDbits.RD0 =0;
    PORTDbits.RD3 =0;
}