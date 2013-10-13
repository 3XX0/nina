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

/*!
 * @file NinaInetAddr.cpp
 * @brief Implements the internet domain addressing
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#include <sstream>
#include "NinaDef.hpp"
#include "NinaInetAddr.hpp"

#if defined (NINA_POSIX)
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <net/if.h>
#endif // !NINA_POSIX
#if defined (NINA_WIN32)
# include <Windows.h>
# include <netioapi.h>
#endif // !NINA_WIN32

#include "NinaSystemError.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

InetAddr::InetAddr(int family)
	: Addr(family, determineSize(family)),
	mForcedSockType(ANY_SOCK)
{ 
	reset();
}

InetAddr::~InetAddr()
{
}

InetAddr::InetAddr(InetAddr const& sap)
	: Addr(sap)
{
	mHostAddr = sap.mHostAddr;
	mHostName = sap.mHostName;
	mForcedSockType = sap.mForcedSockType;
	mSockType = sap.mSockType;
	::memcpy(&mInetAddr, &sap.mInetAddr, sizeof mInetAddr);
}

InetAddr&
InetAddr::operator=(InetAddr const& sap)
{
	if (this != &sap) {
		Addr::operator=(sap);
		mHostAddr = sap.mHostAddr;
		mHostName = sap.mHostName;
		mForcedSockType = sap.mForcedSockType;
		mSockType = sap.mSockType;
		::memcpy(&mInetAddr, &sap.mInetAddr, sizeof mInetAddr);
	}
	return *this;
}

int
InetAddr::setAddr(void* addr, size_t addrLen)
{
	int		size;
	int		family;

	reset();
	mForcedSockType = ANY_SOCK;
	mFamily = AF_UNSPEC;
	if (addrLen > sizeof(sockaddr_storage))
		return -1;
	::memcpy(&mInetAddr, addr, addrLen);
	::memset(reinterpret_cast<char*>(&mInetAddr) + addrLen, 0, sizeof(sockaddr_storage) - addrLen);
	size = getRealSize();
	family = getRealFamily();
	if (family < 0 || size < 0)
		return -1;
	setSize(size);
	setFamily(family);
	return 0;
}

void
InetAddr::remoteQuery(std::string const& host, std::string const& port,
			int protocol, uint8_t mode)
{
	int			r;
	addrinfo	hints;
	addrinfo	*res;

	reset();
	::memset(&hints, 0, sizeof(hints));
#if !defined (NINA_LACK_OF_V4MAPPED)
	if ((mode & IPV4_MAPPED) != 0)
		hints.ai_flags |= AI_V4MAPPED;
#endif // !NINA_LACK_OF_V4MAPPED
	if ((mode & NAME_RESOLUTION) == 0)
		hints.ai_flags |= AI_NUMERICHOST;
	if ((mode & SERV_RESOLUTION) == 0)
		hints.ai_flags |= AI_NUMERICSERV;
	hints.ai_family = mFamily;
	hints.ai_protocol = protocol;
	hints.ai_socktype = mForcedSockType;
	if ((r = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError(::gai_strerror(r));
#endif // !NINA_WIN32
	::memcpy(&mInetAddr, res->ai_addr, res->ai_addrlen);
	setSize(res->ai_addrlen);
	setFamily(res->ai_family);
	mSockType = res->ai_socktype;
	mProtocol = res->ai_protocol;
	::freeaddrinfo(res);
	mForcedSockType = ANY_SOCK;
 }

void
InetAddr::remoteQuery(std::string const& host, uint16_t port,
			int protocol, uint8_t mode)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	mode &= ~SERV_RESOLUTION;
	remoteQuery(host, cPort, protocol, mode);
}

void
InetAddr::remoteQuery(std::string const& host, std::string const& port,
			std::string const& protocol, uint8_t mode)
{
	protoent*			pRes;
	OS::NINAProtoBuf	pBuf;

	if (protocol.compare("any") == 0)
		remoteQuery(host, port, ANY_PROTO, mode);
	else {
		if ((pRes = OS::getProtoByName(protocol.c_str(), &pBuf)) == 0)
#if defined (NINA_WIN32)
			throw Error::SystemError(OS::setErrnoToWSALastError());
#else
			throw Error::SystemError("Could not record protocol");
#endif // !NINA_WIN32
		remoteQuery(host, port, pRes->p_proto, mode);
		delete pRes;
	}
}

void
InetAddr::remoteQuery(std::string const& host, uint16_t port,
			std::string const& protocol, uint8_t mode)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	mode &= ~SERV_RESOLUTION;
	remoteQuery(host, cPort, protocol, mode);
}

void
InetAddr::localQuery(std::string const& host, std::string const& port,
			int protocol, uint8_t mode)
{
	int			r;
	addrinfo	hints;
	addrinfo	*res;

	reset();
	::memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	if ((mode & NAME_RESOLUTION) == 0)
		hints.ai_flags |= AI_NUMERICHOST;
	if ((mode & SERV_RESOLUTION) == 0)
		hints.ai_flags |= AI_NUMERICSERV;
	hints.ai_family = mFamily;
	hints.ai_protocol = protocol;
	hints.ai_socktype = mForcedSockType;
	if ((r = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError(::gai_strerror(r));
#endif // !NINA_WIN32
	::memcpy(&mInetAddr, res->ai_addr, res->ai_addrlen);
	setSize(res->ai_addrlen);
	setFamily(res->ai_family);
	mSockType = res->ai_socktype;
	mProtocol = res->ai_protocol;
	::freeaddrinfo(res);
	mForcedSockType = ANY_SOCK;
 }

void
InetAddr::localQuery(std::string const& host, uint16_t port,
			int protocol, uint8_t mode)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	mode &= ~SERV_RESOLUTION;
	localQuery(host, cPort, protocol, mode);
}

void
InetAddr::localQuery(std::string const& host, std::string const& port,
			std::string const& protocol, uint8_t mode)
{
	protoent*			pRes;
	OS::NINAProtoBuf	pBuf;

	if (protocol.compare("any") == 0)
		localQuery(host, port, ANY_PROTO, mode);
	else {
		if ((pRes = OS::getProtoByName(protocol.c_str(), &pBuf)) == 0)
#if defined (NINA_WIN32)
			throw Error::SystemError(OS::setErrnoToWSALastError());
#else
			throw Error::SystemError("Could not record protocol");
#endif // !NINA_WIN32
		localQuery(host, port, pRes->p_proto, mode);
		delete pRes;
	}
}

void
InetAddr::localQuery(std::string const& host, uint16_t port,
			std::string const& protocol, uint8_t mode)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	mode &= ~SERV_RESOLUTION;
	localQuery(host, cPort, protocol, mode);
}

void
InetAddr::wildcardQuery(std::string const& port, int protocol, uint8_t mode)
{
	int			r;
	addrinfo	hints;
	addrinfo	*res;

	reset();
	::memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	if ((mode & SERV_RESOLUTION) == 0)
		hints.ai_flags |= AI_NUMERICSERV;
	hints.ai_family = mFamily;
	hints.ai_protocol = protocol;
	hints.ai_socktype = mForcedSockType;
	if ((r = ::getaddrinfo(0, port.c_str(), &hints, &res)) != 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError(::gai_strerror(r));
#endif // !NINA_WIN32
	::memcpy(&mInetAddr, res->ai_addr, res->ai_addrlen);
	setSize(res->ai_addrlen);
	setFamily(res->ai_family);
	mSockType = res->ai_socktype;
	mProtocol = res->ai_protocol;
	::freeaddrinfo(res);
	mForcedSockType = ANY_SOCK;
}

void
InetAddr::wildcardQuery(uint16_t port, int protocol)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	wildcardQuery(cPort, protocol, 0);
}

void
InetAddr::wildcardQuery(std::string const& port, std::string const& protocol, uint8_t mode)
{
	protoent*			pRes;
	OS::NINAProtoBuf	pBuf;

	if (protocol.compare("any") == 0)
		wildcardQuery(port, ANY_PROTO, mode);
	else {
		if ((pRes = OS::getProtoByName(protocol.c_str(), &pBuf)) == 0)
#if defined (NINA_WIN32)
			throw Error::SystemError(OS::setErrnoToWSALastError());
#else
			throw Error::SystemError("Could not record protocol");
#endif // !NINA_WIN32
		wildcardQuery(port, pRes->p_proto, mode);
		delete pRes;
	}	
}

void
InetAddr::wildcardQuery(uint16_t port, std::string const& protocol)
{
	std::stringstream	ss;
	std::string			cPort;

	ss << port;
	ss >> cPort;
	wildcardQuery(cPort, protocol, 0);
}

std::string
InetAddr::getPortByName() const
{
	servent*			sRes;
	uint16_t			portNum;
	std::string			port;
	std::string			protocol;
	OS::NINAServBuf		sBuf;	

	if ((portNum = getPortByNumber()) == 0)
		throw Error::SystemError("Specified port is unknown");
	try {
		protocol = getProtocolByName();
	} 
	catch (...) {
		throw;
	} 
	if ((sRes = OS::getServByPort(htons(portNum), protocol.c_str(), &sBuf)) == 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError("Could not find suitable service");
#endif // !NINA_WIN32
	port.assign(sRes->s_name);
	delete sRes;
	return port;
} 

uint16_t
InetAddr::getPortByNumber() const
{
	if (hasIpv4Addr()) {
		sockaddr_in const* sa = reinterpret_cast<sockaddr_in const*> (&mInetAddr);
		return ntohs(sa->sin_port);
	}
	else if (hasIpv6Addr()) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		return ntohs(sa->sin6_port);
	}
	return -1;
}

void
InetAddr::setPort(std::string const& port)
{
	servent*			sRes;
	std::string			protocol;
	OS::NINAServBuf     sBuf;
	uint16_t			portNum;

	try {
		protocol = getProtocolByName();
	}
	catch (...) {
		throw;
	}
	if ((sRes = OS::getServByName(port.c_str(), protocol.c_str(), &sBuf)) == 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError("Could not record service");
#endif // !NINA_WIN32
	portNum = ntohs(sRes->s_port);
	delete sRes;
	if (setPort(portNum) < 0)
		throw Error::SystemError("Address family unknown");
}

int
InetAddr::setPort(uint16_t port)
{
	if (hasIpv4Addr()) {
		sockaddr_in* sa = reinterpret_cast<sockaddr_in*> (&mInetAddr);
		sa->sin_port = htons(port);
		return 0;
	}
	else if (hasIpv6Addr()) {
		sockaddr_in6* sa = reinterpret_cast<sockaddr_in6*> (&mInetAddr);
		sa->sin6_port = htons(port);
		return 0;
	}
	return -1;
}

std::string
InetAddr::getHostName()
{
	int				sz;
	NameInfoBuf		dst;

	if (mHostName.empty() == false)
		return mHostName;
	sz = getRealSize();
	if (sz > 0 && ::getnameinfo(reinterpret_cast<sockaddr const*> (&mInetAddr),
				sz, dst, sizeof dst, 0, 0, NI_NAMEREQD) == 0)
		mHostName.assign(dst);
	else if (sz < 0)
		OS::setLastError(EAI_FAMILY); // WSAEAFNOSUPPORT (Microsoft Windows)
	else
		OS::setErrnoToWSALastError();
	return mHostName;
}

std::string
InetAddr::getHostAddr()
{
	char			dst[INET6_ADDRSTRLEN];

	if (mHostAddr.empty() == false)
		return mHostAddr;
#if defined (NINA_POSIX)
	int				family;
	const void*		addr = 0;

	family = getRealFamily();
	if (family > 0) { 
		if (family == AF_INET)
			addr = &(reinterpret_cast<sockaddr_in const*> (&mInetAddr)->sin_addr);
		else if (family == AF_INET6)
			addr = &(reinterpret_cast<sockaddr_in6 const*> (&mInetAddr)->sin6_addr);
		if (addr && ::inet_ntop(family, addr, dst, INET6_ADDRSTRLEN) != 0)
			mHostAddr.assign(dst);
	}
#else
	int				sz;

	sz = getRealSize();
	if (sz > 0 && ::getnameinfo(reinterpret_cast<sockaddr const*> (&mInetAddr),
				sz, dst, INET6_ADDRSTRLEN, 0, 0, NI_NUMERICHOST) == 0)
		mHostAddr.assign(dst);
	else if (sz < 0)
		OS::setLastError(EAI_FAMILY); // WSAEAFNOSUPPORT (Microsoft Windows)
	else
		OS::setErrnoToWSALastError();
#endif // !NINA_POSIX
	return mHostAddr;
}

std::string
InetAddr::getProtocolByName() const
{
	protoent*			pRes;
	int					protocolNum;
	std::string			protocol;
	OS::NINAProtoBuf	pBuf;

	if ((protocolNum = getProtocolByNumber()) < 0)
		throw Error::SystemError("Specified protocol is unknown");
	if ((pRes = OS::getProtoByNumber(protocolNum, &pBuf)) == 0)
#if defined (NINA_WIN32)
		throw Error::SystemError(OS::setErrnoToWSALastError());
#else
		throw Error::SystemError("Could not find suitable protocol");
#endif // !NINA_WIN32
	protocol.assign(pRes->p_name);
	delete pRes;
	return protocol;
}

uint32_t
InetAddr::getFlowInfo() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		return sa->sin6_flowinfo;
	}
	return -1;
}

uint32_t
InetAddr::getScopeId() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		return sa->sin6_scope_id;
	}
	return -1; 
}

int
InetAddr::setScopeId(std::string const& ifaceName)
{
	uint32_t ifaceIndex;

	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		if ((ifaceIndex = ::if_nametoindex(ifaceName.c_str())) != 0) {
			setScopeId(ifaceIndex);
			return 0;
		}
	}
	return -1;
}

int
InetAddr::getRealSize() const
{
	if (mFamily == AF_UNSPEC) {
		if (hasIpv4Addr())
			return sizeof(sockaddr_in);
		else if (hasIpv6Addr())
			return sizeof(sockaddr_in6);
		return -1;
	}
	else
		return mSize;
}

int
InetAddr::getRealFamily() const
{
	if (mFamily == AF_UNSPEC) {
		if (hasIpv4Addr())
			return AF_INET;
		else if (hasIpv6Addr())
			return AF_INET6;
		return -1;
	}
	else
		return mFamily;
}

void
InetAddr::reset()
{
	::memset(&mInetAddr, 0, sizeof mInetAddr);	
	mHostAddr.clear();
	mHostName.clear();
	mSockType = -1;
	mProtocol = -1;
}

int
InetAddr::determineSize(int family) const
{ 
	switch (family) {
		case AF_INET:
			return sizeof(sockaddr_in);
			break;
		case AF_INET6:
			return sizeof(sockaddr_in6);
			break;
		case AF_UNSPEC:
			return sizeof mInetAddr;
			break;
		default:
			throw std::invalid_argument("Unknown family");
	}
	return -1;
}

NINA_END_NAMESPACE_DECL
