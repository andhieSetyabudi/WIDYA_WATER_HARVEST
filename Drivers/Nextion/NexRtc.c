/*
 * NexRtc.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexRtc.h"

NexRtc_func NexRtc ={
		.write_byString		= NexRtc_write_rtc_time_byString,
		.write_byType		= NexRtc_write_rtc_time_byType,
		.write_byArray		= NexRtc_write_rtc_time_byArray,

		.read_timestamp		= NexRtc_read_rtc_time,
		.read_byType		= NexRtc_read_rtc_time_byType,
		.read_byArray		= NexRtc_read_rtc_time_byArray,
};
