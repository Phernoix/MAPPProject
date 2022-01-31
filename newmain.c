#include <xc.h>
#include "delays.h"

unsigned char j;

void main(void) {
    TRISB = 0b11111111; //RB5 to RB3 are connected to On/Off switches
    TRISD = 0b00000000; //RD7 to RD0 are connected to LEDs
    INTCONbits.GIE = 1; //enable global interrupt (master switch)
    INTCONbits.INT0IE = 1; //interrupt enable for RB0
    comeback();
    
    return;
    
    
}
//This function turns off LEDs at PORTD if the LDR detects light (use phone flashlight)
//When LDR reads 0, it means it is bright (there is light)
//When LDR reads 1, it means it is dim


int ldrTest(){              
    while (1){
        if (PORTBbits.RB3 == 1)  { // 1
            PORTD = 0xFF; //light up all the LEDs on the PIC18 board
            delay_ms(1000);
        } else {
            PORTD = 0x00;
            delay_ms(1000);
        }
    }
    return 0;
}
//RB5 is for Moisture sensor
    //When moisture sensor is dry, it reads 0
    //When moisture sensor is wet, it reads 1
    //PORTBbits.RB5
int moistureTest(){         //This code lights up all LEDs at PORTD if the moisture sensor reads 1 (wet)
    while(1){
        if(PORTBbits.RB5 ==1){
            PORTD=0xFF;
            delay_ms(1000);
        }else{
            PORTD =0x00;
            delay_ms(1000);
        }
    }
    return 0;
}

int motorTest(){
    while (1){
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
    return 0;
}

int comeback(){
    while (1){
        if (PORTBbits.RB3== 0 && PORTBbits.RB5==1){
            PORTD= 0b00101011; // turns both motors
            delay_ms(5000);
            PORTDbits.RD5 = 0; PORTDbits.RD4 = 1; // reverses Motor 2
            delay_ms(5000);
            PORTDbits.RD0 = 0; // stops Motor1
            delay_ms(5000);
            PORTDbits.RD3 = 0; // stops Motor2
            delay_ms(5000);
            
        } else{
            return;
        } 
    }
}

void interrupt overrideButton_isr(void) {
    //for now we do emergency stop
    INTCONbits.INT0IF = 0; //clear flag
    PORTDbits.RD0 = 0; // stops Motor1
    PORTDbits.RD3 = 0; // stops Motor2
}

