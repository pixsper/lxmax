// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "dmx_io_protocol.hpp"

#include "protocols/dmx_artnet_io_protocol.hpp"
#include "protocols/dmx_sacn_io_protocol.hpp"

namespace lxmax
{
	std::unique_ptr<dmx_io_protocol> create_protocol(e_lx_universe_protocol protocol,
													 asio::io_context& io_context,
													 dmx_received_callback callback)
	{
		switch (protocol)
		{
			case LX_UNIVERSE_PROTOCOL_ARTNET:
				return std::make_unique<dmx_artnet_io_protocol>(io_context, callback);

			case LX_UNIVERSE_PROTOCOL_SACN:
				return std::make_unique<dmx_sacn_io_protocol>(io_context, callback);

			default:
				return nullptr;
		}
	}
}