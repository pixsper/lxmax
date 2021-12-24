// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <asio.hpp>

#include "lx_dmx.h"
#include "lx_universe_config.h"
#include "udp_socket_service.hpp"

#include "dmx_buffer.hpp"
#include "dmx_io_protocol.hpp"
#include "protocols/dmx_sacn_io_protocol.hpp"

namespace lxmax
{
	class dmx_io_service
	{
		asio::io_context _io_context;
		asio::any_io_executor _io_context_work;

		std::mutex _mutex;
		dmx_buffer _input_buffer;
		dmx_buffer _output_buffer;

		std::unordered_map<e_lx_universe_protocol, std::unique_ptr<dmx_io_protocol>> _protocols;

	public:
		~dmx_io_service();

		void run();

		void stop();

		void set_universes(t_hashtab* universes, t_lx_network_adapter* global_artnet_adapter, t_lx_network_adapter* global_sacn_adapter);

		void write(t_universe_address universe, t_channel_address channel, const t_dmx_value* src_buffer, size_t channel_count);

		size_t read(t_universe_address universe, t_channel_address channel, t_dmx_value* dst_buffer, size_t channel_count) const;

		void process_io();

	private:
		void dmx_received(e_lx_dmx_ip_protocol, t_universe_address, t_dmx_value*, size_t);
	};
}