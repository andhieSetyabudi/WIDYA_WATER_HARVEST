/*
 * NexRtc.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */
#pragma once
#ifndef NEXTION_NEXRTC_H_
#define NEXTION_NEXRTC_H_

#include "NexHardware.h"
#include "NexObject.h"
#include "stdbool.h"
#include "string.h"

/**
 * time format 0000/00/00 00:00:00 0
 * YYYY/MM/DD HH:MM:SS
 *
 */
static __inline__ bool NexRtc_write_rtc_time_byString(char *time)
{
	char year[5],mon[3],day[3],hour[3],min[3],sec[3];
	char cmd[12];
	sprintf(cmd, " ");

	if(strlen(time) >= 19)
	{
		year[0]=time[0]; 	year[1]=time[1];	year[2]=time[2];year[3]=time[3];year[4]='\0';
		mon[0]=time[5];		mon[1]=time[6];		mon[2]='\0';
		day[0]=time[8];		day[1]=time[9];		day[2]='\0';
		hour[0]=time[11];	hour[1]=time[12];	hour[2]='\0';
		min[0]=time[14];	min[1]=time[15];	min[2]='\0';
		sec[0]=time[17];	sec[1]=time[18];	sec[2]='\0';

		sprintf(cmd, "rtc0=%s", year);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

		sprintf(cmd," ");
		sprintf(cmd,"rtc1=%s", mon);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

		sprintf(cmd," ");
		sprintf(cmd,"rtc2=%s", day);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

		sprintf(cmd," ");
		sprintf(cmd,"rtc3=%s", hour);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

		sprintf(cmd," ");
		sprintf(cmd,"rtc4=%s", min);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

		sprintf(cmd," ");
		sprintf(cmd,"rtc5=%s", sec);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;
	}
	return false;
}

/**
 * write rtc times
 *
 * @param time_type - To type in time   (example:write_rtc_time("year",2016))
 * @param number - the time value
 * @return true if success, false for failure
 */

static __inline__ bool NexRtc_write_rtc_time_byType(char *time_type,uint32_t number)
{
	char cmd[18];
	char buf[10] = {0};
	utoa(number, buf, 10);
	if(strstr(time_type,"year"))
		sprintf(cmd, "rtc0=%s", buf);
	if(strstr(time_type,"mon"))
		sprintf(cmd, "rtc1=%s", buf);
	if(strstr(time_type,"day"))
		sprintf(cmd, "rtc2=%s", buf);
	if(strstr(time_type,"hour"))
		sprintf(cmd, "rtc3=%s", buf);
	if(strstr(time_type,"min"))
		sprintf(cmd, "rtc4=%s", buf);
	if(strstr(time_type,"sec"))
		sprintf(cmd, "rtc5=%s", buf);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

/**
 * write rtc times
 *
 * @param time - Time to write to the array
 * @return true if success, false for failure
 */

static __inline__ bool NexRtc_write_rtc_time_byArray(uint32_t *time)
{

	if ( &time[0] == NULL )
		return false;
	char year[5], cmd[12];
	utoa(time[0],year,10);
	sprintf(cmd,"rtc0=%s", year);
	NexHardware.sendCommand(cmd);
	if ( ! NexHardware.recvRetCommandFinished(100UL) )
		return false;

	if ( &time[1] != NULL )
	{
		char mon[3];
		utoa(time[1],mon, 10);
		sprintf(cmd," ");
		sprintf(cmd,"rtc1=%s", mon);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;
	}

	if ( &time[2] != NULL )
	{
		char day[3];
		utoa(time[2],day, 10);
		sprintf(cmd," ");
		sprintf(cmd,"rtc2=%s", day);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;
	}

	if ( &time[3] != NULL )
	{
		char hour[3];
		utoa(time[3],hour,10);
		sprintf(cmd," ");
		sprintf(cmd,"rtc3=%s", hour);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;
	}

	if ( &time[4] != NULL )
	{
		char min[3];
		utoa(time[4],min, 10);
		sprintf(cmd," ");
		sprintf(cmd,"rtc4=%s", min);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;
	}

	if ( &time[5] != NULL )
	{
		char sec[3];
		utoa(time[5],sec, 10);
		sprintf(cmd," ");
		sprintf(cmd,"rtc5=%s", sec);
		NexHardware.sendCommand(cmd);
		if ( ! NexHardware.recvRetCommandFinished(100UL) )
			return false;

	}
	return true;
}


/**
 * read rtc time
 *
 * @param time - Access data array
 * @param len - len of array
 * @return true if success, false for failure
 */

static __inline__ bool NexRtc_read_rtc_time(char *time,uint32_t len)
{
	char time_buf[22] = {"0000/00/00 00:00:00 0"};
	uint32_t year,mon,day,hour,min,sec,week;
	char cmd[10];

	// take years
	sprintf(cmd, "get rtc0");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&year, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take months
	sprintf(cmd, "get rtc1");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&mon, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take days
	sprintf(cmd, "get rtc2");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&day, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take hours
	sprintf(cmd, "get rtc3");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&hour, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take minutes
	sprintf(cmd, "get rtc4");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&min, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take seconds
	sprintf(cmd, "get rtc5");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&sec, 100UL) )
		return false;
	memset(cmd,'\0', 10);

	// take week-day
	sprintf(cmd, "get rtc6");
	NexHardware.sendCommand(cmd);
	if ( !NexHardware.recvRetNumber(&week, 100UL) )
		return false;

	time_buf[0] = year/1000 + '0';
	time_buf[1] = (year/100)%10 + '0';
	time_buf[2] = (year/10)%10 + '0';
	time_buf[3] = year%10 + '0';
	time_buf[5] = mon/10 + '0';
	time_buf[6] = mon%10 + '0';
	time_buf[8] = day/10 + '0';
	time_buf[9] = day%10 + '0';
	time_buf[11] = hour/10 + '0';
	time_buf[12] = hour%10 + '0';
	time_buf[14] = min/10 + '0';
	time_buf[15] = min%10 + '0';
	time_buf[17] = sec/10 + '0';
	time_buf[18] = sec%10 + '0';
	time_buf[20] = week + '0';
	time_buf[21] = '\0';

	if(len >= 22)
	{
		for(int i=0;i<22;i++)
		{
			time[i] = time_buf[i];
		}
	}
	else{
		for(int i=0;i<len;i++)
		{
			time[i] = time_buf[i];
		}
	}
	return true;
}

/**
 * read rtc times
 *
 * @param time_type - To type in time
 * @param number - the time value
 * @return true if success, false for failure
 */

static __inline__ bool NexRtc_read_rtc_time_byType(char *time_type,uint32_t *number)
{
	char cmd[10];
	if(strstr(time_type,"year"))
		sprintf(cmd, "get rtc0");
	else if(strstr(time_type,"mon"))
		sprintf(cmd, "get rtc1");
	else if(strstr(time_type,"day"))
		sprintf(cmd, "get rtc2");
	else if(strstr(time_type,"hour"))
		sprintf(cmd, "get rtc3");
	else if(strstr(time_type,"min"))
		sprintf(cmd, "get rtc4");
	else if(strstr(time_type,"sec"))
		sprintf(cmd, "get rtc5");
	else if(strstr(time_type,"week"))
		sprintf(cmd, "get rtc6");
	else{
		return false;
	}
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * read rtc time
 *
 * @param time - Access data array
 * @param len - len of array
 * @return true if success, false for failure
 */

static __inline__ bool NexRtc_read_rtc_time_byArray(uint32_t *time,uint32_t len)
{
	if ( len > 0 )
	{
		if ( len > 7 ) len = 7;
		for ( uint8_t i = 0; i < 7; i++ )
		{
			char cmd[10];
			sprintf(cmd, "get rtc%d", i);
			NexHardware.sendCommand(cmd);
			if ( !NexHardware.recvRetNumber(&time[i], 100UL) )
			{
				return false;
			}
		}
		return true;
	}
	else
		return false;
}


typedef struct {
	bool (*write_byString)	(char *time);
	bool (*write_byType)	(char *time_type,uint32_t number);
	bool (*write_byArray)	(uint32_t *time);
	bool (*read_timestamp)  (char *time,uint32_t len);
	bool (*read_byType)		(char *time_type,uint32_t *number);
	bool (*read_byArray)	(uint32_t *time,uint32_t len);
}NexRtc_func;

NexRtc_func NexRtc;

extern NexRtc_func NexRtc;

#endif /* NEXTION_NEXRTC_H_ */
