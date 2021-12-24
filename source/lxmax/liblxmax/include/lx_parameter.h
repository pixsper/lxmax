// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

BEGIN_USING_C_LINKAGE

typedef enum _lx_parameter_resolution
{
	LX_PARAMETER_RESOLUTION_8_BIT,
	LX_PARAMETER_RESOLUTION_16_BIT,
	LX_PARAMETER_RESOLUTION_24_BIT,
	LX_PARAMETER_RESOLUTION_32_BIT

} e_lx_parameter_resolution;

typedef enum _lx_parameter_physical_unit
{
	LX_PARAMETER_PHYSICAL_UNIT_NONE,
	LX_PARAMETER_PHYSICAL_UNIT_PERCENTAGE,
	LX_PARAMETER_PHYSICAL_UNIT_LENGTH,
	LX_PARAMETER_PHYSICAL_UNIT_ANGLE,
	LX_PARAMETER_PHYSICAL_UNIT_ANGULAR_SPEED,
	LX_PARAMETER_PHYSICAL_UNIT_FREQUENCY,
	LX_PARAMETER_PHYSICAL_UNIT_TIME
	LX_PARAMETER_PHYSICAL_UNIT_COLOR_COMPONENT,
	LX_PARAMETER_PHYSICAL_UNIT_TEMPERATURE

} e_lx_parameter_physical_unit;


typedef struct _lx_parameter
{
	t_object object;

	t_symbol* name;
	t_symbol* friendly_name;
	t_symbol* type;
	e_lx_parameter_resolution resolution;
	e_lx_parameter_physical_unit unit;

	t_atom value;

} t_lx_parameter;

END_USING_C_LINKAGE