/*
 * SGP40.c
 *
 *  Created on: Jun 23, 2021
 *      Author: Andhie Setyabudi
 *
 *      for Widya-imersif
 */


#include "SGP40.h"


static void SGP40_delay(uint32_t time_);

static uint8_t checkCrc(uint8_t data1,uint8_t data2)
{
  uint8_t crc = 0xFF;
  uint8_t data[2];
  data[0]=data1;
  data[1]=data2;
  for(int i =0; i<2;i++){
    crc ^= data[i];
    for(uint8_t bit = 8;bit>0;--bit){
      if(crc & 0x80){
        crc = (crc << 1)^0x31u;
      }else{
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

SGP40_status SGP40_write(SGP40_var *var, uint8_t* pData, uint32_t len, uint32_t timeout_)
{
	return (SGP40_status) HAL_I2C_Master_Transmit(var->i2c_ch, var->_deviceAddr, pData, len, timeout_);
}

SGP40_status SGP40_read(SGP40_var *var, uint8_t* pData, uint32_t len, uint32_t timeout_)
{
	return (SGP40_status) HAL_I2C_Master_Receive(var->i2c_ch, var->_deviceAddr, pData, len, timeout_);
}

SGP40_status SGP40_readRAW(SGP40_var *var, uint16_t* raw, uint32_t timeout_)
{
	uint8_t data[3]={0,0,0};
	uint16_t value=0;
	*raw = 0;
	if ( SGP40_read(var, data, 3, timeout_) == SGP40_OK )
	{
		value=(data[0]<<8)|data[1];
		*raw = value;
		return SGP40_OK;
	}
	return SGP40_ERROR;
}


SGP40_status SGP40_isDevice(SGP40_var *var)
{
	if ( var->i2c_ch == NULL )
		return SGP40_ERROR;
	else
		return (SGP40_status)HAL_I2C_IsDeviceReady(var->i2c_ch, var->_deviceAddr, 10, 500);
}

SGP40_status SGP40_begin(SGP40_var *var, I2C_HandleTypeDef *i2c_ch, uint8_t addr, uint32_t duration)
{
	var->i2c_ch 		= i2c_ch;
	var->_deviceAddr 	= addr << 1;
	var->_relativeHumidity	=	50.f;
	var->_temperatureC		= 	25.f;
	SGP40_status ret = SGP40_isDevice(var);
	if ( ret != SGP40_OK )
		return ret;
	VocAlgorithm_init(&var->_vocaAgorithmParams);
	uint32_t t_ = HAL_GetTick();
	while( HAL_GetTick() - t_ <= duration )
	{
		uint32_t rol_ov = HAL_GetTick();
		if ( t_ > rol_ov )	// perventive action for roll-over counting in system-ticks
			t_ = rol_ov;
		// get voc - index
		uint16_t voc_index;
		SGP40_getVOCIndex(var, NULL, 500UL);
	}
	// need to measurement test
	return SGP40_measurementTest(var);
//	return SGP40_OK;
}

SGP40_status SGP40_setRhT(SGP40_var *var, float relativeHumidity, float temperatureC, uint32_t timeout_)
{
	var->_relativeHumidity = relativeHumidity;
	var->_temperatureC = temperatureC;
	SGP40_dataTransform(var);
	return SGP40_write(var, var->_rhTemData,6, timeout_);
}

SGP40_status SGP40_heaterOff(SGP40_var *var, uint32_t timeout_)
{
	uint8_t testCommand[CMD_HEATER_OFF_SIZE]={CMD_HEATER_OFF_H,CMD_HEATER_OFF_L};
	return SGP40_write(var, testCommand,CMD_HEATER_OFF_SIZE, timeout_);
}

SGP40_status SGP40_softReset(SGP40_var *var)
{
	uint8_t testCommand[CMD_SOFT_RESET_SIZE]={CMD_SOFT_RESET_H,CMD_SOFT_RESET_L};
	SGP40_status ret = SGP40_OK;
	ret = SGP40_write(var, testCommand, CMD_SOFT_RESET_SIZE, 100);
	if ( ret == SGP40_OK )
	{
		var->_relativeHumidity = 50;
		var->_temperatureC = 25.f;
	}
	return ret;
}

SGP40_status SGP40_getVOCIndex(SGP40_var *var, uint16_t *VOC_index, uint32_t timeout_)
{
	*VOC_index = 0;
	uint8_t data[3]={0,0,0};
	int32_t value;
	int32_t vocIndex=0;
	SGP40_dataTransform(var);
	SGP40_status ret = SGP40_OK;
	ret = SGP40_write(var, var->_rhTemData,8, timeout_);
	if ( ret != SGP40_OK ) return ret;
	SGP40_delay(DURATION_READ_RAW_VOC);
	ret = SGP40_read(var, data, 3, timeout_);
	if ( ret != SGP40_OK ) return ret;
	value=(data[0]<<8)|data[1];
	VocAlgorithm_process(&var->_vocaAgorithmParams, value, &vocIndex);
	*VOC_index = vocIndex;
	return ret;
}

SGP40_status SGP40_measurementTest(SGP40_var *var)
{
	SGP40_status ret = SGP40_OK;
	uint8_t testCommand[CMD_MEASURE_TEST_SIZE]={CMD_MEASURE_TEST_H,CMD_MEASURE_TEST_L};
	uint16_t value=0;
	ret = SGP40_write(var, testCommand, CMD_MEASURE_TEST_SIZE, 200UL);
	if ( ret != SGP40_OK )
		return ret;
	SGP40_delay(DURATION_WAIT_MEASURE_TEST);
	ret = SGP40_readRAW(var, &value, 500UL);
	if ( ret == SGP40_OK )
	{
		if ( value == TEST_OK )
			return SGP40_OK;
		else
			return SGP40_ERROR;
	}
	else
		return ret;
}

void SGP40_dataTransform(SGP40_var *var)
{
	uint16_t RH = (uint16_t)((var->_relativeHumidity*65535)/100+0.5);
	uint16_t TemC = (uint16_t)((var->_temperatureC+45)*(65535/175)+0.5);
	var->_rhTemData[INDEX_MEASURE_RAW_H]=CMD_MEASURE_RAW_H;
	var->_rhTemData[INDEX_MEASURE_RAW_L]=CMD_MEASURE_RAW_L;
	var->_rhTemData[INDEX_RH_H]=RH>>8;
	var->_rhTemData[INDEX_RH_L]=RH&0x00FF;
	var->_rhTemData[INDEX_RH_CHECK_CRC]=checkCrc(var->_rhTemData[INDEX_RH_H],var->_rhTemData[INDEX_RH_L]);
	var->_rhTemData[INDEX_TEM_H]=TemC>>8;
	var->_rhTemData[INDEX_TEM_L]=TemC&0x00FF;
	var->_rhTemData[INDEX_TEM_CHECK_CRC]=checkCrc(var->_rhTemData[INDEX_TEM_H],var->_rhTemData[INDEX_TEM_L]);
}


SGP40_funct SGP40 =
{
		.halt 			= NULL,
		.begin			= SGP40_begin,
		.isDevice		= SGP40_isDevice,
		.setRhT			= SGP40_setRhT,
		.heaterOff		= SGP40_heaterOff,
		.softReset		= SGP40_softReset,
		.getVOCIndex	= SGP40_getVOCIndex,
};


static void SGP40_delay(uint32_t time_)
{
	if ( SGP40.halt == NULL )
		HAL_Delay(time_);
	else
		SGP40.halt(time_);
}
