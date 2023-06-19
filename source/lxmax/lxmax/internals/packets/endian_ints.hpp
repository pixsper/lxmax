// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <cstdint>

namespace lxmax
{
#pragma pack(push, 1)

	struct uint16_t_be
	{
		uint8_t bytes[2];

		uint16_t_be() = default;

		uint16_t_be(uint16_t value)
		{
			bytes[0] = value >> 8;
			bytes[1] = value & 0x00FF;
		}

		operator uint16_t() const
		{
			return (bytes[0] << 8) + bytes[1];
		}
	};

	struct uint32_t_be
	{
		uint8_t bytes[4];

		uint32_t_be() = default;

		uint32_t_be(uint32_t value)
		{
			bytes[0] = value >> 24;
			bytes[1] = (value & 0x00FF0000) >> 16;
			bytes[2] = (value & 0x0000FF00) >> 8;
			bytes[3] = value & 0x00FF;
		}

		operator uint32_t() const
		{
			return (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
		}
	};

#pragma pack(pop)

}
