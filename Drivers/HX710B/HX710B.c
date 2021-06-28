/*
 * HX710B.h
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */


#include "HX710B.h"
#include "../BASIC_IO/micro_delay.h"


uint8_t shiftInSlow(HX710B_var *var) {
    uint8_t value = 0;
    uint8_t i;

    for(i = 0; i < 8; ++i) {
    	digitalWrite(var->SCI, PORT_HIGH);
    	HX710B_hlt;
            value |= digitalRead(var->DO) << (7 - i);
        digitalWrite(var->SCI, PORT_LOW);
      HX710B_hlt;
    }
    return value;
}

/*
 *  check the state of SDO pin,
 *  if pin state is low, mean data is ready
 */
HX710B_status HX710BIsReady(HX710B_var *var, uint32_t timeout_)
{
	uint32_t time_s = HAL_GetTick();
	while( digitalRead(var->DO) != PORT_LOW )
	{
		// preventive if counter is roll-over and start from beginning
		uint32_t rollOver = HAL_GetTick();
		if ( rollOver < time_s )
		{
			time_s = (uint32_t) abs((int)rollOver - (int)time_s);
		}
		if ( time_s - HAL_GetTick() >= timeout_ )
			return HX710B_TIMEOUT;
		// need delay;
		if ( HX710B.halt != NULL)
			HX710B.halt(10);
		else
			HAL_Delay(5);
	}
	return HX710B_OK;
}

/*
 *  Read the data conversion
 *  data range : 0x800000 to 0x7FFFFF
 */
HX710B_status HX710BRead(HX710B_var *var, int * data, uint32_t timeout_)
{
	// exit sleep mode
	digitalWrite(var->SCI, PORT_LOW);
	if ( HX710BIsReady ( var, timeout_) == HX710B_OK )
	{
		uint8_t i = 0;
		uint32_t raw = 0;
		for ( i = 0; i < var->mode; i++ )
		{
			HX710B_hlt; HX710B_hlt; HX710B_hlt;
			digitalWrite(var->SCI, PORT_HIGH);
			HX710B_hlt; HX710B_hlt; HX710B_hlt;
			digitalWrite(var->SCI, PORT_LOW);
			if ( i < 24)
			{
				raw = raw << 1;
				if ( digitalRead(var->DO) == PORT_HIGH )
						raw= raw | 0x01;
			}
		}
		// force the HX710B to enter the sleep mode
		digitalWrite(var->SCI, PORT_HIGH);
//		if ( raw > 0x3ffffe )
//			raw -= 0x7fffff;
//		*data = raw;
		*data = raw ^ 0x800000 ;
		return HX710B_OK;
	}
	return HX710B_TIMEOUT;
}


HX710B_status HX710BSetMode(HX710B_var *var, uint8_t mode, uint8_t timeout_)
{
	return HX710BRead(var, NULL, timeout_);
}


HX710B_status HX710BInit(HX710B_var* var, PORT_IO SCI, PORT_IO SDO, uint8_t mode_)
{
	var->DO  = SDO;
	var->SCI = SCI;
	var->mode= mode_;
	return HX710BSetMode(var, mode_);
}

HX710B_funct HX710B={
		.halt		= NULL,
		.init	 	= HX710BInit,
		.isReady	= HX710BIsReady,
		.read		= HX710BRead,
		.setMode	= HX710BSetMode,
};
