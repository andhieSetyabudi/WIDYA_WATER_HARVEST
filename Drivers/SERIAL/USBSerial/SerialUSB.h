/*
 * SerialUSB.h
 *
 *  Created on: Aug 11, 2020
 *      Author: Andhie Setyabudi
 */

#ifndef IO_SUPPORT_PERIPHERAL_SERIAL_USBSERIAL_SERIALUSB_H_
#define IO_SUPPORT_PERIPHERAL_SERIAL_USBSERIAL_SERIALUSB_H_

#include <inttypes.h>
#include <string.h>
//#include <USBSerial/usbd_cdc_if_.h>

#include "stm32f4xx_hal.h"

// USB library dependent
#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
//#include "cdc_queue.h"
#include "usbd_cdc_if.h"
#include "../Stream.h"

#define USB_CDC_TIMEOUT 50


extern __IO  uint32_t lineState;

static void SerialUSB_begin(void);
static void SerialUSB_end(void);

static int SerialUSB_availableForWrite();

static size_t SerialUSB_write(const uint8_t c);
static size_t SerialUSB_puts(const uint8_t *buf, size_t len);

static int SerialUSB_available(void);

static int SerialUSB_read(void);
static size_t SerialUSB_readbytes(char *buffer, size_t length);
static size_t SerialUSB_readBytesUntil(char terminator, char *buffer, size_t length);


static int SerialUSB_peek(void);
static void SerialUSB_flush(void);

typedef struct SERIALUSB
{
  void(*begin)	();
  void (*end)	();
  int (*availableForWrite) 	();
  size_t (*write)		(const uint8_t c);
  size_t (*puts)		(const uint8_t *buf, size_t len);
  int (*available)		(void);
  int (*read)			(void);
  size_t(*gets)			(char *buffer, size_t length);
  size_t (*readBytesUntil)	(char terminator, char *buffer, size_t length);

  int (*peek)			(void);
  void(*flush)			(void);
}SERIAL_USB;

extern SERIAL_USB USBSerial;
#endif /* IO_SUPPORT_PERIPHERAL_SERIAL_USBSERIAL_SERIALUSB_H_ */
