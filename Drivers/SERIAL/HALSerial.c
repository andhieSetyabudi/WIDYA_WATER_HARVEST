/*
 * Serial.c
 *
 *  Created on: Mar 24, 2020
 *      Author: frdinawan
 */

#include "HALSerial.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>
#include "main.h"

#define TRUE 1
#define FALSE 0

// macro to guard critical sections when needed for large TX buffer sizes
#if (HALSERIAL_TX_BUFFER_SIZE > 512)
#define TX_BUFFER_ATOMIC ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
#else
#define TX_BUFFER_ATOMIC
#endif

extern UART_HandleTypeDef huart1;
//===================================
//===================================
#define  begin() begin(9600)

static void Serial1_begin(uint32_t baudrate)
{
  HALSerial__construct (&Serial1.serialParam, &huart1);
  Serial1.serialParam._written = FALSE;
//  HAL_UART_Init(this->huart);
  while(HAL_UART_Receive_IT(Serial1.serialParam.huart/*&huart1*/, Serial1.serialParam._rx_buffer + Serial1.serialParam._rx_buffer_head, 1) == HAL_BUSY);

  Serial1.serialParam.huart->Init.BaudRate = baudrate;
  HALSerial_begin(&Serial1.serialParam);
  HAL_UART_Receive_IT(&Serial1.serialParam.huart, Serial1.serialParam._rx_buffer, HALSERIAL_RX_BUFFER_SIZE);
  HAL_UART_Transmit_IT(&Serial1.serialParam.huart, Serial1.serialParam._tx_buffer, HALSERIAL_TX_BUFFER_SIZE);
}

static void Serial1_setTimeout(uint32_t timeout)
{
  Serial1.serialParam._timeout = timeout;
}

static void Serial1_end(void)	 			{HALSerial_end(&Serial1.serialParam);}
static int Serial1_available(void)			{return HALSerial_available(&Serial1.serialParam);}
static int Serial1_peek(void)				{return HALSerial_peek(&Serial1.serialParam);}
static int Serial1_availableForWrite(void)	{return HALSerial_availableForWrite(&Serial1.serialParam);}
static void Serial1_flush(void)				{HALSerial_flush(&Serial1.serialParam);}
static size_t Serial1_write(uint8_t c)		{return HALSerial_writeByte(&Serial1.serialParam, c);}
static size_t Serial1_puts(const uint8_t *str, size_t size) {return HALSerial_write(&Serial1.serialParam, str, size);}
static int Serial1_read(void)				{return HALSerial_read(&Serial1.serialParam);}
static size_t Serial1_readUntil(char terminator, char *buffer, size_t length) {return HALSerial_readBytesUntil(&Serial1.serialParam,terminator,buffer, length);}
static int Serial1_readTimeout(void)		{return HALSerial_timedRead(&Serial1.serialParam);}
static size_t Serial1_gets(char* buffer, size_t len) {return HALSerial_readBytes(&Serial1.serialParam, buffer, len);}

static size_t Serial1_readbytes(char* buffer, size_t len)
{
	return Serial1_readUntil(NULL,buffer, len);
//	return HALSerial_readBytes(&Serial1.serialParam, buffer, len);
}

char serial_buffer_print[256];
static size_t Serial1_print(const char fmt[], ...)
{
  va_list args;
  va_start(args, fmt);
  vsnprintf(serial_buffer_print, sizeof(serial_buffer_print), fmt, args);
  va_end(args);
  size_t ret = Serial1_puts(serial_buffer_print, strlen(serial_buffer_print));
  return ret;
}

Serial Serial1 =
{
    .begin 				= Serial1_begin,
    .setTimeout 		= Serial1_setTimeout,
    .end				= Serial1_end,
    .available  		= Serial1_available,
    .peek				= Serial1_peek,
    .availableForWrite 	= Serial1_availableForWrite,
    .flush				= Serial1_flush,
    .write				= Serial1_write,
    .puts				= Serial1_puts,

    .read				= Serial1_read,
    .readUntil 			= Serial1_readUntil,
    .readTimeout 		= Serial1_readTimeout,
    .gets				= Serial1_gets,
    .print				= Serial1_print,

	.readBytes			= Serial1_readbytes,

    .serialParam = {
	    .huart = &huart1,
	    .millis = HAL_GetTick,
	  #ifdef  CMSIS_OS_H_ | INC_FREERTOS_H
	    .delay = osDelay,
	  #else
	    .delay = HAL_Delay,
	  #endif
	  },
};




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if( huart != NULL )
	{
		if (huart->Instance == USART1)
			HALSerial__rx_complete_irq(&Serial1.serialParam);
		if (huart->Instance == USART2)
			HALSerial__rx_complete_irq(&Serial1.serialParam);
	}

}

/**
  * @brief  Rx Half Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if ( huart == NULL)
		return;
  if (huart->Instance == USART1)
	HALSerial__tx_complete_irq(&Serial1.serialParam);
}


void HALSerial__construct(HALSerial *this, UART_HandleTypeDef *huart_)
{
  Stream___construct((Stream *)this);
  this->_rx_buffer_head = 0;
  this->_rx_buffer_tail = 0;
  this->_tx_buffer_head = 0;
  this->_tx_buffer_tail = 0;
  this-> huart = huart_;//&huart1;
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

static void HALSerial__rx_complete_irq(HALSerial *this)
{
  hal_rx_buffer_index_t i = ((unsigned int)(HALSERIAL_RX_BUFFER_SIZE + this->huart->pRxBuffPtr - this->_rx_buffer)) % HALSERIAL_RX_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != this->_rx_buffer_tail)
    this->_rx_buffer_head = i;
  while(HAL_UART_Receive_IT(this->huart, this->_rx_buffer + this->_rx_buffer_head, 1) == HAL_BUSY);
}

static void HALSerial__tx_complete_irq(HALSerial *this) {
  this->_tx_buffer_tail = ((unsigned int)(HALSERIAL_TX_BUFFER_SIZE + this->huart->pTxBuffPtr - this->_tx_buffer)) % HALSERIAL_TX_BUFFER_SIZE;

  uint16_t size;
  if (this->_tx_buffer_head < this->_tx_buffer_tail)
    size = HALSERIAL_TX_BUFFER_SIZE - this->_tx_buffer_tail;
  else
    size = this->_tx_buffer_head - this->_tx_buffer_tail;

  if (size > 0)
    while(HAL_UART_Transmit_IT(this->huart, this->_tx_buffer + this->_tx_buffer_tail, size) == HAL_BUSY);
}

// Public Methods //////////////////////////////////////////////////////////////

void HALSerial_begin(HALSerial *this)
{
  this->_written = FALSE;
  HAL_UART_Init(this->huart);
  while(HAL_UART_Receive_IT(this->huart, this->_rx_buffer + this->_rx_buffer_head, 1) == HAL_BUSY);
}

void HALSerial_end(HALSerial *this)
{
  // wait for transmission of outgoing data
  HALSerial_flush(this);

  HAL_UART_DeInit(this->huart);
}

int __attribute__((weak)) HALSerial_available(HALSerial *this)
{
  return ((unsigned int)(HALSERIAL_RX_BUFFER_SIZE + this->_rx_buffer_head - this->_rx_buffer_tail)) % HALSERIAL_RX_BUFFER_SIZE;
}

int __attribute__((weak)) HALSerial_peek(HALSerial *this)
{
  if (this->_rx_buffer_head == this->_rx_buffer_tail)
  {
    return -1;
  }
  else
  {
    return this->_rx_buffer[this->_rx_buffer_tail];
  }
}

int __attribute__((weak)) HALSerial_read(HALSerial *this)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (this->_rx_buffer_head == this->_rx_buffer_tail)
  {
    return -1;
  }
  else
  {
    unsigned char c = this->_rx_buffer[this->_rx_buffer_tail];
    this->_rx_buffer_tail = (hal_rx_buffer_index_t)(this->_rx_buffer_tail + 1) % HALSERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

int __attribute__((weak)) HALSerial_availableForWrite(HALSerial *this)
{
  hal_tx_buffer_index_t head;
  hal_tx_buffer_index_t tail;

  TX_BUFFER_ATOMIC
  {
    head = this->_tx_buffer_head;
    tail = this->_tx_buffer_tail;
  }
  if (head >= tail)
    return HALSERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  return tail - head - 1;
}

void __attribute__((weak)) HALSerial_flush(HALSerial *this)
{
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!this->_written)
    return;

  unsigned long ticks = HALSerial_millis(this);
  while (this->_tx_buffer_head != this->_tx_buffer_tail && HALSerial_millis(this) - ticks < 1000) {
    HALSerial_delay(this, 1);
  }
}

size_t __attribute__((weak)) HALSerial_writeByte(HALSerial *this, uint8_t c)
{
  this->_written = TRUE;

  hal_tx_buffer_index_t i = (this->_tx_buffer_head + 1) % HALSERIAL_TX_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == this->_tx_buffer_tail) {
    HALSerial_delay(this, 1);
  }

  this->_tx_buffer[this->_tx_buffer_head] = c;

  if (this->_tx_buffer_head == this->_tx_buffer_tail) {
    while(HAL_UART_Transmit_IT(this->huart, this->_tx_buffer + this->_tx_buffer_tail, 1) == HAL_BUSY)
      HALSerial_delay(this, 1);
  }

  // make atomic to prevent execution of ISR between setting the
  // head pointer and setting the interrupt flag resulting in buffer
  // retransmission
  TX_BUFFER_ATOMIC
  {
    this->_tx_buffer_head = i;
  }

  return 1;
}

size_t HALSerial_write(HALSerial *this, const uint8_t *buffer, size_t size)
{
  if (buffer == NULL)
    return 0;
  size_t n = 0;
  while (size--)
  {
    if (HALSerial_writeByte(this, *buffer++))
      n++;
    else
      break;
  }
//  HAL_UART_Transmit(this->huart, buffer, size, 100);
//  HALSerial__rx_complete_irq(this); // ini tadi lupa di comment
  return n;
}

/* Parent */

unsigned long HALSerial_millis(HALSerial *this) {
  return this->millis();
}

unsigned char HALSerial_delay(HALSerial *this, unsigned long value) {
  return this->delay(value);
}

// protected method to read stream with timeout
int HALSerial_timedRead(HALSerial *this)
{
  int c;
  this->_startMillis = HALSerial_millis(this);
  do
  {
    c = HALSerial_read(this);
    if (c >= 0)
      return c;
    else HALSerial_delay(this, 1);
  } while (HALSerial_millis(this) - this->_startMillis < this->_timeout);
  return -1; // -1 indicates timeout
}

// read characters from stream into buffer
// terminates if length characters have been read, or timeout (see setTimeout)
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
//
size_t HALSerial_readBytes(HALSerial *this, char *buffer, size_t length)
{
  size_t count = 0;
  while (count < length)
  {
    int c = HALSerial_timedRead(this);
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

size_t HALSerial_readBytesUntil(HALSerial *this, char terminator, char *buffer, size_t length)
{
  size_t index = 0;
  while (index < length)
  {
    int c = HALSerial_timedRead(this);
    if (c < 0 || c == terminator)
      break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}





