
#ifndef DRIVEMOTOR_H
#define	DRIVEMOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

void initMotor(void);
void RunMotorX(int duty);
void RunMotorY(int duty);
void MotorXStop();
void MotorYStop();
//void initPID(uint8_t p, uint8_t i, uint8_t d);
//void PIDX(int setpoint);

#endif	/* XC_HEADER_TEMPLATE_H */

