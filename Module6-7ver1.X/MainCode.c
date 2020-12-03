/*
 * File:   MainCode.c
 * Author: natch
 *
 * Created on 6 พฤศจิกายน 2563, 2:53 น.
 */


#include "xc.h"
#include "Drivemotor.h"
#include "MainCode.h"
#include "UART_Vilgax.h"
#include <math.h>

#define SW_X PORTBbits.RB7
#define SW_Y PORTBbits.RB12
#define trajectorytime 800;

float posX, posY,c2,c3,x0,y0,Theta;
unsigned char Trajectory_Flag;

void delay(uint16_t time_ms){
    uint16_t i,j;
    for(i=0; i<time_ms; i++){
        for(j=0; j<4000;j++) Nop();
    }
}

void sethome(){
    T1CONbits.TON =0;            //disable timer1
    char swFlag_X = 0;
    char swFlag_Y = 0;
    while((swFlag_X != 1)||(swFlag_Y != 1)){
        if(SW_X != 0){
            RunMotorX(-260);
        }
        if(SW_Y != 0){
            RunMotorY(-260);
        }
        if(SW_X == 0){
            MotorXStop();
            swFlag_X = 1;
        }
        if(SW_Y == 0){
            MotorYStop();
            swFlag_Y = 1;
        }
    }
    delay(100);
    RunMotorX(260);
    RunMotorY(260);
    delay(500);
    MotorXStop();
    MotorYStop();
    delay(500);
    posX = 0;
    posY = 0;
    POS1CNT = 0;
    POS2CNT = 0;
    T1CONbits.TON =1;            //enable timer1
}

// ----------------------------- UART -----------------------------------------------
void CommandSet(unsigned char buffer[9]){
//    printf("ready");
    static unsigned char command = 9;
    // check if there is a new command
    if (buffer[8] == 1){
        command = buffer[0];
//        printf("BUFFER: %x\n", buffer[0]);
    }
    if (command == 0){
        sethome();
        send_package(0x484D);
        command = 9;
        Trajectory_Flag = 0;
        }
    if (Trajectory_Flag == 0){
        if (command == 1){
            send_package(0x4350);       // send CP
            command = 9;
            } 
        else if (command == 2){
            posX = (buffer[1] << 8) | buffer[2];
            posY = (buffer[3] << 8) | buffer[4];
            printf("%f, %f \n", posX,posY);
            send_package(0x5350);       // send SP
            command = 9;
            } 
        else if (command == 3){
            // trajectory
            float rf = (buffer[1] << 8) | buffer[2];
            float theta = (buffer[3] << 8) | buffer[4];
            send_package(0x544A);       // send TJ
            SetTrajectory(rf,  theta);
            Trajectory_Flag = 1;
            command = 9;
            }
        else{command = 9;}
    }
    else{command = 9;}
}

// ----------------------------- TRAJECTORY -----------------------------------------
void SetTrajectory(float rf, float theta){
 //  let's  v0 = 0; vf = 0;r0 = 0;t0 = 0;
 //  let's  c0 = 0; c1 =0; else (add c0 + c1*t to rt) and (add c1 to vt)
    float tf = 8.00;
    printf("Print: %.2f ", Theta);
    Theta = theta*3.14/(180.0);
    printf("%.2f\n", theta);
    x0 = POS1CNT;
    y0 = POS2CNT;
    c2 =(3.00*rf)/(tf*tf);
    c3 =(-2.00*rf)/(tf*tf*tf);
}

void RunTrajectory(){
    static float t = 0;
//    char r0,v0,vf,t0,tf;
//    int rt,vt,vx,vy;
    
    if(t > 8.50){
        Trajectory_Flag = 0;
        t = 0;
        posX = POS1CNT;
        posY = POS2CNT;
//        printf("Trajed finish %u, %u\n", POS1CNT, POS2CNT);
    }
    else{
//        printf("%f\n", Theta);
        float rt = c2*(t*t) + c3*(t*t*t);
    //    vt =  2*c2*t + 3*c3*t.^2;
        posX = x0 + (rt*cos(Theta));
        posY = y0 + (rt*sin(Theta));
    //    vx = vt*cos(theta);
    //    vy = vt*sin(theta);
//        t+=0.01; // 10 ms
        t+=0.0025; // 2 ms
//        printf("%.2f ", t);
        
    }

}
char TrajectoryFlag(){
    return Trajectory_Flag;
}

float getposX(){
    return (posX/100.00)*98.47;
}
float getposY(){
    return (posY/100.00)*98.47;
}