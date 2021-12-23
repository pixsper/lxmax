// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "udp_socket_service.hpp"

lxmax::udp_socket_service::udp_socket_service(asio::io_service& service, udp::endpoint local_endpoint, udp_receive_callback callback)
	: _socket(service),
	_local_endpoint(std::move(local_endpoint)),
	_callback(std::move(callback))
{
	_socket.open(_local_endpoint.protocol());
	_socket.set_option(udp::socket::reuse_address(true));
	_socket.bind(_local_endpoint);

	start_receive();
}

bool lxmax::udp_socket_service::send(const uint8_t* buffer, size_t buffer_length, const udp::endpoint& destination)
{
	const size_t bytes_sent = _socket.send_to(asio::buffer(buffer, buffer_length), destination);
	return bytes_sent == buffer_length;
}

void lxmax::udp_socket_service::join_multicast_group(const asio::ip::address_v4& multicast_address)
{
	_socket.set_option(asio::ip::multicast::join_group(multicast_address));
}

void lxmax::udp_socket_service::leave_multicast_group(const asio::ip::address_v4& multicast_address)
{
	_socket.set_option(asio::ip::multicast::leave_group(multicast_address));
}

void lxmax::udp_socket_service::start_receive()
{
	_socket.async_receive_from(
		asio::buffer(_buffer), _remote_endpoint,
		[this](const asio::error_code& error, std::size_t bytes_received)
		{
			if (!error || error == asio::error::message_size)
			{
				_callback(_remote_endpoint, _buffer, bytes_received);
				start_receive();
			}
		});
}
