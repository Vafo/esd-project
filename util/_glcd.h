#ifndef GLCD_H
#define GLCD_H

#include <stdint.h>
void cmndl( uint8_t cmd );	/* lcd 명령 출력 */
void cmndr( uint8_t cmd );
void cmnda( uint8_t cmd );

void datal( uint8_t dat );	/* 1 문자 출력  */
void datar( uint8_t dat );
void dataa( uint8_t dat );
	
void lcd_clear(void);	/* GLCD Clear */
void lcd_init(void);	/* GLCD Initialize */
void lcd_xy( uint8_t x, uint8_t y );	/* 문자 위치 세트 */
void lcd_char(uint8_t character);	/* 한 문자 출력 */
void lcd_string(uint8_t x,uint8_t y,char *string);	/* 문자열 출력 */

void GLCD_Axis_xy(unsigned char x, unsigned char y);
void GLCD_Dot(unsigned char xx,unsigned char y);	// 점을 그립니다.
void ScreenBuffer_clear(void);
void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void GLCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);	// 직사각형을 그립니다
void GLCD_Circle(unsigned char x1,unsigned char y1,unsigned char r); // 원을 그립니다.

unsigned char GLCD_1DigitDecimal(unsigned char number, unsigned char flag);	// 1자리의 10진수 값을 표시합니다.
void GLCD_2DigitDecimal(unsigned char number);	// 2자리의 10진수 값을 표시합니다.
void GLCD_3DigitDecimal(unsigned int number);	// 3자리의 10진수 값을 표시합니다.
void GLCD_4DigitDecimal(unsigned int number);	// 4자리의 10진수 값을 표시합니다.

#endif