/*
 * NexPage.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */


#include "NexPage.h"


bool NexPage_show(NexObject_var * var)
{
    const char *name = var->__name;
	if (!name)
	{
		return false;
	}
	char cmd[strlen(var->__name)+8];
	sprintf(cmd, "page %s", var->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}


NexPage_func NexPage=
{
	.create = NexButton_create,
	.show 	= NexPage_show,
};
