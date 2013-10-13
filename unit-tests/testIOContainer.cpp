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

#include <cstdio>
#include <cstring>
#include <utility>
#include <iostream>
#include <nina.h>

#if defined (NINA_WIN32)
#define snprintf _snprintf
#endif // !NINA_WIN32

void		testIOContainer()
{
	char* 				hello = new char[6];
	char*				world = new char[6];
	char*				tmp = new char[10];

	snprintf(hello, 6, "Hello");
	snprintf(world, 6, "World");

	NINA::IOContainer			ioc;
	NINA::IOContainer::IOPair	pair;

	ioc << NINA::IOContainer::IOPair(hello, 6);
	ioc << NINA::IOContainer::IOPair(world, 6);
	ioc << NINA::IOContainer::IOPair(world, 6);
	ioc[2] = NINA::IOContainer::IOPair(tmp, 10);
	memset(ioc[2].first, 0, ioc[2].second);
	ioc << std::make_pair(hello, 6);

	std::cout << "Should print 'Hello World  ' :" << std::endl << std::endl;

	ioc >> pair;
	std::cout << static_cast<char*> (pair.first) << " ";
	ioc.remove(2);
	for (size_t i = 0; i < ioc.getSize(); ++i) {
		std::cout << static_cast<char*> (ioc[i].first) << " ";
	}
	ioc.clear();
	for (size_t i = 0; i < ioc.getSize(); ++i) {
		std::cout << static_cast<char*> (ioc[i].first);
	}

	delete[] hello;
	delete[] world;
	delete[] tmp;

	std::cout << std::endl;
	std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
