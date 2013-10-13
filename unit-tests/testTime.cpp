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

void		testTime()
{
	{
		NINA::Time tm1(5, 100);
		NINA::Time tm2(2, 200);

		std::cout << "tm1 = " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
		std::cout << "tm2 = " << tm2.getSeconds() << ":" << tm2.getUSeconds() << std::endl;
		tm1 += tm2;
		std::cout << "tm1 += tm2 -> " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::Time tm1(5, 100);
		NINA::Time tm2(2, 200);
		NINA::Time tm3(1, 100);
		NINA::Time tm4(1, 100);
		NINA::Time res = NINA::Time::timeNull;

		std::cout << "tm1 = " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
		std::cout << "tm2 = " << tm2.getSeconds() << ":" << tm2.getUSeconds() << std::endl;
		std::cout << "tm3 = " << tm3.getSeconds() << ":" << tm3.getUSeconds() << std::endl;
		std::cout << "tm4 = " << tm4.getSeconds() << ":" << tm4.getUSeconds() << std::endl;
		std::cout << "res = " << res.getSeconds() << ":" << res.getUSeconds() << std::endl;
		res = tm1 + tm2 + tm3 + tm4;
		std::cout << "tm1 + tm2 + tm3 + tm4" << std::endl;
		std::cout << "res = " << res.getSeconds() << ":" << res.getUSeconds() << std::endl;
		std::cout << "tm1 (unchanged ?) = " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::Time tm1(5, 100);

		std::cout << "tm1 = " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
		tm1 *= 0;
		std::cout << "tm1 *= 0 -> " << tm1.getSeconds() << ":" << tm1.getUSeconds() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::Time tm1(2, -2000500);

		std::cout << "Check normalize with 2:-2000500 ... " << tm1.getSeconds() << ":" << tm1.getUSeconds()<< std::endl;
	}
	{
		NINA::Time tm1(0, 2000000);

		std::cout << "Check normalize with 0:2000000 ... " << tm1.getSeconds() << ":" << tm1.getUSeconds()<< std::endl;
	}
	{
		NINA::Time tm1(-2, -500);

		std::cout << "Check normalize with -2:-500 ... " << tm1.getSeconds() << ":" << tm1.getUSeconds()<< std::endl;
	}
	
		NINA::Time tm1(-1, 2000000);

		std::cout << "Check normalize with -1:2000000 ... " << tm1.getSeconds() << ":" << tm1.getUSeconds()<< std::endl;
		tm1.timestamp();
		std::cout << "Current timestamp = " << tm1.getSeconds() << ":" << tm1.getUSeconds()<< std::endl;

		std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
