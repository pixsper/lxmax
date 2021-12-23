// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "dmx.h"

#include <regex>

t_symbol* lx_dmx_universe_to_artnet_format(t_atom_long address)
{
	const t_atom_long net = (address & 0x7F00) >> 8;
	const t_atom_long subnet = (address & 0x00F0) >> 4;
	const t_atom_long universe = address & 0x000F;

	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%ld.%ld.%ld", net, subnet, universe);

	return gensym(buffer);
}

t_max_err lx_dmx_universe_from_artnet_format(t_symbol* s, t_atom_long* address)
{
	const std::regex artnet_universe_regex(R"((?:(\d+)[^\d]+)(?:(\d+)[^\d]+)(\d+))");

	std::cmatch matches;
	if (std::regex_match(s->s_name, matches, artnet_universe_regex))
	{
		int net = 0;
		int subnet = 0;
		int universe;

		switch (matches.size())
		{
			case 2:
				universe = strtol(matches[1].str().c_str(), nullptr, 10);
				break;

			case 3:
				subnet = strtol(matches[1].str().c_str(), nullptr, 10);
				universe = strtol(matches[2].str().c_str(), nullptr, 10);
				break;

			case 4:
				net = strtol(matches[1].str().c_str(), nullptr, 10);
				subnet = strtol(matches[2].str().c_str(), nullptr, 10);
				universe = strtol(matches[3].str().c_str(), nullptr, 10);
				break;

			default:
				return false;
		}

		*address = (net << 8) + (subnet << 4) + universe;
		return MAX_ERR_NONE;
	}
	else
	{
		return MAX_ERR_GENERIC;
	}
}

void lx_dmx_universe_to_sacn_multicast_ip(t_atom_long address, t_lx_ip_address* multicast_ip)
{
	multicast_ip->data[0] = 239;
	multicast_ip->data[1] = 255;
	multicast_ip->data[2] = (address & 0xFF00) >> 8;
	multicast_ip->data[3] = address & 0x00FF;
}

t_max_err lx_dmx_universe_from_sacn_multicast_ip(const t_lx_ip_address* multicast_ip, t_atom_long* address)
{
	if (multicast_ip->data[0] != 239 || multicast_ip->data[1] != 255)
		return MAX_ERR_GENERIC;

	const t_atom_long value = (multicast_ip->data[2] << 8) + multicast_ip->data[3];

	if (value < LX_UNIVERSE_SACN_MIN || value > LX_UNIVERSE_SACN_MAX)
		return MAX_ERR_GENERIC;

	*address = value;
	return MAX_ERR_NONE;
}
