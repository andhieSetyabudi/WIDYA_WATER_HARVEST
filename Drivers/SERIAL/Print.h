/*
 * Print.h
 *
 *  Created on: Mar 24, 2020
 *      Author: frdinawan
 */

#ifndef SERIAL_PRINT_H_
#define SERIAL_PRINT_H_

#include <inttypes.h>
#include <stdio.h> // for size_t

#ifndef VA_MACRO
#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

typedef struct Print
{
  int write_error;
} Print;

void Print___construct(Print *this);

void Print_setWriteError(Print *this, int err);
#define Print_setWriteError_1(s, t, a) Print_setWriteError(a, 1)
#define Print_setWriteError_2(s, t, a, b) Print_setWriteError(a, b)
#define Print_setWriteError(...) VA_MACRO(Print_setWriteError_, void, void, __VA_ARGS__)

int Print_getWriteError(Print *this);
void Print_clearWriteError(Print *this);

extern size_t Print_writeByte(Print *this, uint8_t);
size_t Print_write(Print *this, const uint8_t *buffer, size_t size);
#define Print_write_2(s, t, a, b) Print_write(a, b, strlen((const char *)(b)))
#define Print_write_3(s, t, a, b, c) Print_write(a, b, c)
#define Print_write(...) VA_MACRO(Print_write_, void, void, __VA_ARGS__)

extern int Print_availableForWrite(Print *this) __attribute__((weak));

size_t Print_print(Print *this, const char str[]);
size_t Print_printChar(Print *this, char);
size_t Print_printNumber(Print *this, long, int);
#define Print_printNumber_2(s, t, a, b) Print_printNumber(a, b, DEC)
#define Print_printNumber_3(s, t, a, b, c) Print_printNumber(a, b, c)
#define Print_printNumber(...) VA_MACRO(Print_printNumber_, void, void, __VA_ARGS__)

size_t Print_printFloat(Print *this, double, uint8_t);
#define Print_printFloat_2(s, t, a, b) Print_printFloat(a, b, 2)
#define Print_printFloat_3(s, t, a, b, c) Print_printFloat(a, b, c)
#define Print_printFloat(...) VA_MACRO(Print_printFloat_, void, void, __VA_ARGS__)

size_t Print_println(Print *this, const char c[]);
#define Print_println_1(s, t, a) Print_print(a, "\r\n")
#define Print_println_2(s, t, a, b) Print_println(a, b)
#define Print_println(...) VA_MACRO(Print_println_, void, void, __VA_ARGS__)

size_t Print_printlnChar(Print *this, char);
size_t Print_printlnNumber(Print *this, long, int);
#define Print_printlnNumber_2(s, t, a, b) Print_printlnNumber(a, b, DEC)
#define Print_printlnNumber_3(s, t, a, b, c) Print_printlnNumber(a, b, c)
#define Print_printlnNumber(...) VA_MACRO(Print_printlnNumber_, void, void, __VA_ARGS__)

size_t Print_printlnFloat(Print *this, double, int);
#define Print_printlnFloat_2(s, t, a, b) Print_printlnFloat(a, b, 2)
#define Print_printlnFloat_3(s, t, a, b, c) Print_printlnFloat(a, b, c)
#define Print_printlnFloat(...) VA_MACRO(Print_printlnFloat_, void, void, __VA_ARGS__)

extern void Print_flush(Print *this) __attribute__((weak));

#endif /* SERIAL_PRINT_H_ */
