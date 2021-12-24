// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <array>
#include <functional>
#include <vector>
#include <asio.hpp>

namespace lxmax
{
	using asio::ip::udp;

	using udp_receive_buffer = std::array<uint8_t, 1024>;
	using udp_receive_callback = std::function<void(const udp::endpoint&, const udp_receive_buffer&, size_t)>;

	class udp_socket_service
	{
		udp::socket _socket;
		udp::endpoint _local_endpoint;

		udp_receive_callback _callback;

		std::array<uint8_t, 1024> _buffer;
		udp::endpoint _remote_endpoint;

	public:
		udp_socket_service(asio::io_service& service, udp::endpoint local_endpoint, udp_receive_callback callback);

		bool send(const uint8_t* buffer, size_t buffer_length, const udp::endpoint& destination);

		void join_multicast_group(const asio::ip::address_v4& multicast_address);

		void leave_multicast_group(const asio::ip::address_v4& multicast_address);

	private:
		void start_receive();
	};
}