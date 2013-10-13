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

int 					main()
{
	NINA::SockConnector	connector;
	NINA::SockStream	socket;
	NINA::InetAddr		remote_address;
	NINA::InetAddr		local_address;

	NINA::Init	wsa;
	local_address.localQuery("localhost", 4567, "any");
	remote_address.remoteQuery("localhost", 1234, "any");
	socket.open(local_address, AF_INET6, "any");
	connector.connect(socket, remote_address);
	socket.send("Hello world !\n");
	connector.disconnect(socket, NINA::SockConnector::SHUT_ALL);
	return 0;
}
