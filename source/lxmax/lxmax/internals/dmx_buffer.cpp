// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "dmx_buffer.hpp"

#include <algorithm>
#include <iterator>

namespace lxmax
{
	dmx_buffer::dmx_buffer()
		: _current(&_buffer_a),
		_next(&_buffer_b)
	{
	}

	void dmx_buffer::resize(const std::set<t_universe_address>& universes)
	{
		std::scoped_lock lock(_mutex_write, _mutex_read);

		_next->resize(universes.size() * LX_DMX_UNIVERSE_LENGTH);

		std::map<t_universe_address, size_t> updated_universes;
		size_t i = 0;
		for (const auto& universe : universes)
		{
			updated_universes.emplace(universe, i);
			i += LX_DMX_UNIVERSE_LENGTH;

			const auto it = _universes.find(universe);
			if (it != std::cend(_universes))
				std::copy_n((*_current).data() + it->second, LX_DMX_UNIVERSE_LENGTH, (*_next).data() + i);
			else
				std::fill_n((*_next).data() + it->second, LX_DMX_UNIVERSE_LENGTH, 0);
		}

		_current->resize(universes.size() * LX_DMX_UNIVERSE_LENGTH);
		std::copy_n(_next->data(), _next->size(), _current->data());

		_universes = updated_universes;
	}

	void dmx_buffer::write(t_universe_address universe, t_channel_address channel, const t_dmx_value* src_buffer,
	                       size_t channel_count)
	{
		const auto& it = _universes.find(universe);
		if (it == std::end(_universes))
			return;

		{
			std::scoped_lock lock(_mutex_write);
			std::copy_n(src_buffer, channel_count, (*_next).data() + it->second + (channel - 1));
		}
	}

	size_t dmx_buffer::read(t_universe_address universe, t_channel_address channel, t_dmx_value* dst_buffer,
	                        size_t channel_count) const
	{
		if ((channel - 1) + channel_count > LX_DMX_UNIVERSE_LENGTH)
			return 0;

		const auto& it = _universes.find(universe);
		if (it == std::end(_universes))
			return 0;

		{
			std::scoped_lock lock(_mutex_read);
			std::copy_n((*_current).data() + it->second + (channel - 1), channel_count, dst_buffer);
			return channel_count;
		}

	}

	void dmx_buffer::swap() noexcept
	{
		std::scoped_lock lock(_mutex_write, _mutex_read);
		std::swap(_current, _next);
		std::copy_n(_current->data(), _current->size(), _next->data());
	}

}