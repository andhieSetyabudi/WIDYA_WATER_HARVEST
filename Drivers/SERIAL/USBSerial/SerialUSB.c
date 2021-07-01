/*
 * SerialUSB.c
 *
 *  Created on: Aug 11, 2020
 *      Author: Andhie Setyabudi
 */


#include "SerialUSB.h"
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>

#include "stdbool.h"
#ifdef __CDC_QUEUE_H
  #include "cdc_queue.h"
#else
  #include "USBSerial/cdc_queue.h"
#endif

extern CDC_TransmitQueue_TypeDef TransmitQueue;
extern CDC_ReceiveQueue_TypeDef ReceiveQueue;

static void SerialUSB_begin(void)
{
  CDC_init();
}

static void SerialUSB_end(void)
{
  CDC_DeInit();
}

static int SerialUSB_availableForWrite()
{
  return CDC_TransmitQueue_WriteSize(&TransmitQueue);
}

static size_t SerialUSB_write(const uint8_t c)
{
  return SerialUSB_puts(&c, 1);
}

static size_t SerialUSB_puts(const uint8_t *buf, size_t len)
{
  size_t rest = len;
    while (rest > 0 && CDC_connected()) {
      // Determine buffer size available for write
      size_t portion = (size_t)CDC_TransmitQueue_WriteSize(&TransmitQueue);
      // Truncate it to content size (if rest is greater)
      if (rest < portion) {
        portion = rest;
      }
      if (portion > 0) {
        // Only if some space in the buffer exists.
        // TS: Only main thread calls write and writeSize methods,
        // it's thread-safe since IRQ does not affects
        // TransmitQueue write position
        CDC_TransmitQueue_Enqueue(&TransmitQueue, buf, portion);
        rest -= portion;
        buf += portion;
        // After storing data, start transmitting process
        CDC_continue_transmit();
      }
    }
    return (len - rest);
}

static int SerialUSB_available(void)
{
  return (CDC_ReceiveQueue_ReadSize(&ReceiveQueue));
}

static int SerialUSB_read(void)
{
  // Dequeue only one char from queue
  // TS: it safe, because only main thread affects ReceiveQueue->read pos
  int ch = CDC_ReceiveQueue_Dequeue(&ReceiveQueue);
  // Resume receive process, if possible
  CDC_resume_receive();
  return ch;
}

static size_t SerialUSB_readbytes(char *buffer, size_t length)
{
  uint16_t read;
    auto rest = (uint16_t)(length);
    uint32_t _startMillis = HAL_GetTick();
    do {
      read = CDC_ReceiveQueue_Read(&ReceiveQueue, (uint8_t *)(buffer), rest);
      CDC_resume_receive();
      rest -= read;
      buffer += read;
      if (rest == 0) {
        return length;
      }
    } while (HAL_GetTick() - _startMillis < USB_CDC_TIMEOUT);
    return length - rest;
}

static size_t SerialUSB_readBytesUntil(char terminator, char *buffer, size_t length)
{
  uint16_t read;
    auto rest = (uint16_t)(length);
    uint32_t _startMillis = HAL_GetTick();
    do {
      bool found = CDC_ReceiveQueue_ReadUntil(&ReceiveQueue, (uint8_t)(terminator),
                                              (uint8_t*)(buffer), rest, &read);
      CDC_resume_receive();
      rest -= read;
      buffer += read;
      if (found) {
        return length - rest;
      }
      if (rest == 0) {
        return length;
      }
    } while (HAL_GetTick() - _startMillis < USB_CDC_TIMEOUT);
    return length - rest;
}

static int SerialUSB_peek(void)
{
  // Peek one symbol, it can't change receive avaiablity
    return CDC_ReceiveQueue_Peek(&ReceiveQueue);
}

static void SerialUSB_flush(void)
{
  // Wait for TransmitQueue read size becomes zero
  // TS: safe, because it not be stopped while receive 0
  while (CDC_TransmitQueue_ReadSize(&TransmitQueue) > 0) {}
}



static size_t SerialUSB_print(const char fmt[], ...)
{
	char serialusb_buffer_print[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(serialusb_buffer_print, sizeof(serialusb_buffer_print), fmt, args);
	va_end(args);
	size_t ret = SerialUSB_puts(serialusb_buffer_print, strlen(serialusb_buffer_print));
	return ret;
}

SERIAL_USB USBSerial =
{
    .begin = SerialUSB_begin,
    .end   = SerialUSB_end,
    .availableForWrite = SerialUSB_availableForWrite,
    .write	= SerialUSB_write,
    .puts	= SerialUSB_puts,

    .available = SerialUSB_available,
    .read	= SerialUSB_read,
    .gets	= SerialUSB_readbytes,
    .readBytesUntil = SerialUSB_readBytesUntil,

	.print  = SerialUSB_print,
    .peek 	= SerialUSB_peek,
    .flush 	= SerialUSB_flush,
};
