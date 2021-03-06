#include <xc.h>
#include "delays.h"
#include <stdbool.h> 

#define LDR PORTBbits.RB3
#define MOIST_SENS PORTBbits.RB5
#define MOTOR2_EN PORTDbits.RD3

int isDark(void);
int isWet(void);
void interrupt overrideButton_isr(void);
void motorBringIn(void);
void motorBringOut(void);

int motorTime = 1000;
int stopMotorTime = 2000;
bool bringIn = false;
bool outside = false;
bool overridden = false;

void main(void) {
    TRISB = 0b11111111;     // For Buttons, LDR and Moisture sensor
    TRISD = 0b00000000;     // For H-bridge (motor driver)
    INTCONbits.GIE = 1;     // Enable global interrupt
    
    while(1) { 
        /* override mode */
        if (PORTBbits.RB2 == 1) {
            overridden = !overridden;
            delay_ms(10);
        }
        
        if (!overridden) {   // not overridden
            if (outside && isDark() && isWet()) { 
                // dont ever change this code, what this does is: when dark and rain and outside it will spin back and it will STOP
                //ONLY CODE THAT WILL STOP THE FUCKING MOTOR
                motorBringIn();
                outside = false;    
            }
        } else {  // overridden
            if (PORTBbits.RB0 == 1) {  // we can't set flags inside interrupt function
                delay_ms(10);
                INTCONbits.INT0IE = 1;  // Enable int for RB0
                bringIn = !bringIn;
                outside = !outside;
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


void interrupt overrideButton_isr(void) {
    INTCONbits.INT0IF = 0;      //clear flag
       
    switch(bringIn){
        case 0: 
            motorBringOut();
            MOTOR2_EN = 0;    
            INTCONbits.INT0IE = 0;  // Enable int for RB0
            break;
        case 1: 
            motorBringIn();
            MOTOR2_EN = 0;    
            INTCONbits.INT0IE = 0;  // Enable int for RB0
            break;
        default:
            break;
    }
}


/* Code for Motor:
 * We are using a H bridge for the motor
 * https://www.modularcircuits.com/blog/articles/h-bridge-secrets/h-bridges-the-basics
 */

void motorBringIn(void) {
    PORTD = 0b00011000;
    delay_ms(motorTime);
    MOTOR2_EN = 0; // stops Motor2
    delay_ms(stopMotorTime);
    
    return;
}

void motorBringOut(void) {
    PORTD = 0b00101000;
    delay_ms(motorTime);
    MOTOR2_EN = 0; // stops Motor2
    delay_ms(stopMotorTime);
    
    return;
}