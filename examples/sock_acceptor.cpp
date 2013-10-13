// Copyright (c) 2011, Jonathan Calmels <jbjcalmels@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <nina.h>

int 						main(void)
{
	NINA::SockAcceptor		passive_socket;
	NINA::SockStream		new_socket;
	NINA::InetAddr			local_address;
	NINA::InetAddr			peer_address;

	NINA::Init  wsa;
	local_address.wildcardQuery("1234", "tcp");
	passive_socket.open(local_address, AF_INET6, "any", true);
	std::cout << "Listenning on wildcard address, port : " << local_address.getPortByNumber() << std::endl;
	passive_socket.accept(new_socket, &peer_address);
	std::cout << "New guest : " << peer_address.getHostAddr() << "/" << peer_address.getPortByNumber() << std::endl;
	new_socket.getRemoteAddr(peer_address);
	std::cout << "New guest by getRemoteAddr() : " << peer_address.getHostAddr() << "/" << peer_address.getPortByNumber() << std::endl;
	return (0);
}
