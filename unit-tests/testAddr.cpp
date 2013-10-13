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

void		testAddr()
{
	if (NINA::Addr::sapAny == NINA::Addr::sapAny)
		std::cout << "static members sapAny equal : [OK]" << std::endl;
	{
		NINA::InetAddr addr(AF_UNSPEC);

		std::cout << "Testing scoping on eth0 ... ";
		addr.remoteQuery("::1", 80, "udp");
		if (addr.setScopeId("eth0") < 0)
			std::cout << "[FAILED]" << std::endl;
		else
			std::cout << addr.getScopeId() << " [OK]" << std::endl;
		std::cout << "Testing scoping on re0 ... ";
		addr.remoteQuery("::1", 80, "udp");
		if (addr.setScopeId("re0") < 0)
			std::cout << "[FAILED]" << std::endl;
		else
			std::cout << addr.getScopeId() << " [OK]" << std::endl;
	}
	std::cout << std::endl;	
	{
		NINA::InetAddr addr(AF_UNSPEC);

		addr.wildcardQuery("80", IPPROTO_TCP);
		std::cout << "UNSPEC -> Wildcard on port 80, IPPROTO_TCP : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByName() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_INET);

		addr.wildcardQuery("80", IPPROTO_TCP);
		std::cout << "AF_INET -> Wildcard on port 80, IPPROTO_TCP : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByNumber() << std::endl;
		std::cout << "String protocol : " << addr.getProtocolByName() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_INET6);

		addr.wildcardQuery("80", IPPROTO_TCP);
		std::cout << "AF_INET6 -> Wildcard on port 80, IPPROTO_TCP : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByName() << std::endl;
		addr.setPort("ftp");
		std::cout << "Changing port to ftp : " << addr.getPortByNumber() << " (" <<  addr.getPortByName() << ")" << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_UNSPEC);

		addr.remoteQuery("127.0.0.1", "www", "tcp");
		std::cout << "UNSPEC -> 127.0.0.1 on service www, tcp : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByNumber() << std::endl;
		std::cout << "Is mapped : " << std::boolalpha << addr.isV4Mapped() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_INET6);

		try {
			addr.remoteQuery("127.0.0.1", "www", "tcp");
			std::cout << "AF_INET6 -> 127.0.0.1 on service www, tcp : [OK]" << std::endl;
			std::cout << "Retrieving address ..." << std::endl;
			std::cout << addr.getHostAddr() << ":" << addr.getPortByNumber() << std::endl;
			std::cout << "Is mapped : " << std::boolalpha << addr.isV4Mapped() << std::endl;
			std::cout << "Has ipv4 : " << std::boolalpha << addr.hasIpv4Addr() << std::endl;
			std::cout << "Has ipv6 : " << std::boolalpha << addr.hasIpv6Addr() << std::endl;
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "Should fail on BSD operating systems : " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_UNSPEC);

		addr.forceSockType(SOCK_STREAM);
		std::cout << "Forcing Socktype to SOCK_STREAM : [OK]" << std::endl;
		addr.remoteQuery("localhost", "www", "tcp");
		std::cout << "UNSPEC -> localhost on service www, tcp : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByName() << std::endl;
		std::cout << "Is mapped : " << std::boolalpha << addr.isV4Mapped() << std::endl;
		std::cout << "Has ipv4 : " << std::boolalpha << addr.hasIpv4Addr() << std::endl;
		std::cout << "Has ipv6 : " << std::boolalpha << addr.hasIpv6Addr() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_INET6);

		addr.forceSockType(SOCK_STREAM);
		std::cout << "Forcing Socktype to SOCK_STREAM : [OK]" << std::endl;
		addr.remoteQuery("::1", "www", "any", NINA::InetAddr::SERV_RESOLUTION);
		std::cout << "AF_INET6 -> ::1 on service www, any : [OK]" << std::endl;
		std::cout << "Retrieving address ..." << std::endl;
		std::cout << "Reverse DNS : " << addr.getHostName() << std::endl;
		std::cout << addr.getHostAddr() << ":" << addr.getPortByNumber() << std::endl;
		std::cout << "Is loopback : " << std::boolalpha << addr.isLoopback() << std::endl;
		std::cout << "Has ipv4 : " << std::boolalpha << addr.hasIpv4Addr() << std::endl;
		std::cout << "Has ipv6 : " << std::boolalpha << addr.hasIpv6Addr() << std::endl;
	}
	std::cout << std::endl;
	{
		NINA::InetAddr addr(AF_INET);

		try {
			addr.remoteQuery("::1", "www", "any", NINA::InetAddr::SERV_RESOLUTION);
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "::1 with AF_INET should fail : " << e.what() << std::endl;
		}
	}
	{
		NINA::InetAddr addr(AF_INET6);

		try {
			addr.remoteQuery("::1", "www", "any", 0);
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "www without SERV_RESOLUTION should fail : " << e.what() << std::endl;
		}
	}
	{
		NINA::InetAddr addr(AF_INET6);

		try {
			addr.remoteQuery("127.0.0.1", 80, NINA::InetAddr::ANY_PROTO, 0);
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "127.0.0.1 without IPV4_MAPPED should fail : " << e.what() << std::endl;
		}
	}
	{
		NINA::InetAddr addr(AF_INET6);

		try {
			addr.remoteQuery("localhost", 80, NINA::InetAddr::ANY_PROTO, NINA::InetAddr::IPV4_MAPPED);
		}
		catch (NINA::Error::SystemError const& e) {
			std::cout << "localhost without NAME_RESOLUTION should fail : " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

#if defined (NINA_WIN32)
	system("pause");
#endif // !NINA_WIN32
}
