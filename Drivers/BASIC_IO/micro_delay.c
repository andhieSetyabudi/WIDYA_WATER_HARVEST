/*
 * micro_delay.c
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#include "micro_delay.h"

DWTdelay_status  delay_microsInit(void)
{
	 if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
	        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	        DWT->CYCCNT = 0;
	        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	    }
	 __NOP();
	 __NOP();
	 __NOP();
	 if(DWT->CYCCNT)
	 {
		return DWTDelay_OK; /*clock cycle counter started*/
	 }
	 else
	 {
	   return DWTDelay_ERROR; /*clock cycle counter not started*/
	 }
};

#if DWT_DELAY_EX
void delay_microSeconds(uint32_t us)
{
	uint32_t startTick  = DWT->CYCCNT,
//			 targetTick = DWT->CYCCNT + us * (SystemCoreClock/1000000);
			targetTick = DWT->CYCCNT + us * (HAL_RCC_GetHCLKFreq()/1000000);

	// Must check if target tick is out of bounds and overflowed
	if (targetTick > startTick) {
		// Not overflowed
		while (DWT->CYCCNT < targetTick);
	} else {
		// Overflowed
		while (DWT->CYCCNT > startTick || DWT->CYCCNT < targetTick);
	}
}
#else
void delay_microSeconds(uint32_t us)
{
	uint32_t startTick = DWT->CYCCNT,
			 delayTicks = us * (HAL_RCC_GetHCLKFreq()/1000000);

	while (DWT->CYCCNT - startTick < delayTicks);
}
#endif
