#include "Driver/Chip/Oled.h"
#include "Driver/St/Sys.h"

const uint8_t OledCharSize = 16;
const uint8_t OledMaxColumn = 128;

//����ASCII��
//ƫ����32
//ASCII�ַ���
//ƫ����32
//��С:12*6
/************************************6*8�ĵ���************************************/
const uint8_t F6x8[][6] =		
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00,// !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00,// "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14,// #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12,// $
    0x00, 0x62, 0x64, 0x08, 0x13, 0x23,// %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50,// &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00,// '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00,// (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00,// )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14,// *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,// +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00,// ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08,// -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00,// .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02,// /
    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,// 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46,// 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,// 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,// 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39,// 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03,// 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36,// 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,// 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00,// :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00,// ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00,// <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14,// =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08,// >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06,// ?
    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E,// @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C,// A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,// B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,// C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C,// D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,// E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,// F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,// G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,// H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,// I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01,// J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,// K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,// L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F,// M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F,// N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,// O
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,// P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,// R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31,// S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,// T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,// U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,// V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F,// W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63,// X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07,// Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43,// Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,// [
    0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55,// 55
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,// ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04,// ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40,// _
    0x00, 0x00, 0x01, 0x02, 0x04, 0x00,// '
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78,// a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,// b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20,// c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,// d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18,// e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,// f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,// g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,// h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,// i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,// j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,// k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,// l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,// m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,// n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38,// o
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,// p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,// q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,// r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20,// s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,// t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,// u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,// v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,// w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44,// x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,// y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,// z
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14,// horiz lines
};
/****************************************8*16�ĵ���************************************/
const uint8_t F8X16[]=	  
{
#if 0  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
    0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
    0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
    0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
    0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
    0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
    0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
    0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
    0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
    0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
    0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
    0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
    0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
    0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
    0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
    0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
    0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
    0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
    0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
    0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
    0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
    0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
    0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
    0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
    0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
    0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
    0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
    0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
    0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
    0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
    0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
    0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
    0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
    0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
    0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
    0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
    0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
    0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
    0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
    0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
    0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
    0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
    0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
    0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
    0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
    0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
    0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
    0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
    0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
    0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
    0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
    0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
    0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
    0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
    0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
    0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
    0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
    0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
    0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
    0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
    0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
    0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
    0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
    0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
    0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
    0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
    0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
    0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
    0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
    0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
    0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
    0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
    0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
    0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
    0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
    0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
    0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
    0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
    0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
    0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
    0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
    0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
    0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
    0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
    0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
    0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
    0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
    0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 //~ 94
#else
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
    0x00,0x00,0x70,0xF8,0xF8,0x70,0x00,0x00,0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,/*"!",1*/
    0x00,0x38,0x38,0x00,0x00,0x38,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*""",2*/
    0x00,0x20,0xF8,0xF8,0x20,0xF8,0xF8,0x20,0x00,0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x02,/*"#",3*/
    0x00,0x30,0x78,0xCE,0x8E,0x18,0x10,0x00,0x00,0x04,0x0C,0x38,0x39,0x0F,0x06,0x00,/*"$",4*/
    0x18,0x3C,0x24,0xBC,0xD8,0x60,0x30,0x00,0x00,0x06,0x03,0x0D,0x1E,0x12,0x1E,0x0C,/*"%",5*/
    0x00,0xB0,0xF8,0x48,0x78,0x30,0x00,0x00,0x00,0x07,0x0F,0x08,0x09,0x07,0x0F,0x09,/*"&",6*/
    0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"'",7*/
    0x00,0x00,0xC0,0xF0,0x38,0x08,0x00,0x00,0x00,0x00,0x07,0x1F,0x38,0x20,0x00,0x00,/*"(",8*/
    0x00,0x00,0x08,0x38,0xF0,0xC0,0x00,0x00,0x00,0x00,0x20,0x38,0x1F,0x07,0x00,0x00,/*")",9*/
    0x00,0x80,0xA0,0xE0,0xC0,0xE0,0xA0,0x80,0x00,0x00,0x02,0x03,0x01,0x03,0x02,0x00,/*"*",10*/
    0x00,0x80,0x80,0xE0,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,/*"+",11*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00,/*",",12*/
    0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"-",13*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00,/*".",14*/
    0x00,0x00,0x00,0x80,0xE0,0x78,0x18,0x00,0x00,0x18,0x1E,0x07,0x01,0x00,0x00,0x00,/*"/",15*/
    0x00,0x00,0xF0,0xF8,0x08,0x68,0xF8,0xF0,0x00,0x00,0x07,0x0F,0x0B,0x08,0x0F,0x07,/*"0",16*/
    0x00,0x20,0x20,0x30,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,/*"1",17*/
    0x00,0x30,0x38,0x08,0x88,0xF8,0x70,0x00,0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00,/*"2",18*/
    0x00,0x30,0x38,0x88,0x88,0xF8,0x70,0x00,0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00,/*"3",19*/
    0x00,0x00,0xF8,0xF8,0x00,0xE0,0xE0,0x00,0x00,0x03,0x03,0x02,0x02,0x0F,0x0F,0x02,/*"4",20*/
    0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,0x00,0x08,0x08,0x08,0x0C,0x07,0x03,0x00,/*"5",21*/
    0x00,0xC0,0xE0,0x78,0x58,0xC8,0x80,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"6",22*/
    0x00,0x08,0x08,0x88,0xE8,0x78,0x18,0x00,0x00,0x00,0x0E,0x0F,0x01,0x00,0x00,0x00,/*"7",23*/
    0x00,0x70,0xF8,0xC8,0x88,0xF8,0x70,0x00,0x00,0x07,0x0F,0x08,0x09,0x0F,0x07,0x00,/*"8",24*/
    0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,0x00,0x00,0x09,0x0D,0x0F,0x03,0x01,0x00,/*"9",25*/
    0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00,/*":",26*/
    0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00,/*";",27*/
    0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00,0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00,/*"<",28*/
    0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,/*"=",29*/
    0x00,0x08,0x18,0x30,0x60,0xC0,0x80,0x00,0x00,0x08,0x0C,0x06,0x03,0x01,0x00,0x00,/*">",30*/
    0x00,0x30,0x38,0x88,0xC8,0x78,0x30,0x00,0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,/*"?",31*/
    0xF0,0xF8,0x08,0x88,0xC8,0x48,0xF8,0xF0,0x07,0x0F,0x08,0x09,0x0B,0x0A,0x0B,0x0B,/*"@",32*/
    0x00,0xE0,0xF0,0x18,0x18,0xF0,0xE0,0x00,0x00,0x0F,0x0F,0x01,0x01,0x0F,0x0F,0x00,/*"A",33*/
    0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"B",34*/
    0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00,0x00,0x07,0x0F,0x08,0x08,0x0E,0x06,0x00,/*"C",35*/
    0x00,0xF8,0xF8,0x08,0x18,0xF0,0xE0,0x00,0x00,0x0F,0x0F,0x08,0x0C,0x07,0x03,0x00,/*"D",36*/
    0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,/*"E",37*/
    0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,/*"F",38*/
    0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00,0x00,0x07,0x0F,0x08,0x09,0x0F,0x0F,0x00,/*"G",39*/
    0x00,0xF8,0xF8,0x80,0x80,0xF8,0xF8,0x00,0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,/*"H",40*/
    0x00,0x00,0x08,0xF8,0xF8,0x08,0x00,0x00,0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,/*"I",41*/
    0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00,/*"J",42*/
    0x00,0xF8,0xF8,0x80,0xE0,0x78,0x18,0x00,0x00,0x0F,0x0F,0x00,0x03,0x0F,0x0C,0x00,/*"K",43*/
    0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,/*"L",44*/
    0x00,0xF8,0xF8,0x20,0xC0,0x20,0xF8,0xF8,0x00,0x0F,0x0F,0x00,0x01,0x00,0x0F,0x0F,/*"M",45*/
    0x00,0xF8,0xF8,0x60,0xC0,0x80,0xF8,0xF8,0x00,0x0F,0x0F,0x00,0x00,0x01,0x0F,0x0F,/*"N",46*/
    0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"O",47*/
    0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,/*"P",48*/
    0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,0x00,0x07,0x0F,0x08,0x18,0x3F,0x27,0x00,/*"Q",49*/
    0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,0x00,0x0F,0x0F,0x00,0x01,0x0F,0x0E,0x00,/*"R",50*/
    0x00,0x30,0x78,0xC8,0x88,0x18,0x10,0x00,0x00,0x04,0x0C,0x08,0x09,0x0F,0x06,0x00,/*"S",51*/
    0x00,0x08,0x08,0xF8,0xF8,0x08,0x08,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,/*"T",52*/
    0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"U",53*/
    0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00,0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00,/*"V",54*/
    0x00,0xF8,0xF8,0x00,0xC0,0x00,0xF8,0xF8,0x00,0x01,0x0F,0x0E,0x01,0x0E,0x0F,0x01,/*"W",55*/
    0x00,0x18,0x38,0xE0,0xC0,0x38,0x18,0x00,0x00,0x0E,0x0F,0x00,0x01,0x0F,0x0E,0x00,/*"X",56*/
    0x00,0x78,0xF8,0x80,0x80,0xF8,0x78,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,/*"Y",57*/
    0x00,0x08,0x08,0x88,0xC8,0x78,0x38,0x00,0x00,0x0E,0x0F,0x09,0x08,0x08,0x08,0x00,/*"Z",58*/
    0x00,0x00,0xF8,0xF8,0x08,0x08,0x00,0x00,0x00,0x00,0x7F,0x7F,0x40,0x40,0x00,0x00,/*"[",59*/
    0x00,0x18,0x78,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1E,0x18,0x00,/*"\",60*/
    0x00,0x00,0x08,0x08,0xF8,0xF8,0x00,0x00,0x00,0x00,0x40,0x40,0x7F,0x7F,0x00,0x00,/*"]",61*/
    0x00,0x08,0x0C,0x06,0x06,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"^",62*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,/*"_",63*/
    0x00,0x00,0x02,0x06,0x0E,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"`",64*/
    0x00,0x00,0x20,0x20,0x20,0xE0,0xC0,0x00,0x00,0x06,0x0F,0x09,0x09,0x0F,0x0F,0x00,/*"a",65*/
    0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00,0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"b",66*/
    0x00,0xC0,0xE0,0x20,0x20,0x60,0x40,0x00,0x00,0x07,0x0F,0x08,0x08,0x0C,0x04,0x00,/*"c",67*/
    0x00,0xC0,0xE0,0x20,0x20,0xF8,0xF8,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00,/*"d",68*/
    0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00,0x00,0x07,0x0F,0x09,0x09,0x09,0x01,0x00,/*"e",69*/
    0x00,0x80,0xF0,0xF8,0x88,0x88,0x88,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,/*"f",70*/
    0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00,0x00,0x47,0x4F,0x48,0x48,0x7F,0x3F,0x00,/*"g",71*/
    0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00,0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,/*"h",72*/
    0x00,0x20,0x20,0xEC,0xEC,0x00,0x00,0x00,0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00,/*"i",73*/
    0x00,0x00,0x20,0x20,0xEC,0xEC,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x3F,0x00,0x00,/*"j",74*/
    0x00,0xF8,0xF8,0x00,0x80,0xE0,0x60,0x00,0x00,0x0F,0x0F,0x01,0x03,0x0E,0x0C,0x00,/*"k",75*/
    0x00,0x08,0x08,0xF8,0xF8,0x00,0x00,0x00,0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00,/*"l",76*/
    0x00,0xE0,0xE0,0x20,0xE0,0x20,0xE0,0xC0,0x00,0x0F,0x0F,0x00,0x07,0x00,0x0F,0x0F,/*"m",77*/
    0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00,0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,/*"n",78*/
    0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,/*"o",79*/
    0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00,0x00,0x7F,0x7F,0x08,0x08,0x0F,0x07,0x00,/*"p",80*/
    0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00,0x00,0x07,0x0F,0x08,0x08,0x7F,0x7F,0x00,/*"q",81*/
    0x00,0xE0,0xE0,0x80,0x40,0x60,0x60,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,/*"r",82*/
    0x00,0xC0,0xE0,0x20,0x20,0x20,0x20,0x00,0x00,0x08,0x09,0x09,0x09,0x0F,0x06,0x00,/*"s",83*/
    0x00,0x20,0xF8,0xF8,0x20,0x20,0x20,0x00,0x00,0x00,0x07,0x0F,0x08,0x08,0x08,0x00,/*"t",84*/
    0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00,/*"u",85*/
    0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00,/*"v",86*/
    0x00,0xE0,0xE0,0x00,0xC0,0x00,0xE0,0xE0,0x00,0x03,0x0F,0x0C,0x03,0x0C,0x0F,0x03,/*"w",87*/
    0x00,0x60,0xE0,0x80,0x80,0xE0,0x60,0x00,0x00,0x0C,0x0E,0x03,0x03,0x0E,0x0C,0x00,/*"x",88*/
    0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,0x40,0x47,0x4F,0x68,0x38,0x1F,0x07,0x00,/*"y",89*/
    0x00,0x20,0x20,0x20,0xA0,0xE0,0x60,0x00,0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00,/*"z",90*/
    0x00,0x00,0x80,0xF0,0x78,0x08,0x00,0x00,0x00,0x01,0x03,0x1E,0x3C,0x20,0x00,0x00,/*"{",91*/
    0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,/*"|",92*/
    0x00,0x00,0x08,0x78,0xF0,0x80,0x00,0x00,0x00,0x00,0x20,0x3C,0x1E,0x03,0x01,0x00,/*"}",93*/
    0x30,0x18,0x08,0x18,0x30,0x20,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"~",94*/
	0x00,0x00,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x0F,0x07,0x03,0x01,0x00,0x00/*"fg",95*/
#endif
};


const uint8_t Logo_MO_2300D[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0xF8, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0xF8, 0x38, 0x38, 0x38, 
    0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xE0, 0xF0, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 
    0xE0, 0xF0, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0xE0, 0xF0, 
    0xF8, 0xF8, 0x38, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0xF8, 
    0x38, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x38, 0x38, 
    0x38, 0x38, 0x38, 0xF8, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xE0, 0x80, 
    0xE0, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0xC0, 0xE0, 0xF8, 0xFF, 0x7F, 0x1F, 0x07, 0x00, 0x00, 
    0x03, 0x03, 0x03, 0x03, 0x40, 0xE0, 0xF0, 0xF0, 0xBF, 0xBF, 0x1F, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x07, 0x7F, 0xFF, 
    0x7F, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 
    0x00, 0x00, 0xC0, 0xE0, 0xF8, 0xFC, 0x7F, 0x1F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0xF8, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 
    0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
    0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x01, 0x03, 
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
    0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//m^n����
uint32_t Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

Oled::Oled(OledParam &param) {
  i2c_ = new I2c(param.i2cAddress_, false);
  resetIo_ = new Gpio(param.resetIoParam_);
  resetIo_->Init();
  enableIo_ = new Gpio(param.enableIoParem_);
  enableIo_->Init();
}

void Oled::WriteOneByte(uint8_t op, uint8_t data) {
    i2c_->Send(&data, op, 1);
}

void Oled::DisplayOn() {
    WriteOneByte(oldeOp_Cmd,0xAF);  //DISPLAY ON
}

void Oled::DisplayOff() {
    WriteOneByte(oldeOp_Cmd,0xAE);  //DISPLAY ON
}

void Oled::SetPos(uint8_t x, uint8_t y) {
	WriteOneByte(oldeOp_Cmd, 0xb0+y);
	WriteOneByte(oldeOp_Cmd, ((x&0xf0)>>4)|0x10);
	WriteOneByte(oldeOp_Cmd, (x&0x0f)|0x01); 
}

void Oled::Init() {
    enableIo_->Enable();
    resetIo_->Enable();
    Sys::DelayMs(100);
    
    resetIo_->Disable();
    Sys::DelayMs(100);
  
    resetIo_->Enable();
    
	WriteOneByte(oldeOp_Cmd, 0xAE);//--turn off oled panel
    WriteOneByte(oldeOp_Cmd, 0x00);//---set low column address
    WriteOneByte(oldeOp_Cmd, 0x10);//---set high column address
    WriteOneByte(oldeOp_Cmd, 0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    WriteOneByte(oldeOp_Cmd, 0x81);//--set contrast control register
    WriteOneByte(oldeOp_Cmd, 0xCF); // Set SEG Output Current Brightness
    WriteOneByte(oldeOp_Cmd, 0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    WriteOneByte(oldeOp_Cmd, 0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    WriteOneByte(oldeOp_Cmd, 0xA6);//--set normal display
    WriteOneByte(oldeOp_Cmd, 0xA8);//--set multiplex ratio(1 to 64)
    WriteOneByte(oldeOp_Cmd, 0x3f);//--1/64 duty
    WriteOneByte(oldeOp_Cmd, 0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    WriteOneByte(oldeOp_Cmd, 0x00);//-not offset
    WriteOneByte(oldeOp_Cmd, 0xd5);//--set display clock divide ratio/oscillator frequency
    WriteOneByte(oldeOp_Cmd, 0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    WriteOneByte(oldeOp_Cmd, 0xD9);//--set pre-charge period
    WriteOneByte(oldeOp_Cmd, 0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    WriteOneByte(oldeOp_Cmd, 0xDA);//--set com pins hardware configuration
    WriteOneByte(oldeOp_Cmd, 0x12);
    WriteOneByte(oldeOp_Cmd, 0xDB);//--set vcomh
    WriteOneByte(oldeOp_Cmd, 0x40);//Set VCOM Deselect Level
    WriteOneByte(oldeOp_Cmd, 0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    WriteOneByte(oldeOp_Cmd, 0x02);//
    WriteOneByte(oldeOp_Cmd, 0x8D);//--set Charge Pump enable/disable
    WriteOneByte(oldeOp_Cmd, 0x14);//--set(0x10) disable
    WriteOneByte(oldeOp_Cmd, 0xA4);// Disable Entire Display On (0xa4/0xa5)
    WriteOneByte(oldeOp_Cmd, 0xA6);// Disable Inverse Display On (0xa6/a7) 
    WriteOneByte(oldeOp_Cmd, 0xAF);//--turn on oled panel
    WriteOneByte(oldeOp_Cmd, 0xAF); /*display ON*/   
    
    
    Clear();
    SetPos(0,0);
    //uint8_t chr[] = "192.168.000.001 255.255.000.000 192.168.100.000";
    //ShowString(0, 0, chr);
    DrawBMP(0, 0, 128, 8, Logo_MO_2300D);
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
void Oled::Clear() {
    uint8_t i,n;		    
    for(i=0;i<8;i++)  
    {  
        WriteOneByte(oldeOp_Cmd, 0xb0+i);    //����ҳ��ַ��0~7��
        WriteOneByte(oldeOp_Cmd, 0x00);      //������ʾλ�á��е͵�ַ
        WriteOneByte(oldeOp_Cmd, 0x10);      //������ʾλ�á��иߵ�ַ   
        for(n=0;n<OledMaxColumn;n++)WriteOneByte(oledOp_Data, 0); 
    } //������ʾ
}

#if 0
void Oled::DrawPoint(uint8_t x,uint8_t y,uint8_t t) {

}

void Oled::Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot) {

}
#endif

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void Oled::ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
	uint8_t c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ			
	if(x>OledMaxColumn-1)
        {x=0;y=y+2;}

    if(OledCharSize == 16)
	{
        SetPos(x,y);	
        for(i=0;i<8;i++)
            WriteOneByte(oledOp_Data, F8X16[c*16+i]);
        SetPos(x,y+1);
        for(i=0;i<8;i++)
            WriteOneByte(oledOp_Data, F8X16[c*16+i+8]);
    }
    else {
   		SetPos(x,y+1);
		for(i=0;i<6;i++)
			WriteOneByte(oledOp_Data, F6x8[c][i]); 
    }
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	
void Oled::ShowNum(uint8_t x, uint8_t y,uint32_t num, uint8_t len, uint8_t size2) {
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
}

//��ʾһ���ַ��Ŵ�
void Oled::ShowString(uint8_t x, uint8_t y, const uint8_t *chr) {
	uint8_t j=0;
	while (chr[j]!='\0')
	{	ShowChar(x,y,chr[j]);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}

#if 0
void Oled::ShowCHinese(uint8_t x, uint8_t y, uint8_t no) {
	uint8_t t,adder=0;
	SetPos(x,y);	
    for(t=0;t<16;t++)
    {
        WriteOneByte(oledOp_Data,Hzk[2*no][t]);
        adder+=1;
    }	
	SetPos(x,y+1);	
    for(t=0;t<16;t++)
    {	
        WriteOneByte(oledOp_Data,Hzk[2*no+1][t]);
        adder+=1;
    }
}
#endif

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Oled::DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]) {
    uint32_t j=0;
    uint8_t x,y;

    if(y1%8==0) 
        y=y1/8;      
    else 
        y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        SetPos(x0,y);
        for(x=x0;x<x1;x++)
        {      
            WriteOneByte(oledOp_Data, BMP[j++]);	    	
        }
    }
}