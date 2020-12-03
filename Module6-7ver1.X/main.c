/*
 * File:   module.c
 * Author: natch
 *
 * Created on 23 กันยายน 2563, 15:19 น.
 */


#include "xc.h"
#include "configuration.h"
#include <stdio.h>
#include "Drivemotor.h"
#include "MainCode.h"
#include "QEI.h"
#include "UART_Vilgax.h"


unsigned char UART_buffer[9] = {9,0,0,0,0,0,0,0,0};


// ----------------------------- PID -----------------------------------------------
volatile float kp,ki,kd;
float perr[2]={0,0};
float iterm[2]={0,0};
float dterm[2]={0,0};
int lastValue = 0;
float getposx,getposy;

float PID(float setpoint,unsigned int enc, char axis, float kp,float ki,float kd){
    float err;
    float u;
    kd = kd/4.8;
    err = setpoint - enc;
    if (err >= 100){
        iterm[axis] += err;
        dterm[axis] = (err - perr[axis]);
        u = (kp*err)+(ki*iterm[axis])+(kd*dterm[axis]);
        perr[axis] = err;
    }
    else{u = 0;}
    return u;
}



// -------------------------------- CONTROL --------------------------------------------------------

void initCONT()
{ 
    T1CONbits.TCKPS = 0b01; //set timer prescaler to 1:8
    PR1 = 10417;             //set period to 10 ms1
//    PR1 = 50000;             //set period to 10 ms
    _T1IE = 1;              //enable interrupt for timer1
    _T1IP = 7;              //set interrupt priority to 3
}

//---------------------------- interrupt function
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void)
{
    if (TrajectoryFlag()){
        RunTrajectory();
    }
//    getposx = 5000.00;
//    getposy = 5000.00;
//    float pidoutx = PID(getposx,POS1CNT,0,12.0,0.5,10.0)/20;
//    float pidouty = PID(getposy,POS2CNT,1,12.0,0.5,10.0)/20;
////    printf("%f   %u\n",getpos,position);
    float pidoutx = PID(getposX(),POS1CNT,0,8.0,0,5.0)/20;
    float pidouty = PID(getposY(),POS2CNT,1,8.0,0,5.0)/20;
    if (TrajectoryFlag()){
//        printf("%.2f %.2f %u  %u %.2f %.2f\n",getposX(),getposY(),POS1CNT,POS2CNT,pidoutx,pidouty);
    }
//    send_package(getposX());  
//    send_package(getposY());  
//    send_package(POS1CNT);  
//    send_package(POS2CNT);
//    printf("%u  %u\n",POS1CNT,POS2CNT);
//    printf("%u\n",POS2CNT);
        RunMotorX(pidoutx);
        RunMotorY(pidouty);
//    printf("K");
    _T1IF =0 ;  //clear interrupt flag
}

// --------------------------------------------- Allinit -------------------------
void initPLL() // Set Fcy to 40 MHz
{
    PLLFBD = 150;           // M  = 152
    CLKDIVbits.PLLPRE = 5;  // N1 = 7
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    OSCTUN = 0;             // Tune FRC oscillator, if FRC is used
    
    // Clock switching to incorporate PLL
    __builtin_write_OSCCONH(0x01);    // Initiate Clock Switch to FRCPLL
    // Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01);    // Start clock switching

    while (OSCCONbits.COSC != 0b001); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK!=1) {};    // Wait for PLL to lock
}

int main(void) {
    
    __builtin_disable_interrupts(); 
           
    initPLL();
    initUART();
    initCONT();
    initMotor();
    initQEIX();
    initQEIY();
//    initQEIX(11,10);
    /*enable global interrupt*/
    
    __builtin_enable_interrupts();
    
    AD1PCFGL = 0xFFFF;                // Analog/digital config, 0 for analog(default), 1 for digital
    TRISB = 0x1FE3;                   // 0-15 bitI/O config 0 for output, 1 for input
    
    T2CONbits.TON = 1;                // enable timer 2 enable = 1
    sethome();
//    T1CONbits.TON =1;            //enable timer1
//    printf("Program start\n");
// ------------------------------- code start here ------------------------------------
    
    while(1){
//        printStar();
        ReadUARTBuffer(UART_buffer);
        delay(50);
        CommandSet(UART_buffer);
        char i;
        if(UART_buffer[8]){
            for(i=0; i<9; i++){
            printf("%x",UART_buffer[i]);
            }
            printf("\n");
        }
        
//    RunMotorX(260);
//    RunMotorY(260);
//    delay(500);
//    MotorXStop();
//    MotorYStop();
//    delay(500);
//    RunMotorX(-260);
//    RunMotorY(-260);
//    delay(500);
//    MotorXStop();
//    MotorYStop();
//    delay(500);
    }
    return 0;
}
