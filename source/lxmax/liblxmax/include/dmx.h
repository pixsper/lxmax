// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "common.h"

#include <limits.h>
#include <stdint.h>

#include "net.h"

BEGIN_USING_C_LINKAGE

#define LX_DMX_UNIVERSE_LENGTH 512
#define LX_UNIVERSE_MIN 0
#define LX_UNIVERSE_MAX 63999
#define LX_UNIVERSE_ARTNET_MIN 0
#define LX_UNIVERSE_ARTNET_MAX 32767
#define LX_UNIVERSE_SACN_MIN 1
#define LX_UNIVERSE_SACN_MAX 63999
#define LX_PRIORITY_SACN_MIN 1
#define LX_PRIORITY_SACN_MAX 200
#define LX_DMX_FRAMERATE_MIN 0.1
#define LX_DMX_FRAMERATE_MAX 44

#define LX_DMX_8_BIT_MAX UINT8_MAX
#define LX_DMX_16_BIT_MAX UINT16_MAX
#define LX_DMX_24_BIT_MAX 16777215
#define LX_DMX_32_BIT_MAX UINT_MAX

typedef uint8_t dmx_value;
typedef int universe_address;
typedef int channel_address;
typedef int local_channel_address;

typedef enum _lx_dmx_ip_protocol
{
	IP_DMX_PROTOCOL_ARTNET,
	IP_DMX_PROTOCOL_SACN

} e_lx_dmx_ip_protocol;

DLL_API t_symbol* lx_dmx_universe_to_artnet_format(t_atom_long address);
DLL_API t_max_err lx_dmx_universe_from_artnet_format(t_symbol* s, t_atom_long* address);

DLL_API void lx_dmx_universe_to_sacn_multicast_ip(t_atom_long address, t_lx_ip_address* multicast_ip);
DLL_API t_max_err lx_dmx_universe_from_sacn_multicast_ip(const t_lx_ip_address* multicast_ip, t_atom_long* address);

END_USING_C_LINKAGE