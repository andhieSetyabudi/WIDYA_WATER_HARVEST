/*
 * Print.c
 *
 *  Created on: Mar 24, 2020
 *      Author: frdinawan
 */

#include "Print.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


// Private Methods //////////////////////////////////////////////////////////////

size_t Print_printNumberUnsigned(Print *this, unsigned long, uint8_t);
size_t Print_printFloat(Print *this, double, uint8_t);

// Protected Methods //////////////////////////////////////////////////////////////

void Print_setWriteError(Print *this, int err)
{
  this->write_error = err;
}

// Public Methods //////////////////////////////////////////////////////////////

void Print___construct(Print *this)
{
  this->write_error = 0;
}

int Print_getWriteError(Print *this)
{
  return this->write_error;
}

void Print_clearWriteError(Print *this)
{
  Print_setWriteError(this, 0);
}

/* default implementation: may be overridden */
size_t Print_write(Print *this, const uint8_t *buffer, size_t size)
{
  if (buffer == NULL)
    return 0;
  size_t n = 0;
  while (size--)
  {
    if (Print_writeByte(this, *buffer++))
      n++;
    else
      break;
  }
  return n;
}

int __attribute__((weak)) Print_availableForWrite(Print *this)
{
  return 0;
}

size_t Print_print(Print *this, const char str[])
{
  return Print_write(this, (const uint8_t *)str);
}

size_t Print_printChar(Print *this, char c)
{
  return Print_writeByte(this, (uint8_t) c);
}

size_t Print_printNumber(Print *this, long n, int base)
{
  if (base == 0)
  {
    return Print_writeByte(this, (uint8_t) n);
  }
  else if (base == 10)
  {
    if (n < 0)
    {
      int t = Print_printChar(this, '-');
      n = -n;
      return Print_printNumberUnsigned(this, n, 10) + t;
    }
    return Print_printNumberUnsigned(this, n, 10);
  }
  else
  {
    return Print_printNumberUnsigned(this, n, base);
  }
}

size_t Print_println(Print *this, const char c[])
{
  size_t n = Print_print(this, c);
  n += Print_println(this);
  return n;
}

size_t Print_printlnChar(Print *this, char c)
{
  size_t n = Print_printChar(this, c);
  n += Print_println(this);
  return n;
}

size_t Print_printlnNumber(Print *this, long num, int base)
{
  size_t n = Print_printNumber(this, num, base);
  n += Print_println(this);
  return n;
}

size_t Print_printlnFloat(Print *this, double num, int digits)
{
  size_t n = Print_printFloat(this, num, digits);
  n += Print_println(this);
  return n;
}

void __attribute__((weak)) Print_flush(Print *this) {
  return;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print_printNumberUnsigned(Print *this, unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2)
    base = 10;

  do
  {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return Print_write(this, (const uint8_t *) str);
}

size_t Print_printFloat(Print *this, double number, uint8_t digits)
{
  size_t n = 0;

  if (isnan(number))
    return Print_print(this, "nan");
  if (isinf(number))
    return Print_print(this, "inf");
  if (number > 4294967040.0)
    return Print_print(this, "ovf"); // constant determined empirically
  if (number < -4294967040.0)
    return Print_print(this, "ovf"); // constant determined empirically

  // Handle negative numbers
  if (number < 0.0)
  {
    n += Print_printChar(this, '-');
    number = -number;
  }

  // Round correctly so that Print_print(this, 1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += Print_printNumber(this, int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
  {
    n += Print_printChar(this, '.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += Print_printNumber(this, toPrint);
    remainder -= toPrint;
  }

  return n;
}
