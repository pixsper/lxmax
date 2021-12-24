// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <vector>
#include <set>
#include <map>
#include <mutex>

#include "lx_common.h"
#include "lx_dmx.h"
#include "dmx_io_protocol.hpp"


namespace lxmax
{
    class dmx_buffer
	{
        std::map<t_universe_address, size_t> _universes;

        std::vector<t_dmx_value> _buffer_a;
        std::vector<t_dmx_value> _buffer_b;

        std::vector<t_dmx_value>* _current;
        std::vector<t_dmx_value>* _next;

        mutable std::mutex _mutex_read;
        std::mutex _mutex_write;

    public:
        dmx_buffer();

        void resize(const std::set<t_universe_address>& universes);

        void write(t_universe_address universe, t_channel_address channel, const t_dmx_value* src_buffer, size_t channel_count);

        size_t read(t_universe_address universe, t_channel_address channel, t_dmx_value* dst_buffer, size_t channel_count) const;

        void swap() noexcept;
	};
}