// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <vector>
#include <set>
#include <map>
#include <mutex>

#include "common.hpp"
#include "dmx.hpp"

namespace lxmax
{
    class dmx_buffer
	{
        std::map<universe_address, size_t> _universes;

        std::vector<dmx_value> _buffer_a;
        std::vector<dmx_value> _buffer_b;

        std::vector<dmx_value>* _current;
        std::vector<dmx_value>* _next;

        mutable std::mutex _mutex_read;
        std::mutex _mutex_write;

    public:
        dmx_buffer();

        void resize(const std::set<universe_address>& universes);

        void write(universe_address universe, channel_address channel, const dmx_value* src_buffer, size_t channel_count);

        size_t read(universe_address universe, channel_address channel, dmx_value* dst_buffer, size_t channel_count) const;

        void swap() noexcept;
	};

	
}