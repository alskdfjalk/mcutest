/*--------------------------------------------------------------------------
  REG52.H

  Header file for generic 80C52 and 80C32 microcontroller.
  Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
  All rights reserved.
  --------------------------------------------------------------------------*/

#ifndef __REG52_H__
#define __REG52_H__

/* Led number */
#define ZERO 0x3F
#define ONE 0x06
#define TWO 0x5B
#define THREE 0x4F
#define FOUR 0x66
#define FIVE 0x6D
#define SIX 0x7D
#define SEVEN 0x07
#define EIGHT 0xFF
#define NINE 0xEF

#define usint unsigned short int
#define uchar unsigned char

void delay(usint waitsec)
{
    waitsec = 131 * waitsec;
    while (waitsec-- > 0);
}
/*  BYTE Registers  */
__sfr __at (0x80) P0   ;
__sfr __at (0x90) P1   ;
__sfr __at (0xA0) P2   ;
__sfr __at (0xB0) P3   ;
__sfr __at (0xD0) PSW  ;
__sfr __at (0xE0) ACC  ;
__sfr __at (0xF0) B    ;
__sfr __at (0x81) SP   ;
__sfr __at (0x82) DPL  ;
__sfr __at (0x83) DPH  ;
__sfr __at (0x87) PCON ;
__sfr __at (0x88) TCON ;
__sfr __at (0x89) TMOD ;
__sfr __at (0x8A) TL0  ;
__sfr __at (0x8B) TL1  ;
__sfr __at (0x8C) TH0  ;
__sfr __at (0x8D) TH1  ;
__sfr __at (0xA8) IE   ;
__sfr __at (0xB8) IP   ;
__sfr __at (0x98) SCON ;
__sfr __at (0x99) SBUF ;

/*  8052 Extensions  */
__sfr __at (0xC8) T2CON ;
__sfr __at (0xCA) RCAP2L;
__sfr __at (0xCB) RCAP2H;
__sfr __at (0xCC) TL2   ;
__sfr __at (0xCD) TH2   ;


/*  BIT Registers  */
/*  PSW   */
__sbit __at (0xD0) P    ;
__sbit __at (0xD1) F1   ;
__sbit __at (0xD2) OV   ;
__sbit __at (0xD3) RS0  ;
__sbit __at (0xD4) RS1  ;
__sbit __at (0xD5) F0   ;
__sbit __at (0xD6) AC   ;
__sbit __at (0xD7) CY   ;
/*  PSW
__sbit __at (PSW^7) CY   ;
__sbit __at (PSW^6) AC   ;
__sbit __at (PSW^5) F0   ;
__sbit __at (PSW^4) RS1  ;
__sbit __at (PSW^3) RS0  ;
__sbit __at (PSW^2) OV   ;
__sbit __at (PSW^0) P    ; //8052 only
*/
/*  TCON  */
__sbit __at (0x88) IT0  ;
__sbit __at (0x89) IE0  ;
__sbit __at (0x8A) IT1  ;
__sbit __at (0x8B) IE1  ;
__sbit __at (0x8C) TR0  ;
__sbit __at (0x8D) TF0  ;
__sbit __at (0x8E) TR1  ;
__sbit __at (0x8F) TF1  ;
/*  TCON
__sbit __at (TCON^7) TF1  ;
__sbit __at (TCON^6) TR1  ;
__sbit __at (TCON^5) TF0  ;
__sbit __at (TCON^4) TR0  ;
__sbit __at (TCON^3) IE1  ;
__sbit __at (TCON^2) IT1  ;
__sbit __at (TCON^1) IE0  ;
__sbit __at (TCON^0) IT0  ;
*/
/*  IE   */
__sbit __at (0xA8) EX0  ;
__sbit __at (0xA9) ET0  ;
__sbit __at (0xAA) EX1  ;
__sbit __at (0xAB) ET1  ;
__sbit __at (0xAC) ES   ;
__sbit __at (0xAF) EA   ;
/*  IE
__sbit __at (IE^7) EA   ;
__sbit __at (IE^5) ET2  ; //8052 only
__sbit __at (IE^4) ES   ;
__sbit __at (IE^3) ET1  ;
__sbit __at (IE^2) EX1  ;
__sbit __at (IE^1) ET0  ;
__sbit __at (IE^0) EX0  ;
*/
/*  IP   */
__sbit __at (0xB8) PX0  ;
__sbit __at (0xB9) PT0  ;
__sbit __at (0xBA) PX1  ;
__sbit __at (0xBB) PT1  ;
__sbit __at (0xBC) PS   ;
/*  IP
__sbit __at (IP^5) PT2  ;
__sbit __at (IP^4) PS   ;
__sbit __at (IP^3) PT1  ;
__sbit __at (IP^2) PX1  ;
__sbit __at (IP^1) PT0  ;
__sbit __at (IP^0) PX0  ;
*/
/*  P3  */
__sbit __at (0xB0) P3_0 ;
__sbit __at (0xB1) P3_1 ;
__sbit __at (0xB2) P3_2 ;
__sbit __at (0xB3) P3_3 ;
__sbit __at (0xB4) P3_4 ;
__sbit __at (0xB5) P3_5 ;
__sbit __at (0xB6) P3_6 ;
__sbit __at (0xB7) P3_7 ;
/*  P3
__sbit __at (P3^7) RD   ;
__sbit __at (P3^6) WR   ;
__sbit __at (P3^5) T1   ;
__sbit __at (P3^4) T0   ;
__sbit __at (P3^3) INT1 ;
__sbit __at (P3^2) INT0 ;
__sbit __at (P3^1) TXD  ;
__sbit __at (P3^0) RXD  ;
*/
/*  SCON  */
__sbit __at (0x98) RI   ;
__sbit __at (0x99) TI   ;
__sbit __at (0x9A) RB8  ;
__sbit __at (0x9B) TB8  ;
__sbit __at (0x9C) REN  ;
__sbit __at (0x9D) SM2  ;
__sbit __at (0x9E) SM1  ;
__sbit __at (0x9F) SM0  ;
/*  SCON
__sbit __at (SCON^7) SM0  ;
__sbit __at (SCON^6) SM1  ;
__sbit __at (SCON^5) SM2  ;
__sbit __at (SCON^4) REN  ;
__sbit __at (SCON^3) TB8  ;
__sbit __at (SCON^2) RB8  ;
__sbit __at (SCON^1) TI   ;
__sbit __at (SCON^0) RI   ;
*/
/*  P1  */
// __sbit __at (P1^1) T2EX ; // 8052 only
// __sbit __at (P1^0) T2   ; // 8052 only

/*  T2CON
__sbit __at (T2CON^7) TF2   ;
__sbit __at (T2CON^6) EXF2  ;
__sbit __at (T2CON^5) RCLK  ;
__sbit __at (T2CON^4) TCLK  ;
__sbit __at (T2CON^3) EXEN2 ;
__sbit __at (T2CON^2) TR2   ;
__sbit __at (T2CON^1) C_T2  ;
sbit CP_RL2 = T2CON^0;
*/
#endif
