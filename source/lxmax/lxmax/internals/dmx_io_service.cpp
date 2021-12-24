// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "dmx_io_service.hpp"

namespace lxmax
{
	dmx_io_service::~dmx_io_service()
	{
		stop();
	}

	void dmx_io_service::run()
	{
		{
			std::lock_guard lock(_mutex);
			_io_context_work = asio::require(_io_context.get_executor(),
			                                 asio::execution::outstanding_work_t::tracked);
		}
		_io_context.run();
	}

	void dmx_io_service::stop()
	{
		std::lock_guard lock(_mutex);

		if (!_io_context.stopped())
			_io_context_work = asio::any_io_executor();
	}

	void dmx_io_service::set_universes(t_hashtab* universes, t_lx_network_adapter* global_artnet_adapter,
	                                   t_lx_network_adapter* global_sacn_adapter)
	{
		std::lock_guard lock(_mutex);

		std::set<t_universe_address> input_universe_addresses;
		std::set<t_universe_address> output_universe_addresses;

		long keys_count = 0;
		t_symbol** keys = NULL;

		if (hashtab_getkeys(universes, &keys_count, &keys) != MAX_ERR_NONE)
			return;

		std::unordered_map<t_symbol*, t_lx_universe_config*> configs;

		for(long i = 0; i < keys_count; ++i)
		{
			t_lx_universe_config* config = NULL;
			if (hashtab_lookup(universes, keys[i], (t_object**)&config) != MAX_ERR_NONE)
				continue;

			if (config->protocol != LX_UNIVERSE_PROTOCOL_ARTNET && config->protocol != LX_UNIVERSE_PROTOCOL_SACN)
				continue;

			configs.insert( { config->name, config });

			switch (config->type)
			{
				case LX_UNIVERSE_TYPE_INPUT:
					input_universe_addresses.insert((t_universe_address)config->internal_universe);
					break;

				case LX_UNIVERSE_TYPE_OUTPUT:
					output_universe_addresses.insert((t_universe_address)config->internal_universe);
					break;

				case LX_UNIVERSE_TYPE_NONE:
				default:
					break;
			}
		}

		_input_buffer.resize(input_universe_addresses);
		_output_buffer.resize(output_universe_addresses);

		if (keys)
			sysmem_freeptr(keys);

		for(const auto& pair : configs)
		{
			t_lx_universe_config* config = pair.second;

			auto it = _protocols.find(config->protocol);
			if (it == std::cend(_protocols))
			{
				dmx_received_callback callback = std::bind(&dmx_io_service::dmx_received, this,
				                                           std::placeholders::_1, std::placeholders::_2,
				                                           std::placeholders::_3, std::placeholders::_4);

				it = _protocols.insert(
						{
							config->protocol,
							create_protocol(config->protocol, _io_context, callback)
						}).first;
			}

			dmx_io_protocol& protocol = *it->second;

			protocol.add_universe(config);
		}

		for(const auto& pair : _protocols)
		{
			dmx_io_protocol& protocol = *pair.second;
			auto universes = protocol.universes();

			for(t_symbol* config_name : universes)
			{
				if (configs.find(config_name) == std::cend(configs))
					protocol.remove_universe(config_name);
			}
		}
	}

	void dmx_io_service::write(t_universe_address universe, t_channel_address channel, const t_dmx_value* src_buffer,
	                           size_t channel_count)
	{
		_output_buffer.write(universe, channel, src_buffer, channel_count);
	}

	size_t dmx_io_service::read(t_universe_address universe, t_channel_address channel, t_dmx_value* dst_buffer,
	                            size_t channel_count) const
	{
		return _input_buffer.read(universe, channel, dst_buffer, channel_count);
	}

	void dmx_io_service::process_io()
	{
		std::lock_guard lock(_mutex);

		_output_buffer.swap();

		for(const auto& pair : _protocols)
			pair.second->send_data(_output_buffer);
	}

	void dmx_io_service::dmx_received(e_lx_dmx_ip_protocol protocol, t_universe_address universe,
									  t_dmx_value* buffer, size_t buffer_length)
	{

	}
}

