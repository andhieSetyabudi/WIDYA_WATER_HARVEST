/*
* Serial.h
*
*	Originator by : Farid Inawan on March 24, 2020
*
*	Revised by Andhie setyabudi
*/

#ifndef SERIAL_HALSERIAL_H_
#define SERIAL_HALSERIAL_H_

#include <inttypes.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "Stream.h"

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
// NOTE: a "power of 2" buffer size is reccomended to dramatically
//       optimize all the modulo operations for ring buffers.
// WARNING: When buffer sizes are increased to > 256, the buffer index
// variables are automatically increased in size, but the extra
// atomicity guards needed for that are not implemented. This will
// often work, but occasionally a race condition can occur that makes
// Serial behave erratically. See https://github.com/arduino/Arduino/issues/2405
#ifndef HALSERIAL_RX_BUFFER_SIZE
#define HALSERIAL_RX_BUFFER_SIZE 256 // RX buffer size
#endif
#ifndef HALSERIAL_TX_BUFFER_SIZE
#define HALSERIAL_TX_BUFFER_SIZE 256 // TX buffer size
#endif
#if (HALSERIAL_TX_BUFFER_SIZE > 256)
typedef uint16_t hal_tx_buffer_index_t;
#else
typedef uint8_t hal_tx_buffer_index_t;
#endif
#if (HALSERIAL_RX_BUFFER_SIZE > 256)
typedef uint16_t hal_rx_buffer_index_t;
#else
typedef uint8_t hal_rx_buffer_index_t;
#endif

extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
extern void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

typedef struct
{
  int write_error;
  unsigned long _timeout;     // number of milliseconds to wait for the next char before aborting timed read
  unsigned long _startMillis; // used for timeout measurement

  UART_HandleTypeDef *huart;

  uint8_t _written;

  volatile hal_rx_buffer_index_t _rx_buffer_head;
  volatile hal_rx_buffer_index_t _rx_buffer_tail;
  volatile hal_tx_buffer_index_t _tx_buffer_head;
  volatile hal_tx_buffer_index_t _tx_buffer_tail;

  unsigned long (*millis)(void);
  unsigned char (*delay)(unsigned long value);

  unsigned char _rx_buffer[HALSERIAL_RX_BUFFER_SIZE];
  unsigned char _tx_buffer[HALSERIAL_TX_BUFFER_SIZE];
} HALSerial;

void HALSerial__construct(HALSerial *this);

void HALSerial_begin(HALSerial *this);
void HALSerial_end(HALSerial *this);

extern int HALSerial_available(HALSerial *this);
extern int HALSerial_peek(HALSerial *this);
extern int HALSerial_read(HALSerial *this);
extern int HALSerial_availableForWrite(HALSerial *this);
extern void HALSerial_flush(HALSerial *this);
extern size_t HALSerial_writeByte(HALSerial *this, uint8_t);

size_t HALSerial_write(HALSerial *this, const uint8_t *buffer, size_t size);
#define HALSerial_write_2(s, t, a, b) HALSerial_write(a, b, strlen((const char *)(b)))
#define HALSerial_write_3(s, t, a, b, c) HALSerial_write(a, b, c)
#define HALSerial_write(...) VA_MACRO(HALSerial_write_, void, void, __VA_ARGS__)

// Interrupt handlers - Not intended to be called externally
static void HALSerial__rx_complete_irq(HALSerial *this);
static void HALSerial__tx_complete_irq(HALSerial *this);

/* Parent */

extern unsigned long HALSerial_millis(HALSerial *this);
extern unsigned char HALSerial_delay(HALSerial *this, unsigned long value);

int HALSerial_timedRead(HALSerial *this); // read stream with timeout

size_t HALSerial_readBytes(HALSerial *this, char *buffer, size_t length); // read chars from stream into buffer
// terminates if length characters have been read or timeout (see setTimeout)
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t HALSerial_readBytesUntil(HALSerial *this, char terminator, char *buffer, size_t length); // as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)



/*
 *  method for serial 4
 *
 */
static void Serial4_begin(uint32_t baudrate);
static void Serial4_setTimeout(uint32_t timeout);
typedef struct Serial
{
  HALSerial serialParam;

  void (*begin) (uint32_t baudrate);
  void (*end) 	(void);

  int (*available)		(void);
  int (*peek)			(void);
  int (*availableForWrite)	(void);
  void (*flush)			(void);

  size_t (*write)		(uint8_t);
  size_t (*puts)		(const uint8_t *buffer, size_t size);
  size_t (*print)		(const char fmt[], ...);
  int (*read)			(void);
  size_t (*readUntil)		(char terminator, char *buffer, size_t length);
  int (*readTimeout)		(void);
  size_t (*gets)		(char *buffer, size_t length);

  void (*setTimeout)		(uint32_t timeout);

}Serial;

extern Serial Serial1;
#endif /* SERIAL_HALSERIAL_H_ */
