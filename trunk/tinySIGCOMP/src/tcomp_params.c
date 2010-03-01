/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_params.c
 * @brief  SigComp parameters as per rfc 3320 subclause 3.3.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_params.h"
#include "tsk_binaryutils.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tcomp_params_group SigComp parameters.
*/

/**@ingroup tcomp_params_group
* Checks if CPB, DMS and SMS values have been initialized.
* @param params The sigcomp parameters containing the values to check.
* @retval 1 if values have been set and zero otherwise.
*/
int tcomp_params_hasCpbDmsSms(tcomp_params_t* params)
{
	if(params)
	{
		return (params->cpbCode || params->dmsCode || params->smsCode) ? 1 : 0;
	}
	else TSK_DEBUG_WARN("NULL sigcomp parameters.");
	
	return 0;
}

/**@ingroup tcomp_params_group
* Sets CPB bits.
* @param params The sigcomp parameters containing cpb bits to set.
* @param cpbCode The new CPB code.
* @sa @ref tcomp_params_setCpbValue.
*/
void tcomp_params_setCpbCode(tcomp_params_t* params, uint8_t cpbCode)
{
	if(params)
	{
		params->cpbCode = cpbCode; 
		params->cpbValue = sigcomp_encoding_cpb[cpbCode]; 
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Sets CPB bits.
* @param params The sigcomp parameters containing cpb bits to set.
* @param cpbValue The new CPB value.
* @sa @ref tcomp_params_setCpbCode.
*/
void tcomp_params_setCpbValue(tcomp_params_t* params, uint8_t cpbValue)
{
	if(params)
	{
		uint8_t code;
		for(code=0; code<4; code++)
		{
			if( cpbValue <= sigcomp_encoding_cpb[code])
			{
				params->cpbCode = code;
				break;
			}
		}
		params->cpbValue = cpbValue;
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Sets DMS bits.
* @param params The sigcomp parameters containing dms bits to set.
* @param dmsCode The new DMS code.
* @sa @ref tcomp_params_setDmsValue.
*/
void tcomp_params_setDmsCode(tcomp_params_t* params, uint8_t dmsCode)
{
	if(params)
	{
		params->dmsCode = dmsCode;
		params->dmsValue = sigcomp_encoding_dms[dmsCode];
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Sets DMS bits.
* @param params The sigcomp parameters containing dms bits to set.
* @param dmsValue The new DMS value.
* @sa @ref tcomp_params_setDmsCode.
*/
void tcomp_params_setDmsValue(tcomp_params_t* params, uint32_t dmsValue)
{
	if(params)
	{
		uint8_t code;
		for(code=1; code<8; code++)
		{
			if(dmsValue <= sigcomp_encoding_dms[code])
			{
				params->dmsCode = code;
				break;
			}
		}
		params->dmsValue = dmsValue;
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Sets SMS bits.
* @param params The sigcomp parameters containing sms bits to set.
* @param smsCode The new SMS code.
* @sa @ref tcomp_params_setSmsValue.
*/
void tcomp_params_setSmsCode(tcomp_params_t* params, uint8_t smsCode)
{
	if(params)
	{
		params->smsCode = smsCode; 
		params->smsValue = sigcomp_encoding_sms[smsCode];
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Sets SMS bits.
* @param params The sigcomp parameters containing sms bits to set.
* @param smsValue The new SMS value.
* @sa @ref tcomp_params_setSmsCode.
*/
void tcomp_params_setSmsValue(tcomp_params_t* params, uint32_t smsValue)
{
	if(params)
	{
		uint8_t code;
		for(code = 0; code < 8; code++)
		{
			if(smsValue <= sigcomp_encoding_sms[code])
			{
				params->smsCode = code;
				break;
			}
		}
		params->smsValue = smsValue;
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Gets CPB, DMS and SMS values as a single 2-bytes value.
* @param params The sigcomp parameters containing the values.
* @retval CPB||DMS||SMS as 2-bytes value.
* @sa @ref tcomp_params_setParameters.
*/
uint16_t tcomp_params_getParameters(tcomp_params_t* params)
{
	if(params)
	{
		/*
		+---+---+---+---+---+---+---+---+
		|  cpb  |    dms    |    sms    |
		+---+---+---+---+---+---+---+---+
		|        SigComp_version        |
		+---+---+---+---+---+---+---+---+
		*/
		uint16_t result = ((params->cpbCode<<6)|(params->dmsCode<<3)|params->smsCode);
		result <<=8;
		return (result | params->SigComp_version);
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");

	return 0;
}

/**@ingroup tcomp_params_group
* Sets CPB, DMS and SMS values.
* @param params The sigcomp parameters containing the values to set.
* @param sigCompParameters New values as 2-bytes value.
* @sa @ref tcomp_params_getParameters.
*/
void tcomp_params_setParameters(tcomp_params_t* params, uint16_t sigCompParameters)
{
	if(params)
	{
		/*
		+---+---+---+---+---+---+---+---+
		|  cpb  |    dms    |    sms    |
		+---+---+---+---+---+---+---+---+
		|        SigComp_version        |
		+---+---+---+---+---+---+---+---+
		*/
		tcomp_params_setCpbCode( params, (sigCompParameters>>14) );
		tcomp_params_setDmsCode( params, ((sigCompParameters>>11) & 0x07) );
		tcomp_params_setSmsCode( params, ((sigCompParameters>>8) & 0x07) );
		params->SigComp_version = ( (sigCompParameters & 0x00ff) );
	}
	else TSK_DEBUG_ERROR("NULL sigcomp parameters.");
}

/**@ingroup tcomp_params_group
* Reset all parameters.
* @param params The params to reset.
*/
void tcomp_params_reset(tcomp_params_t* params)
{
	if(params)
	{
		params->cpbCode = params->dmsCode = params->smsCode = params->SigComp_version = 0;
		params->cpbValue = params->dmsValue = params->smsValue = 0;
		
		tsk_list_clear_items(params->returnedStates);
	}
	else TSK_DEBUG_WARN("NULL sigcomp parameters.");
}








//========================================================
//	SigComp parameters object definition
//

/**@ingroup tcomp_params_group
* Creates new sigcomp params. You MUST use @ref tcomp_params_destroy to free the params.
* @retval New sigcomp params.
* @sa @ref tcomp_params_destroy.
*/
static void* tcomp_params_create(void *self, va_list * app)
{
	tcomp_params_t *params = self;
	if(params)
	{
		//tcomp_params_reset(params);
		params->returnedStates = TSK_LIST_CREATE();
	}
	else TSK_DEBUG_ERROR("Failed to create new sigcomp params.");
	
	return self;
}

/**@ingroup tcomp_nackinfo_group
* Destroy SigComp parameters previously created using @ref tcomp_params_create.
* @param params The SigComp params to free.
* @sa @ref tcomp_params_create.
*/
static void* tcomp_params_destroy(void *self)
{
	tcomp_params_t *params = self;
	if(params)
	{
		TSK_OBJECT_SAFE_FREE(params->returnedStates);
	}
	else TSK_DEBUG_WARN("NULL sigcomp parameters.");

	return self;
}

static const tsk_object_def_t tcomp_params_def_s = 
{
	sizeof(tcomp_params_t),
	tcomp_params_create, 
	tcomp_params_destroy,
	0
};
const void *tcomp_params_def_t = &tcomp_params_def_s;
