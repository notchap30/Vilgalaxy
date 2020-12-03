/* 
 * File:   MainCode.h
 * Author: natch
 *
 * Created on 6 พฤศจิกายน 2563, 2:54 น.
 */

#ifndef MAINCODE_H
#define	MAINCODE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void delay(uint16_t time_ms);
void sethome();
void CommandSet(unsigned char buffer[9]);
float getposX();
float getposY();
char TrajectoryFlag();

#endif	/* MAINCODE_H */

