/*
 * File:   Drivemotor.c
 * Author: natch
 *
 * Created on 6 พฤศจิกายน 2563, 2:09 น.
 */


#include "xc.h"
#include "Drivemotor.h"
#include "math.h"


#define INA_X LATBbits.LATB3
#define INB_X LATBbits.LATB2
#define INA_Y LATBbits.LATB14
#define INB_Y LATBbits.LATB13

void initMotor(void)
{
    // PWM(X) at PB11 PWM(Y) at RB14.
    T2CONbits.TCKPS = 0b10;         // set timer prescaler
    
    // PWM 1
    PR2 = 1300;                    // set period to X tick per cycle
    OC1RS = 650;                  // set pwm1 numerator to OC1
    // Config bits
    OC1CONbits.OCM = 0b000;         // set to pwm without fault pin mode
    OC1CONbits.OCTSEL = 0;          // OC1 use time2 as counter source
    OC1CONbits.OCM = 0b110;         // set to pwm without fault pin mode
//     PWM 2
    OC2RS = 650;                   // set pwm2 numerator to OC2
    OC2CONbits.OCM = 0b000;         // disable 
    OC2CONbits.OCTSEL = 0;          // OC2 also use timer2 as counter source
    OC2CONbits.OCM = 0b110;         // set to pwm without fault pin mode
    
    __builtin_write_OSCCONL(OSCCON & 0xBF); //PPS RECONFIG UNLOCK
    _RP4R = 0b10010;           // remap RP4 connect to OC1
    _RP15R = 0b10011;           // remap RP15 connect to OC2
    __builtin_write_OSCCONL(OSCCON | 0x40); //PPS RECONFIG LOCK 
        
}

void RunMotorX(int duty){
    if(duty>0){
        INA_X = 1;
        INB_X = 0;
    }
    if(duty<0){
        INA_X = 0;
        INB_X = 1;
    }
    if(duty==0){
        INA_X = 1;
        INB_X = 1;
    }
    unsigned int getduty = fabs(duty);
    if(getduty > 1200){
        getduty = 1200;
    }
    OC1RS = getduty;
//    OC1RS = (getduty*1300)/100;
}

void RunMotorY(int duty){
    if(duty>0){
        INA_Y = 1;
        INB_Y = 0;
    }
    if(duty<0){
        INA_Y = 0;
        INB_Y = 1;
    }
    if(duty==0){
        INA_Y = 1;
        INB_Y = 1;
    }
    unsigned int getduty = fabs(duty);
    if(getduty > 1200){
        getduty = 1200;
    }
//    OC2RS = (getduty*1300)/100;
    OC2RS = getduty;
}

void MotorXStop(){
    INA_X = 1;
    INB_X = 1;
}

void MotorYStop()
{
    INA_Y = 1;
    INB_Y = 1;
}


