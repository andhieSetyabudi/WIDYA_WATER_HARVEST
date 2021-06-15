/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
//#include "../SERIAL/USBSerial/cdc_queue.h"
#include "cdc_queue.h"
#include "stdbool.h"
   extern CDC_TransmitQueue_TypeDef TransmitQueue;
   extern CDC_ReceiveQueue_TypeDef ReceiveQueue;
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define CDC_POLLING_INTERVAL             2 /* in ms. The max is 65 and the min is 1 */

#define CDC_MAX_PACKET_SIZE USB_OTG_FS_MAX_PACKET_SIZE

#ifndef USB_CDC_TRANSMIT_TIMEOUT
  #define USB_CDC_TRANSMIT_TIMEOUT 3
#endif

#ifndef USB_CDC_RECEIVE_TIMEOUT
  #define USB_CDC_RECEIVE_TIMEOUT 50
#endif

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
bool CDC_initialized = false;

/* Received Data over USB are stored in this buffer       */
CDC_TransmitQueue_TypeDef TransmitQueue;
CDC_ReceiveQueue_TypeDef ReceiveQueue;
__IO uint32_t lineState = 0;
__IO bool receivePended = true;
static uint32_t transmitStart = 0;

#ifdef DTR_TOGGLING_SEQ
/* DTR toggling sequence management */
extern void dtr_togglingHook(uint8_t *buf, uint32_t *len);
uint8_t dtr_toggling = 0;
#endif
USBD_CDC_LineCodingTypeDef linecoding = {
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
bool CDC_connected();
void CDC_continue_transmit(void);
bool CDC_resume_receive(void);
uint8_t USBD_CDC_ClearBuffer(USBD_HandleTypeDef *pdev)
{
  /* Suspend or Resume USB Out process */
  if (pdev->pClassData != NULL) {
    /* Prepare Out endpoint to receive next packet */
    USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, 0, 0);
    return USBD_OK;
  } else {
    return USBD_FAIL;
  }
}
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  USBD_CDC_Transferred,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  CDC_TransmitQueue_Init(&TransmitQueue);
  CDC_ReceiveQueue_Init(&ReceiveQueue);
  receivePended = true;
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, CDC_ReceiveQueue_ReserveBlock(&ReceiveQueue));

  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
      linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | \
					(pbuf[2] << 16) | (pbuf[3] << 24));
      linecoding.format     = pbuf[4];
      linecoding.paritytype = pbuf[5];
      linecoding.datatype   = pbuf[6];
    break;

    case CDC_GET_LINE_CODING:
      pbuf[0] = (uint8_t)(linecoding.bitrate);
      pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
      pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
      pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
      pbuf[4] = linecoding.format;
      pbuf[5] = linecoding.paritytype;
      pbuf[6] = linecoding.datatype;
    break;

    case CDC_SET_CONTROL_LINE_STATE:
      lineState =
              (((USBD_SetupReqTypedef *)pbuf)->wValue & 0x01) != 0; // Check DTR state
            if (lineState) { // Reset the transmit timeout when the port is connected
              transmitStart = 0;
            }
      #ifdef DTR_TOGGLING_SEQ
            dtr_toggling++; /* Count DTR toggling */
      #endif

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
#ifdef DTR_TOGGLING_SEQ
  if (dtr_toggling > 3) {
    dtr_togglingHook(Buf, Len);
    dtr_toggling = 0;
  }
#else
  UNUSED(Buf);
#endif
  /* It always contains required amount of free space for writing */
  CDC_ReceiveQueue_CommitBlock(&ReceiveQueue, (uint16_t)(*Len));
  receivePended = false;
  /* If enough space in the queue for a full buffer then continue receive */
  if (!CDC_resume_receive()) {
    USBD_CDC_ClearBuffer(&hUsbDeviceFS);
  }

//  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */

  transmitStart = 0;
  CDC_TransmitQueue_CommitRead(&TransmitQueue);
  CDC_continue_transmit();

  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
bool CDC_connected()
{
  /* Save the transmitStart value in a local variable to avoid twice reading - fix #478 */
  uint32_t transmitTime = transmitStart;
  if (transmitTime) {
    transmitTime = HAL_GetTick() - transmitTime;
  }
  return hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED
         && transmitTime < USB_CDC_TRANSMIT_TIMEOUT
         && lineState;
}

void CDC_continue_transmit(void)
{
  uint16_t size;
  uint8_t *buffer;
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *) hUsbDeviceFS.pClassData;
  /*
   * TS: This method can be called both in the main thread
   * (via USBSerial::write) and in the IRQ stream (via USBD_CDC_Transferred),
   * BUT the main thread cannot pass this condition while waiting for a IRQ!
   * This is not possible because TxState is not zero while waiting for data
   * transfer ending! The IRQ thread is uninterrupted, since its priority
   * is higher than that of the main thread. So this method is thread safe.
   */
  if (hcdc->TxState == 0U) {
    buffer = CDC_TransmitQueue_ReadBlock(&TransmitQueue, &size);
    if (size > 0) {
      transmitStart = HAL_GetTick();
      USBD_CDC_SetTxBuffer(&hUsbDeviceFS, buffer, size);
      /*
       * size never exceed PMA buffer and USBD_CDC_TransmitPacket make full
       * copy of block in PMA, so no need to worry about buffer damage
       */
      USBD_CDC_TransmitPacket(&hUsbDeviceFS);
    }
  }
}


bool CDC_resume_receive(void)
{
  /*
   * TS: main and IRQ threads can't pass it at same time, because
   * IRQ may occur only if receivePended is true. So it is thread-safe!
   */
  if (!receivePended) {
    uint8_t *block = CDC_ReceiveQueue_ReserveBlock(&ReceiveQueue);
    if (block != NULL) {
      receivePended = true;
      /* Set new buffer */
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, block);
      USBD_CDC_ReceivePacket(&hUsbDeviceFS);
      return true;
    }
  }
  return false;
}

int8_t CDC_init(void)
{
  return CDC_Init_FS();
}

int8_t CDC_DeInit(void)
{
  return CDC_DeInit_FS();
}

int8_t USBD_CDC_Transferred(void)
{
  transmitStart = 0;
  CDC_TransmitQueue_CommitRead(&TransmitQueue);
  CDC_continue_transmit();
  return (USBD_OK);
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
