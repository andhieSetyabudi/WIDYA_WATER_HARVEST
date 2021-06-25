/*
 * SGP40.h
 *
 *  Created on: Jun 23, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef VOC_SENSOR_SGP40_H_
#define VOC_SENSOR_SGP40_H_

#include "main.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "stddef.h"

#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

// special lib for VOC

#include "sensirion_arch_config.h"
#include "sensirion_voc_algorithm.h"

#define DFRobot_SGP40_ICC_ADDR                              0x59

#define TEST_OK                                           0xD400

#define CMD_HEATER_OFF_H                                  0x36
#define CMD_HEATER_OFF_L                                  0x15
#define CMD_HEATER_OFF_SIZE                               2

#define CMD_MEASURE_TEST_H                                0x28
#define CMD_MEASURE_TEST_L                                0x0E
#define CMD_MEASURE_TEST_SIZE                             2

#define CMD_SOFT_RESET_H                                  0x00
#define CMD_SOFT_RESET_L                                  0x06
#define CMD_SOFT_RESET_SIZE                               2

#define CMD_MEASURE_RAW_H                                 0x26
#define CMD_MEASURE_RAW_L                                 0x0F

#define INDEX_MEASURE_RAW_H                               0
#define INDEX_MEASURE_RAW_L                               1
#define INDEX_RH_H                                        2
#define INDEX_RH_L                                        3
#define INDEX_RH_CHECK_CRC                                4
#define INDEX_TEM_H                                       5
#define INDEX_TEM_L                                       6
#define INDEX_TEM_CHECK_CRC                               7

#define DURATION_READ_RAW_VOC                             30
#define DURATION_WAIT_MEASURE_TEST                        250


/* Status Reply */
typedef enum
{
	SGP40_OK       = 0x00U,
	SGP40_ERROR    = 0x01U,
	SGP40_BUSY     = 0x02U,
	SGP40_TIMEOUT  = 0x03U
} SGP40_status;

typedef struct{
	I2C_HandleTypeDef *i2c_ch;
	uint8_t addr;
	float _relativeHumidity;
	float _temperatureC;
	uint8_t _rhTemData[8];
	uint8_t _deviceAddr;
	VocAlgorithmParams _vocaAgorithmParams;
}SGP40_var;

static SGP40_status SGP40_begin(SGP40_var *var, I2C_HandleTypeDef *i2c_ch, uint8_t addr, uint32_t duration);

static SGP40_status SGP40_isDevice(SGP40_var *var);

static SGP40_status SGP40_setRhT(SGP40_var *var, float relativeHumidity, float temperatureC, uint32_t timeout_);

static SGP40_status SGP40_heaterOff(SGP40_var *var, uint32_t timeout_);

static SGP40_status SGP40_softReset(SGP40_var *var);

static SGP40_status SGP40_getVOCIndex(SGP40_var *var, uint16_t *VOC_index, uint32_t timeout_);

static SGP40_status SGP40_measurementTest(SGP40_var *var);

static void SGP40_dataTransform(SGP40_var *var);

typedef struct{
				void (*halt)				(uint32_t time_halt);
				SGP40_status (*begin)		(SGP40_var *var, I2C_HandleTypeDef *i2c_ch, uint8_t addr, uint32_t duration);
				SGP40_status (*isDevice)	(SGP40_var *var );
				SGP40_status (*setRhT)		(SGP40_var *var, float relativeHumidity, float temperatureC, uint32_t timeout_);
				SGP40_status (*heaterOff)	(SGP40_var *var, uint32_t timeout_);
				SGP40_status (*softReset)	(SGP40_var *var);
				SGP40_status (*getVOCIndex)	(SGP40_var *var, uint16_t *VOC_index, uint32_t timeout_);
}SGP40_funct;
extern SGP40_funct SGP40;





#endif /* VOC_SENSOR_SGP40_H_ */
