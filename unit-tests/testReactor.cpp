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

int			timeoutFunction(NINA::Sock& sap)
{
	std::cout << "Timeout on handle : " << sap.getHandle() << std::endl;
	return 0;
}

int			writeFunction(NINA::Sock& sap)
{
	std::cout << "Permission to write on handle : " << sap.getHandle() << std::endl;
	return -1;
}

int			closeFunction(NINA::Sock&)
{
	std::cout << "Should be triggered by the write function returning -1" << std::endl;
	return 0;
}

void		testReactor()
{
	NINA::Reactor<NINA::PollPolicy>					react;
	NINA::Time										t(2, 0);
	NINA::EventHandlerAdapter<NINA::SockDatagram>	adapt(NINA::Addr::sapAny, AF_INET6, "udp");

	adapt.setTimeoutHandler(timeoutFunction);
	adapt.setWriteHandler(writeFunction);
	adapt.setCloseHandler(closeFunction);
	if (react.registerHandler(&adapt, NINA::Events::READ) == -1)
		std::cout << "Register handler failed" << std::endl;
	if (react.handleEvents(&t) == -1)
		std::cout << "Error while handling events" << std::endl;
	if (react.registerHandler(&adapt, NINA::Events::WRITE) == -1)
		std::cout << "Register handler failed" << std::endl;
	if (react.handleEvents(0) == -1)
		std::cout << "Error while handling events" << std::endl;
	std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
