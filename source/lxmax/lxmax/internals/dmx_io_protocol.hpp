// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <unordered_set>
#include <asio.hpp>

#include "lx_dmx.h"
#include "lx_universe_config.h"

namespace lxmax
{
	using dmx_received_callback = std::function<void(e_lx_dmx_ip_protocol, t_universe_address, t_dmx_value*, size_t)>;

	class dmx_buffer;

	class dmx_io_protocol
	{
	protected:
		asio::io_context& _io_context;
		dmx_received_callback _callback;

		dmx_io_protocol(asio::io_context& io_context, dmx_received_callback callback)
				: _io_context(io_context),
				_callback(callback)
		{

		}

	public:
		virtual void add_universe(t_lx_universe_config* config) = 0;
		virtual void update_universe(t_lx_universe_config* config) = 0;
		virtual void remove_universe(t_symbol* name) = 0;
		virtual void clear_universes(t_lx_universe_config* config) = 0;

		virtual std::unordered_set<t_symbol*> universes() const = 0;

		virtual void send_data(const dmx_buffer& buffer) = 0;
	};

	std::unique_ptr<dmx_io_protocol> create_protocol(e_lx_universe_protocol protocol,
	                                                 asio::io_context& io_context,
	                                                 dmx_received_callback callback);
}