/*
 * Stream.h
 *
 *  Created on: Mar 24, 2020
 *      Author: frdinawan
 */

#ifndef SERIAL_STREAM_H_
#define SERIAL_STREAM_H_

#include <inttypes.h>

#include "Print.h"

// This enumeration provides the lookahead options for parseInt(), parseFloat()
// The rules set out here are used until either the first valid character is found
// or a time out occurs due to lack of input.
typedef enum LookaheadMode
{
  SKIP_ALL,       // All invalid characters are ignored.
  SKIP_NONE,      // Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
  SKIP_WHITESPACE // Only tabs, spaces, line feeds & carriage returns are skipped.
} LookaheadMode;

#define NO_IGNORE_CHAR '\x01' // a char not found in a valid ASCII numeric field

typedef struct Stream
{
  int write_error;
  unsigned long _timeout;     // number of milliseconds to wait for the next char before aborting timed read
  unsigned long _startMillis; // used for timeout measurement
} Stream;

typedef struct Stream_MultiTarget
{
  const char *str; // string you're searching for
  size_t len;      // length of string you're searching for
  size_t index;    // index used by the search routine.
} Stream_MultiTarget;

// compatibility macros for testing
/*
#define   getInt()            parseInt()
#define   getInt(ignore)    parseInt(ignore)
#define   getFloat()          parseFloat()
#define   getFloat(ignore)  parseFloat(ignore)
#define   getString( pre_string, post_string, buffer, length)
readBytesBetween( pre_string, terminator, buffer, length)
*/

void Stream___construct(Stream *this);

extern unsigned long Stream_millis(Stream *this);
extern unsigned long Stream_delay(Stream *this);

extern int Stream_available(Stream *this);
extern int Stream_read(Stream *this);
extern int Stream_peek(Stream *this);

int Stream_timedRead(Stream *this);                                                     // read stream with timeout
int Stream_timedPeek(Stream *this);                                                     // peek stream with timeout
int Stream_peekNextDigit(Stream *this, LookaheadMode lookahead, uint8_t detectDecimal); // returns the next numeric digit in the stream or -1 if timeout

void Stream_setTimeout(Stream *this, unsigned long timeout); // sets maximum milliseconds to wait for stream data, default is 1 second
unsigned long Stream_getTimeout(Stream *this);

uint8_t Stream_find(Stream *this, char *target, size_t length); // reads data from the stream until the target string of given length is found
#define Stream_find_2(s, t, a, b) Stream_find(a, b, strlen(b))
#define Stream_find_3(s, t, a, b, c) Stream_find(a, b, c)
#define Stream_find(...) VA_MACRO(Stream_find_, void, void, __VA_ARGS__)
// returns true if target string is found, false if timed out

uint8_t Stream_findChar(Stream *this, char target);

uint8_t Stream_findUntil(Stream *this, char *target, size_t targetLen, char *terminate, size_t termLen); // as above but search ends if the terminate string is found
#define Stream_findUntil_3(s, t, a, b, c) Stream_findUntil(a, b, strlen(b), c, strlen(c))
#define Stream_findUntil_5(s, t, a, b, c, d, e) Stream_findUntil(a, b, c, d, e)
#define Stream_findUntil(...) VA_MACRO(Stream_findUntil_, void, void, __VA_ARGS__)

long Stream_parseInt(Stream *this, LookaheadMode lookahead, char ignore);
#define Stream_parseInt_1(s, t, a) Stream_parseInt(a, SKIP_ALL, NO_IGNORE_CHAR)
#define Stream_parseInt_2(s, t, a, b) Stream_parseInt(a, b, NO_IGNORE_CHAR)
#define Stream_parseInt_3(s, t, a, b, c) Stream_parseInt(a, b, c)
#define Stream_parseInt(...) VA_MACRO(Stream_parseInt_, void, void, __VA_ARGS__)
// returns the first valid (long) integer value from the current position.
// lookahead determines how parseInt looks ahead in the stream.
// See LookaheadMode enumeration at the top of the file.
// Lookahead is terminated by the first character that is not a valid part of an integer.
// Once parsing commences, 'ignore' will be skipped in the stream.

float Stream_parseFloat(Stream *this, LookaheadMode lookahead, char ignore);
#define Stream_parseFloat_1(s, t, a) Stream_parseFloat(a, SKIP_ALL, NO_IGNORE_CHAR)
#define Stream_parseFloat_2(s, t, a, b) Stream_parseFloat(a, b, NO_IGNORE_CHAR)
#define Stream_parseFloat_3(s, t, a, b, c) Stream_parseFloat(a, b, c)
#define Stream_parseFloat(...) VA_MACRO(Stream_parseFloat_, void, void, __VA_ARGS__)
// float version of parseInt

size_t Stream_readBytes(Stream *this, char *buffer, size_t length); // read chars from stream into buffer
// terminates if length characters have been read or timeout (see setTimeout)
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t Stream_readBytesUntil(Stream *this, char terminator, char *buffer, size_t length); // as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)

// Arduino String functions to be added here
// String Stream_readString(Stream *this);
// String Stream_readStringUntil(Stream *this, char terminator);

/* Protected */

// This allows you to search for an arbitrary number of strings.
// Returns index of the target that is found first or -1 if timeout occurs.
int Stream_findMulti(Stream *this, struct Stream_MultiTarget *targets, int tCount);

#undef NO_IGNORE_CHAR
#endif /* SERIAL_STREAM_H_ */
