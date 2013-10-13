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

#include <iostream>
#include <nina.h>

void		testSock()
{
	NINA::SockDatagram	sock;
	char				buf[1024];
	int					on = 1;
	int					onLen = sizeof on;
	
	std::cout << "Creating UDP socket ... ";
	if (sock.open(NINA::Addr::sapAny, AF_INET6) == -1)
		std::cout << "[FAILED]" << std::endl;
	else
		std::cout << "[OK]" << std::endl;
	std::cout << "Setting non block mode ... ";
	if (sock.enable(NINA::SAP::NON_BLOCK) == -1)
		std::cout << "[FAILED]" << std::endl;
	else
		std::cout << "[OK]" << std::endl;
	std::cout << "Call to receive, should not block !" << std::endl;
	sock.receive(buf, sizeof buf);
	std::cout << "Setting broadcast message = true ... ";
	if (sock.setOption(SOL_SOCKET, SO_BROADCAST, &on, sizeof on) == -1)
		std::cout << "[FAILED]" << std::endl;
	else
		std::cout << "[OK]" << std::endl;
	on = 0;
	std::cout << "Retrieving broadcast option : ";
	if (sock.getOption(SOL_SOCKET, SO_BROADCAST, &on, &onLen) == -1)
		std::cout << "[FAILED]" << std::endl;
	else
		on == 1 ? std::cout << "true" << std::endl : std::cout << "[FAILED] (false)" << std::endl;
	std::cout << "Closing socket ... ";
	if (sock.close() == -1)
		std::cout << "[FAILED]" << std::endl;
	else
		std::cout << "[OK]" << std::endl;
	std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
