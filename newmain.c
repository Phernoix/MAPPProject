#include <xc.h>
#include "delays.h"
#include <stdbool.h> 

#define LDR PORTBbits.RB3
#define MOIST_SENS PORTBbits.RB5
#define MOTOR_EN PORTDbits.RD3

int isDark(void);
int isWet(void);
void interrupt buttonISR(void);
void outButton_isr(void);
void inButton_isr(void);
void moveMotor(void);
void moveMotor_Opposite(void);

bool outside = false;
bool overriden = false;

void main(void) {
    TRISB = 0b11111111;     // For LDR and Moisture sensor
    TRISD = 0b00000000;     // RD7 to RD0 are connected to LEDs
    INTCONbits.GIE = 1;     // Enable global interrupt
    INTCONbits.INT0IE = 1;  // Enable int for RB0
    INTCON3bits.INT2IE = 1; //interrupt enable for RB2
    
    while(1) { 
        if (!overriden) {
            if (outside && isDark() && isWet()) { 
                // dont ever change this code, what this does is: when dark and rain and outside it will spin back and it will STOP
                //ONLY CODE THAT WILL STOP THE FUCKING MOTOR
                moveMotor_Opposite();
                outside = false;    
            }  
        }
    }
}

 
/* This function turns off LEDs at PORTD if the LDR detects light (use phone flashlight)
 * When LDR reads 0, it means it is bright (there is light)
 * When LDR reads 1, it means it is dim
 */
int isDark() {              
    if (LDR == 1)
        return 1;
    
    return 0;
}

/* RB5 is for Moisture sensor
    * When moisture sensor is dry, it reads 0
    * When moisture sensor is wet, it reads 1
    * PORTBbits.RB5 
 */
//This code lights up all LEDs at PORTD if the moisture sensor reads 1 (wet)
int isWet(void) {         
    if(MOIST_SENS == 1)
        return 1;
    
    return 0;
}

/* Code for buttons 
 * We have 2 buttons:
 * One for bringing in clothes
 * Another one for bringing out clothes
 */
void interrupt buttonISR(void) {    
    if (INTCONbits.INT0IF == 1) 
        outButton_isr();
    if (INTCON3bits.INT2IF == 1)
        inButton_isr();
}

void outButton_isr(void) {
    moveMotor();
    INTCONbits.INT0IF = 0;
}

void inButton_isr(void) {
    moveMotor_Opposite();
    INTCON3bits.INT2IF = 0;
}

/* Code for motors
 * We will only be using motor 2
 */
void moveMotor(void) {
    PORTD = 0b00101011; // turns both motors
    delay_ms(1000);
    PORTDbits.RD3 = 0; // stops Motor2
    delay_ms(2000);
    PORTDbits.RD0 = 0; // stops Motor1
    delay_ms(2000);
}

void moveMotor_Opposite(void) {
    PORTD = 0b00001011; // turns both motors
    PORTDbits.RD5 = 0; PORTDbits.RD4 = 1;
    delay_ms(1000);
    PORTDbits.RD3 = 0; // stops Motor2
    delay_ms(2000);
    PORTDbits.RD0 = 0; // stops Motor1
    delay_ms(2000);

}
