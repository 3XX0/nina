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

#include <algorithm>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <nina.h>

void		hexDump(std::string const& src)
{
	std::ostringstream	dump;

	dump << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
	std::copy(src.begin(), src.end(), std::ostream_iterator<unsigned int>(dump, " "));
	std::cout << dump.str() << std::endl;
}

void		testPacket()
{
	NINA::InputPacket	ipacket;
	NINA::OutputPacket	opacket;

	NINA::CDR::Char		boolean = -1;
	NINA::CDR::String	hello("Hello world!");
	NINA::CDR::Char		eor = '\0';
	NINA::CDR::String	test("This is a test");
	NINA::CDR::Integer	i = 0xAABBCCDD;
	NINA::CDR::Long		c = 0x5B1122334455665DULL;
	NINA::CDR::UChar	sh = 1;

	opacket << boolean << hello << eor << test << eor << i << c << sh << sh;

	std::cout << "Output packet dump :\t";
	hexDump(opacket.dump());
	ipacket.assign(opacket.dump());
	std::cout << "Input packet dump :\t";
	hexDump(ipacket.dump());
	std::cout << std::endl;

	NINA::CDR::Boolean  rboolean;
	NINA::CDR::String	rhello;
	NINA::CDR::String	rtest;
	NINA::CDR::Integer	ri1;
	NINA::CDR::Integer	ri1b;
	NINA::CDR::Integer	ri2;
	NINA::CDR::UShort	rs;
	NINA::CDR::UChar	rc1;
	NINA::CDR::UChar	rc2;
	NINA::CDR::UShort	num;

	ipacket >> rboolean >> rhello >> rtest;
	ipacket.peek(ri1b);
	ipacket >> ri1 >> rc1 >> rs >> ri2 >> rc2 >> num;

	std::cout << "Result should be :\ttrue Hello world! This is a test 0xAABBCCDD 0xAABBCCDD [ 0x1122 0x33445566 ] 0x101 0" << std::endl;
	std::cout << "->\t\t\t" << std::boolalpha << rboolean << " ";
	std::cout << rhello << " ";
	std::cout << rtest << " ";
	std::cout << "0x" << std::uppercase << std::hex << ri1 << " 0x" << ri1b << " ";
	std::cout << rc1 << " ";
	std::cout << "0x" << rs << " ";
	std::cout << "0x" << ri2 << " ";
	std::cout << rc2 << " ";
	std::cout << "0x" << num << " ";

	ipacket >> hello;
	std::cout << hello.size() << std::endl << std::endl;

	NINA::PacketFactory<> pfactory("separator");

	pfactory.pushStream("Hello W");
	pfactory.pushStream("orld!separator");
	pfactory.pushStream("It's meseparator");
	pfactory.pushStream("1234567891011121314151617181920");
	pfactory.pushStream(" ...separator");
	pfactory.pushStream("separatorTest, ");
	pfactory.pushStream("this is a test");
	pfactory.pushStream("separatorseparator");
	pfactory.pushStream("a");
	pfactory.pushStream("b");
	pfactory.pushStream("c");
	pfactory.pushStream("d");
	pfactory.pushStream("ef");
	pfactory.pushStream("gseparator");
	for (NINA::PacketFactory<>::const_iterator i = pfactory.begin(); i != pfactory.end(); ++i)
		std::cout << i->dump() << std::endl;
	std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
