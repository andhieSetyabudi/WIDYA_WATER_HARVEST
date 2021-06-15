#ifndef BASIC_LOGIC_H_
#define BASIC_LOGIC_H_

#include "main.h"
#include "stdio.h"
#include "stdbool.h"

#define PORT_LOW	GPIO_PIN_RESET
#define PORT_HIGH	GPIO_PIN_SET

typedef struct __attribute__ ((__packed__)) {
	  GPIO_TypeDef* port;
	  uint16_t pin;
}PORT_IO;

static void __inline__ digitalWrite(PORT_IO port_map, uint8_t logic)
{
	HAL_GPIO_WritePin(port_map.port, port_map.pin, logic);
}

static uint8_t __inline__ digitalRead(PORT_IO port_map)
{
	return (uint8_t) HAL_GPIO_ReadPin(port_map.port, port_map.pin);
}

static void __inline__ digitalToggle(PORT_IO port_map)
{
	HAL_GPIO_TogglePin(port_map.port, port_map.pin);
}

#endif /* BASIC_LOGIC_H_ */
