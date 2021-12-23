// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <asio.hpp>

#include "dmx.hpp"
#include "universe_config.hpp"
#include "udp_socket_service.hpp"

#include "dmx_buffer.hpp"
#include "packets/dmx_packet_artnet.hpp"
#include "packets/dmx_packet_sacn.hpp"

namespace lxmax
{
	using asio::ip::udp;

	class dmx_io_service
	{
		asio::io_service& _io_service;

		std::mutex _mutex;
		std::unordered_map <udp::endpoint, udp_socket_service> _sockets;
		dmx_buffer _input_buffer;
		dmx_buffer _output_buffer;

	public:
		dmx_io_service(asio::io_service& io_service)
			: _io_service(io_service)
		{
			
		}

		void set_universes(t_linklist* universes)
		{
			std::lock_guard lock(_mutex);

			std::set<universe_address> input_universe_addresses;
			std::set<universe_address> output_universe_addresses;

			std::unordered_set<udp::endpoint> local_endpoints;

			t_lx_universe_config* config = (t_lx_universe_config*)linklist_getindex(universes, 0);
			while (config)
			{
				switch (config->type)
				{
					
					case LX_UNIVERSE_TYPE_INPUT:
						input_universe_addresses.insert((universe_address)config->internal_universe);
						switch(config->protocol)
						{
							case LX_UNIVERSE_PROTOCOL_ARTNET:
								break;

							case LX_UNIVERSE_PROTOCOL_SACN:
								break;

							case LX_UNIVERSE_PROTOCOL_NONE:
							default:
								break;
						}
						break;

					case LX_UNIVERSE_TYPE_OUTPUT:
						output_universe_addresses.insert((universe_address)config->internal_universe);
						break;

					case LX_UNIVERSE_TYPE_NONE:
					default:
						break;
				}


				linklist_next(universes, config, (void**)&config);
			}

			_input_buffer.resize(input_universe_addresses);

			auto it = std::cbegin(_sockets);
			while(it != std::cend(_sockets))
			{
				if (local_endpoints.find(it->first) == std::cend(local_endpoints))
					it = _sockets.erase(it);
				else
					++it;
			}

			for(const auto& e : local_endpoints)
			{
				if (_sockets.find(e) == std::cend(_sockets))
				{
					udp_receive_callback callback;

					switch(e.port())
					{
						case k_artnet_port:
							callback = [this](const udp::endpoint& remote_endpoint, const udp_receive_buffer& buffer, size_t length)
							{
								handle_received_packet_artnet(remote_endpoint, buffer, length);
							};
							break;

						case k_sacn_port:
							callback = [this](const udp::endpoint& remote_endpoint, const udp_receive_buffer& buffer, size_t length)
							{
								handle_received_packet_sacn(remote_endpoint, buffer, length);
							};
							break;

						default:
							break;
					}

					_sockets.emplace(e, udp_socket_service(_io_service, e, callback));
				}
			}
		}

		void process_io()
		{
			std::lock_guard lock(_mutex);
		}

	private:

		void handle_received_packet_artnet(const udp::endpoint& remote_endpoint, const udp_receive_buffer& buffer, size_t length)
		{
			
		}

		void handle_received_packet_sacn(const udp::endpoint& remote_endpoint, const udp_receive_buffer& buffer, size_t length)
		{

		}


		
	};
}