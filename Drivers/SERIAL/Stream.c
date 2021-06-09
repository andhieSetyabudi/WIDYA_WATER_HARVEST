/*
 * Stream.c
 *
 *  Created on: Mar 24, 2020
 *      Author: frdinawan
 */

#include "Stream.h"

#define PARSE_TIMEOUT 1000 // default number of milli-seconds to wait

#define TRUE 1
#define FALSE 0

#undef Stream_parseInt_2
#define Stream_parseInt_2(s, t, a, b) Stream_parseInt(a, SKIP_ALL, b)
#undef Stream_parseFloat_2
#define Stream_parseFloat_2(s, t, a, b) Stream_parseFloat(a, SKIP_ALL, b)
// These overload exists for compatibility with any class that has derived
// Stream and used parseFloat/Int with a custom ignore character. To keep
// the public API simple, these overload remains protected.

void Stream___construct(Stream *this)
{
  Print___construct((Print *)this);
  this->_timeout = 1000;
}

// protected method to read stream with timeout
int Stream_timedRead(Stream *this)
{
  int c;
  this->_startMillis = Stream_millis(this);
  do
  {
    c = Stream_read(this);
    if (c >= 0)
      return c;
  } while (Stream_millis(this) - this->_startMillis < this->_timeout);
  return -1; // -1 indicates timeout
}

// protected method to peek stream with timeout
int Stream_timedPeek(Stream *this)
{
  int c;
  this->_startMillis = Stream_millis(this);
  do
  {
    c = Stream_peek(this);
    if (c >= 0)
      return c;
  } while (Stream_millis(this) - this->_startMillis < this->_timeout);
  return -1; // -1 indicates timeout
}

// returns peek of the next digit in the stream or -1 if timeout
// discards non-numeric characters
int Stream_peekNextDigit(Stream *this, LookaheadMode lookahead, uint8_t detectDecimal)
{
  int c;
  while (1)
  {
    c = Stream_timedPeek(this);

    if (c < 0 ||
        c == '-' ||
        (c >= '0' && c <= '9') ||
        (detectDecimal && c == '.'))
      return c;

    switch (lookahead)
    {
    case SKIP_NONE:
      return -1; // Fail code.
    case SKIP_WHITESPACE:
      switch (c)
      {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        break;
      default:
        return -1; // Fail code.
      }
    case SKIP_ALL:
      break;
    }
    Stream_read(this); // discard non-numeric
  }
}

// Public Methods
//////////////////////////////////////////////////////////////

void Stream_setTimeout(Stream *this, unsigned long timeout) // sets the maximum number of milliseconds to wait
{
  this->_timeout = timeout;
}

unsigned long Stream_getTimeout(Stream *this)
{
  return this->_timeout;
}

// reads data from the stream until the target string of given length is found
// returns TRUE if target string is found, FALSE if timed out
uint8_t Stream_find(Stream *this, char *target, size_t length)
{
  return Stream_findUntil(this, target, length, NULL, 0);
}

uint8_t Stream_findChar(Stream *this, char target)
{
  return Stream_find(this, &target, 1);
}

// reads data from the stream until the target string of the given length is found
// search terminated if the terminator string is found
// returns TRUE if target string is found, FALSE if terminated or timed out
uint8_t Stream_findUntil(Stream *this, char *target, size_t targetLen, char *terminator, size_t termLen)
{
  if (terminator == NULL)
  {
    Stream_MultiTarget t[1] = {{target, targetLen, 0}};
    return Stream_findMulti(this, t, 1) == 0 ? TRUE : FALSE;
  }
  else
  {
    Stream_MultiTarget t[2] = {{target, targetLen, 0}, {terminator, termLen, 0}};
    return Stream_findMulti(this, t, 2) == 0 ? TRUE : FALSE;
  }
}

// returns the first valid (long) integer value from the current position.
// lookahead determines how parseInt looks ahead in the stream.
// See LookaheadMode enumeration at the top of the file.
// Lookahead is terminated by the first character that is not a valid part of an integer.
// Once parsing commences, 'ignore' will be skipped in the stream.
long Stream_parseInt(Stream *this, LookaheadMode lookahead, char ignore)
{
  uint8_t isNegative = FALSE;
  long value = 0;
  int c;

  c = Stream_peekNextDigit(this, lookahead, FALSE);
  // ignore non numeric leading characters
  if (c < 0)
    return 0; // zero returned if timeout

  do
  {
    if (c == ignore)
      ; // ignore this character
    else if (c == '-')
      isNegative = TRUE;
    else if (c >= '0' && c <= '9') // is c a digit?
      value = value * 10 + c - '0';
    Stream_read(this); // consume the character we got with peek
    c = Stream_timedPeek(this);
  } while ((c >= '0' && c <= '9') || c == ignore);

  if (isNegative)
    value = -value;
  return value;
}

// as parseInt but returns a floating point value
float Stream_parseFloat(Stream *this, LookaheadMode lookahead, char ignore)
{
  uint8_t isNegative = FALSE;
  uint8_t isFraction = FALSE;
  long value = 0;
  int c;
  float fraction = 1.0;

  c = Stream_peekNextDigit(this, lookahead, TRUE);
  // ignore non numeric leading characters
  if (c < 0)
    return 0; // zero returned if timeout

  do
  {
    if (c == ignore)
      ; // ignore
    else if (c == '-')
      isNegative = TRUE;
    else if (c == '.')
      isFraction = TRUE;
    else if (c >= '0' && c <= '9')
    { // is c a digit?
      value = value * 10 + c - '0';
      if (isFraction)
        fraction *= 0.1;
    }
    Stream_read(this); // consume the character we got with peek
    c = Stream_timedPeek(this);
  } while ((c >= '0' && c <= '9') || (c == '.' && !isFraction) || c == ignore);

  if (isNegative)
    value = -value;
  if (isFraction)
    return value * fraction;
  else
    return value;
}

// read characters from stream into buffer
// terminates if length characters have been read, or timeout (see setTimeout)
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
//
size_t Stream_readBytes(Stream *this, char *buffer, size_t length)
{
  size_t count = 0;
  while (count < length)
  {
    int c = Stream_timedRead(this);
    if (c < 0)
      break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}

// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t Stream_readBytesUntil(Stream *this, char terminator, char *buffer, size_t length)
{
  size_t index = 0;
  while (index < length)
  {
    int c = Stream_timedRead(this);
    if (c < 0 || c == terminator)
      break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

// String Stream_readString(Stream *this)
// {
//   String ret;
//   int c = Stream_timedRead(this);
//   while (c >= 0)
//   {
//     ret += (char)c;
//     c = Stream_timedRead(this);
//   }
//   return ret;
// }

// String Stream_readStringUntil(Stream *this, char terminator)
// {
//   String ret;
//   int c = Stream_timedRead(this);
//   while (c >= 0 && c != terminator)
//   {
//     ret += (char)c;
//     c = Stream_timedRead(this);
//   }
//   return ret;
// }

int Stream_findMulti(Stream *this, struct Stream_MultiTarget *targets, int tCount)
{
  // any zero length target string automatically matches and would make
  // a mess of the rest of the algorithm.
  for (struct Stream_MultiTarget *t = targets; t < targets + tCount; ++t)
  {
    if (t->len <= 0)
      return t - targets;
  }

  while (1)
  {
    int c = Stream_timedRead(this);
    if (c < 0)
      return -1;

    for (struct Stream_MultiTarget *t = targets; t < targets + tCount; ++t)
    {
      // the simple case is if we match, deal with that first.
      if (c == t->str[t->index])
      {
        if (++t->index == t->len)
          return t - targets;
        else
          continue;
      }

      // if not we need to walk back and see if we could have matched further
      // down the stream (ie '1112' doesn't match the first position in '11112'
      // but it will match the second position so we can't just reset the current
      // index to 0 when we find a mismatch.
      if (t->index == 0)
        continue;

      int origIndex = t->index;
      do
      {
        --t->index;
        // first check if current char works against the new current index
        if (c != t->str[t->index])
          continue;

        // if it's the only char then we're good, nothing more to check
        if (t->index == 0)
        {
          t->index++;
          break;
        }

        // otherwise we need to check the rest of the found string
        int diff = origIndex - t->index;
        size_t i;
        for (i = 0; i < t->index; ++i)
        {
          if (t->str[i] != t->str[i + diff])
            break;
        }

        // if we successfully got through the previous loop then our current
        // index is good.
        if (i == t->index)
        {
          t->index++;
          break;
        }

        // otherwise we just try the next index
      } while (t->index);
    }
  }
  // unreachable
  return -1;
}
