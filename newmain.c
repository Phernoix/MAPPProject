#include <xc.h>
#include "delays.h"
#include <stdbool.h> 

#define LDR PORTBbits.RB3
#define MOIST_SENS PORTBbits.RB5
#define MOTOR1_EN1 PORTDbits.RD0
#define MOTOR1_IN1 PORTDbits.RD1
#define MOTOR1_IN2 PORTDbits.RD2
#define MOTOR2_EN2 PORTDbits.RD3
#define MOTOR2_IN4 PORTDbits.RD4
#define MOTOR2_IN3 PORTDbits.RD5

int isDark(void);
int isWet(void);
void interrupt overrideButton_isr(void);
void enableMotors(void);
void disableMotors(void);
void moveMotor(void);
void moveMotor_Opposite(void);
void manualBrake(void);

int motorTime = 1000;
bool outside = false;

void main(void) {
    TRISB = 0b11111111;     // For LDR and Moisture sensor
    TRISD = 0b00000000;     // RD7 to RD0 are connected to LEDs
    INTCONbits.GIE = 1;     // Enable global interrupt
    INTCONbits.INT0IE = 1;  // Enable int for RB0

    while(1) {
        if (isDark() && isWet())  {
            moveMotor();
            outside = true;
            delay_ms(1000);
            manualBrake();
        } else {
            manualBrake();
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
    if (outside) {
        INTCONbits.INT0IF = 0;      //clear flag
        moveMotor_Opposite();
        outside = false;
    }
}


/* Code for Motor:
 * We are using a H bridge for the motor
 * https://www.modularcircuits.com/blog/articles/h-bridge-secrets/h-bridges-the-basics
 */
void enableMotors(void) {
    MOTOR1_EN1 = 1;
    MOTOR2_EN2 = 1;    
}

void disableMotors(void) {
    MOTOR1_EN1 = 0;
    MOTOR2_EN2 = 0;
}

void moveMotor(void) {
    enableMotors();
    MOTOR1_IN1 = 1;
    MOTOR1_IN2 = 0;
    MOTOR2_IN3 = 1;
    MOTOR2_IN4 = 0;
    delay_ms(motorTime);
    disableMotors();
}

void moveMotor_Opposite(void) {
    enableMotors();
    MOTOR1_IN1 = 0;
    MOTOR1_IN2 = 1;
    MOTOR2_IN3 = 0;
    MOTOR2_IN4 = 1;
    delay_ms(motorTime);
    disableMotors();
}

void manualBrake(void) {
    enableMotors();
    MOTOR1_IN1 = 0;
    MOTOR1_IN2 = 0;
    MOTOR2_IN3 = 0;
    MOTOR2_IN4 = 0;
    delay_ms(motorTime);
    disableMotors();
}


