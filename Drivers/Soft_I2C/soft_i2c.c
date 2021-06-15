/*
 * soft_i2c.c
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#include "soft_i2c.h"

/*
 * initialize pin as fast_moded
 */
void softI2C_init(PORT_IO SCL, PORT_IO SDA)
{
	GPIO_InitTypeDef			GPIO_InitStructure;

	GPIO_InitStructure.Speed	=	GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode		=	GPIO_MODE_OUTPUT_PP;

	GPIO_InitStructure.Pin   	= SCL.pin;
	HAL_GPIO_Init(SCL.port, &GPIO_InitStructure);
	GPIO_InitStructure.Pin   	= SDA.pin;
	HAL_GPIO_Init(SDA.port, &GPIO_InitStructure);
	delay_microsInit();
}

/*
 * Set mode of pin
 * as output while transmitting data
 * as input while receiving data
 */

void pinMode(PORT_IO pin_, uint8_t MODE_)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if ( MODE_ == I2C_PIN_OUTPUT_MODE )
	{
		GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;
	}
	else
	{
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
	}
	GPIO_InitStructure.Pin   = pin_.pin;
	HAL_GPIO_Init(pin_.port, &GPIO_InitStructure);
}

/*
 * Set pin logic output
 * logic High or Low
 */
void pinOutput(PORT_IO pin_, uint8_t state)
{
	digitalWrite(pin_, state);
}

/*
 * Pulse on a pin
 */
void pinOnPulse(PORT_IO pin_)
{
	digitalWrite(pin_, PORT_HIGH);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    digitalWrite(pin_, PORT_LOW);
}

/*
 * get state logic of a pin
 */
uint8_t getPinState(PORT_IO pin_)
{
	return digitalRead(pin_);
}

/*
 * Initial Port of the SW-I2C
 */
void SW_I2C_initial_port(softI2C_pin *SW_port)
{
	pinOutput(SW_port->SDA_pin, PORT_HIGH);
	pinOutput(SW_port->SCL_pin, PORT_HIGH);
}

/*
 * send start bit on line
 */
void SW_I2C_start_condition(softI2C_pin *SW_port)
{
	SW_I2C_initial_port(SW_port);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    pinOutput(SW_port->SDA_pin, PORT_LOW);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    pinOutput(SW_port->SCL_pin, PORT_LOW);
    delay_microSeconds(SW_I2C_WAIT_TIME << 1);
}

/*
 * send stop bit on line
 */
void SW_I2C_stop_condition(softI2C_pin *SW_port)
{
	pinOutput(SW_port->SDA_pin, PORT_LOW);
	pinOutput(SW_port->SCL_pin, PORT_HIGH);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    pinOutput(SW_port->SDA_pin, PORT_HIGH);
    delay_microSeconds(SW_I2C_WAIT_TIME);
}

/*
 * Check for the ACK status
 */
SoftI2C_status SW_I2C_check_ack(softI2C_pin *SW_port)
{
	SoftI2C_status  ack;
    int             i;
    unsigned int    temp;
    pinMode(SW_port->SDA_pin, I2C_PIN_INPUT_MODE);
    pinOutput(SW_port->SCL_pin, PORT_HIGH);
    ack = SoftI2C_OK;
    delay_microSeconds(SW_I2C_WAIT_TIME);
    for (i = 10; i > 0; i--)
    {
        // read SDA pin state
        temp = !getPinState(SW_port->SDA_pin);
    	if (temp)
        {
            ack = SoftI2C_ERROR;
            break;
        }
    }
    pinOutput(SW_port->SCL_pin, PORT_LOW);
    pinMode(SW_port->SDA_pin, I2C_PIN_OUTPUT_MODE);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    return ack;
}

/*
 * send command for NACK
 */
void SW_I2C_check_not_ack(softI2C_pin *SW_port)
{
	pinMode(SW_port->SDA_pin, I2C_PIN_INPUT_MODE);
	pinOnPulse(SW_port->SCL_pin);
	pinMode(SW_port->SDA_pin, I2C_PIN_OUTPUT_MODE);
    delay_microSeconds(SW_I2C_WAIT_TIME);
}

/*
 * send cmd to NACK continue
 */
void SW_I2C_check_not_ack_continue(softI2C_pin *SW_port)
{
	pinOnPulse(SW_port->SCL_pin);
    delay_microSeconds(SW_I2C_WAIT_TIME);
}


void SW_I2C_slave_address(softI2C_pin *SW_port, uint8_t IICID, uint8_t readwrite)
{
    if (readwrite)
        IICID |= I2C_READ;
    else
        IICID &= ~I2C_READ;
    pinOutput(SW_port->SCL_pin, PORT_LOW);
    for (int x = 7; x >= 0; x--)
    {
    	pinOutput(SW_port->SDA_pin, IICID & (1 << x));
        delay_microSeconds(SW_I2C_WAIT_TIME);
        pinOnPulse(SW_port->SCL_pin);
    }
}

void SW_I2C_register_address(softI2C_pin *SW_port, uint8_t addr)
{
    pinOutput(SW_port->SCL_pin, PORT_LOW);
    for (int x = 7; x >= 0; x--)
    {
    	pinOutput(SW_port->SDA_pin, addr & (1 << x));
        delay_microSeconds(SW_I2C_WAIT_TIME);
        pinOnPulse(SW_port->SCL_pin);
    }
}

void SW_I2C_send_ack(softI2C_pin *SW_port)
{
	pinMode(SW_port->SDA_pin, I2C_PIN_OUTPUT_MODE);
    pinOutput(SW_port->SDA_pin, PORT_LOW);
    delay_microSeconds(SW_I2C_WAIT_TIME);
    pinOutput(SW_port->SCL_pin, PORT_HIGH);
    delay_microSeconds(SW_I2C_WAIT_TIME << 1);
    pinOutput(SW_port->SDA_pin, PORT_LOW);
    delay_microSeconds(SW_I2C_WAIT_TIME << 1);
    pinOutput(SW_port->SCL_pin, PORT_LOW);
    pinMode(SW_port->SDA_pin, I2C_PIN_OUTPUT_MODE);
    delay_microSeconds(SW_I2C_WAIT_TIME);
}



/*uint8_t SW_I2C_ReadVal_SDA(uint8_t sel)
{
    if(sel == 1)
        return GPIO_ReadInputDataBit(SW_I2C1_SDA_GPIO, SW_I2C1_SDA_PIN);
	return 0;
}

uint8_t SW_I2C_ReadVal_SCL(uint8_t sel)
{
    if(sel == 1)
        return GPIO_ReadInputDataBit(SW_I2C1_SCL_GPIO, SW_I2C1_SCL_PIN);

    return 0;
}*/

void SW_I2C_Write_Data(softI2C_pin *SW_port, uint8_t data)
{
	pinOutput(SW_port->SCL_pin, PORT_LOW);

    for (int x = 7; x >= 0; x--)
    {
    	pinOutput(SW_port->SDA_pin, data & (1 << x));
        delay_microSeconds(SW_I2C_WAIT_TIME);
        pinOnPulse(SW_port->SCL_pin);
    }
}

/*
 * Read data 8bit
 */
uint8_t SW_I2C_Read_Data(softI2C_pin *SW_port)
{
    uint8_t  readdata = 0;
    pinMode(SW_port->SDA_pin, I2C_PIN_INPUT_MODE);
    for (int x = 8; x--;)
    {
    	pinOutput(SW_port->SCL_pin, PORT_HIGH);
        readdata <<= 1;
        if( getPinState(SW_port->SDA_pin))
            readdata |= 0x01;
        delay_microSeconds(SW_I2C_WAIT_TIME);
        pinOutput(SW_port->SCL_pin, PORT_LOW);
        delay_microSeconds(SW_I2C_WAIT_TIME);
    }
    pinMode(SW_port->SDA_pin, I2C_PIN_OUTPUT_MODE);
    return readdata;
}

/*
 * Write Control Data 8bit
 */
SoftI2C_status SW_I2C_WriteControl_8Bit(softI2C_pin *SW_port, uint8_t IICID, uint8_t regaddr, uint8_t data, uint32_t timeout)
{
	SoftI2C_status   returnack = TRUE;
	uint32_t time_tick=0;
    SW_I2C_start_condition(SW_port);
    SW_I2C_start_condition(SW_port);
    SW_I2C_slave_address(SW_port, IICID, WRITE_CMD);
    if( SW_port->getTick != NULL )
    	time_tick = SW_port->getTick();
    else
    	time_tick = HAL_GetTick();
    if( SW_I2C_check_ack(SW_port) != SoftI2C_OK )
    	returnack = SoftI2C_ERROR;
    delay_microSeconds(SW_I2C_WAIT_TIME);
    SW_I2C_register_address(SW_port, regaddr);
//
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    SW_I2C_Write_Data(sel, data);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_stop_condition(sel);

    return returnack;
}

uint8_t SW_I2C_WriteControl_8Bit_OnlyRegAddr(uint8_t sel, uint8_t IICID, uint8_t regaddr)
{
    uint8_t   returnack = TRUE;
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    i2c_register_address(sel, regaddr);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//


    return returnack;
}

uint8_t SW_I2C_WriteControl_16Bit(uint8_t sel, uint8_t IICID, uint8_t regaddr, uint16_t data)
{
    uint8_t   returnack = TRUE;
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_register_address(sel, regaddr);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    SW_I2C_Write_Data(sel, (data >> 8) & 0xFF);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    SW_I2C_Write_Data(sel, data & 0xFF);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_stop_condition(sel);

    return returnack;
}

uint8_t SW_I2C_ReadControl_8Bit_OnlyRegAddr(uint8_t sel, uint8_t IICID, uint8_t regaddr)
{
    uint8_t   returnack = TRUE;

//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_register_address(sel, regaddr);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_stop_condition(sel);

    return returnack;
}

uint8_t SW_I2C_ReadControl_8Bit_OnlyData(uint8_t sel, uint8_t IICID)
{
    uint8_t  readdata = 0;

//    i2c_port_initial(sel);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    i2c_check_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    readdata = SW_I2C_Read_Data(sel);
//
//    i2c_check_not_ack(sel);
//
//    i2c_stop_condition(sel);

    return readdata;
}

uint16_t SW_I2C_ReadControl_16Bit_OnlyData(uint8_t sel, uint8_t IICID)
{
    uint8_t  readimsi = 0;
    uint16_t  readdata = 0;
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    i2c_check_not_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    readimsi = SW_I2C_Read_Data(sel);
//    i2c_check_not_ack_continue(sel);
//
//    readdata = readimsi<<8;
//
//    readimsi = SW_I2C_Read_Data(sel);
//    i2c_check_not_ack(sel);
//
//
//    readdata |= readimsi;
//
//    i2c_stop_condition(sel);

    return readdata;
}

uint8_t SW_I2C_ReadControl_8Bit(uint8_t sel, uint8_t IICID, uint8_t regaddr)
{
    uint8_t  readdata = 0;

//    i2c_port_initial(sel);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    i2c_check_ack(sel);
//
//    i2c_register_address(sel, regaddr);
//    i2c_check_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    i2c_check_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    readdata = SW_I2C_Read_Data(sel);
//
//    i2c_check_not_ack(sel);
//
//    i2c_stop_condition(sel);

    return readdata;
}

uint16_t SW_I2C_ReadControl_16Bit(uint8_t sel, uint8_t IICID, uint8_t regaddr)
{
    uint16_t  readdata = 0;
//
//    i2c_port_initial(sel);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    i2c_check_ack(sel);
//
//    i2c_register_address(sel, regaddr);
//    i2c_check_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    i2c_check_ack(sel);
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    readdata = SW_I2C_Read_Data(sel);
//    i2c_send_ack(sel);
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    readdata = ((readdata << 8) | SW_I2C_Read_Data(sel));
//
//    i2c_check_not_ack(sel);
//
//    i2c_stop_condition(sel);

    return readdata;
}

uint8_t SW_I2C_ReadnControl_8Bit(uint8_t sel, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
    uint8_t   returnack = TRUE;
    uint8_t  index;

//    i2c_port_initial(sel);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_register_address(sel, regaddr);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    for ( index = 0 ; index < rcnt ; index++){
//    	TIMER__Wait_us(SW_I2C_WAIT_TIME);
//    	pdata[index] = SW_I2C_Read_Data(sel);
//    }
//
//    pdata[rcnt-1] = SW_I2C_Read_Data(sel);
//
//    i2c_check_not_ack(sel);
//
//    i2c_stop_condition(sel);

    return returnack;
}

uint8_t SW_I2C_Multi_ReadnControl_8Bit(uint8_t sel, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
    uint8_t   returnack = TRUE;
    uint8_t  index;

//    i2c_port_initial(sel);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_register_address(sel, regaddr);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    TIMER__Wait_us(SW_I2C_WAIT_TIME);
//
//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, READ_CMD);
//    if (!i2c_check_ack(sel)) { returnack = FALSE; }
//
//    for ( index = 0 ; index < (rcnt-1) ; index++){
//    	TIMER__Wait_us(SW_I2C_WAIT_TIME);
//    	pdata[index] = SW_I2C_Read_Data(sel);
//	i2c_send_ack(sel);
//
//    }
//
//    pdata[rcnt-1] = SW_I2C_Read_Data(sel);
//
//    i2c_check_not_ack(sel);
//
//    i2c_stop_condition(sel);

    return returnack;
}

uint8_t SW_I2C_Check_SlaveAddr(uint8_t sel, uint8_t IICID)
{
    uint8_t   returnack = TRUE;

//    i2c_start_condition(sel);
//
//    i2c_slave_address(sel, IICID, WRITE_CMD);
//    if (!i2c_check_ack(sel))
//    {
//        returnack = FALSE;
//    }

    return returnack;
}

uint8_t SW_I2C_UTIL_WRITE(uint8_t sel, uint8_t IICID, uint8_t regaddr, uint8_t data)
{
	return TRUE;
	//return SW_I2C_WriteControl_8Bit(sel, IICID<<1, regaddr, data);
}

uint8_t SW_I2C_UTIL_Read(uint8_t sel, uint8_t IICID, uint8_t regaddr)
{
	return TRUE;
//	return SW_I2C_ReadControl_8Bit(sel, IICID<<1, regaddr);
}

uint8_t SW_I2C_UTIL_Read_Multi(uint8_t sel, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
	return TRUE;
//	return SW_I2C_Multi_ReadnControl_8Bit(sel, IICID<<1, regaddr, rcnt, pdata);
}

