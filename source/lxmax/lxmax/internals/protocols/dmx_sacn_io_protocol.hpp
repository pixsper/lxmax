// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "../dmx_io_protocol.hpp"

#include <map>
#include <unordered_map>

#include "../udp_socket_service.hpp"

namespace lxmax
{
	class dmx_sacn_io_protocol final : public dmx_io_protocol
	{
		struct active_universe
		{
			t_symbol* name;
			udp::endpoint local_endpoint;
			std::vector<udp::endpoint> remote_endpoints;
			t_universe_address internal_universe;
			t_universe_address protocol_universe;
			double time_last_sent_received;
		};

		std::unordered_map<udp::endpoint, udp_socket_service> _sockets;
		std::map<t_universe_address, active_universe> _input_universes;
		std::map<t_universe_address, active_universe> _output_universes;

	public:
		dmx_sacn_io_protocol(asio::io_context& io_context, dmx_received_callback callback)
			: dmx_io_protocol(io_context, callback)
		{

		}

		void add_universe(t_lx_universe_config* config) override
		{

		}

		void update_universe(t_lx_universe_config* config) override
		{

		}

		void remove_universe(t_symbol* name) override
		{

		}

		void clear_universes(t_lx_universe_config* config) override
		{

		}

		std::unordered_set<t_symbol*> universes() const override
		{
			std::unordered_set<t_symbol*> names;

			for(const auto& u : _input_universes)
				names.insert(u.second.name);

			for(const auto& u : _output_universes)
				names.insert(u.second.name);

			return names;
		}

		void send_data(const dmx_buffer& buffer) override
		{

		}
	};
}