#include <xc.h>
#include "delays.h"
#include <stdbool.h> 

#define LDR PORTBbits.RB3
#define MOIST_SENS PORTBbits.RB5

int isDark(void);
int isWet(void);
void interrupt overrideButton_isr(void);
void moveMotor(void);
void moveMotor_Opposite(void);

int motorTime = 1000;
int stopMotorTime = 2000;
bool b = true;
bool outside = false;
bool overridden = false;
bool outsideTemp = false;

void main(void) {
    TRISB = 0b11111111;     // For LDR and Moisture sensor
    TRISD = 0b00000000;     // RD7 to RD0 are connected to LEDs
    INTCONbits.GIE = 1;     // Enable global interrupt
    INTCONbits.INT0IE = 1;  // Enable int for RB0
    
    while(1) { 
       
        if (outside ==true && isDark() && isWet()) { 
            // dont ever change this code, what this does is: when dark and rain and outside it will spin back and it will STOP
            //ONLY CODE THAT WILL STOP THE FUCKING MOTOR
            moveMotor_Opposite();
            outside = false;    
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


void interrupt overrideButton_isr(void) {
    INTCONbits.INT0IF = 0;      //clear flag
    
    
    switch(b){
        case 0: moveMotor();PORTDbits.RD3 = 0;
        break;
        case 1: moveMotor_Opposite();PORTDbits.RD3 = 0;
        break;
        default:
            break;
    }
    b = !b;
}


/* Code for Motor:
 * We are using a H bridge for the motor
 * https://www.modularcircuits.com/blog/articles/h-bridge-secrets/h-bridges-the-basics
 */

void moveMotor(void) {
    PORTD = 0b00011000;
    delay_ms(motorTime);
    PORTDbits.RD3 = 0; // stops Motor2
    delay_ms(stopMotorTime);
    
    return;
}

void moveMotor_Opposite(void) {
    PORTD = 0b00101000;
    delay_ms(motorTime);
    PORTDbits.RD3 = 0; // stops Motor2
    delay_ms(stopMotorTime);
    
    return;
}