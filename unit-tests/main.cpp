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

void testAddr();
void testSock();
void testTime();
void testIOContainer();
void testReactor();
void testPacket();

int 			main(void)
{
	{
		try {
			NINA::Init	wsa("0.5");
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "WSA Init must failed here : " << e.what() << std::endl;
		}
		std::cout << std::endl;
	}

	NINA::Init	wsa;
	
	std::cout << "-------------- TESTING ADDRESSING --------------" << std::endl << std::endl;
	testAddr();
	std::cout << "-------------- TESTING TIME --------------" << std::endl << std::endl;
	testTime();
	std::cout << "-------------- TESTING CONTAINERS --------------" << std::endl << std::endl;
	testIOContainer();
	std::cout << "-------------- TESTING SOCKETS --------------" << std::endl << std::endl;
	testSock();
	std::cout << "-------------- TESTING REACTOR --------------" << std::endl << std::endl;
	testReactor();
	std::cout << "-------------- TESTING PACKET --------------" << std::endl << std::endl;
	testPacket();
	return 0;
}
