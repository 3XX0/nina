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
 * @file NinaSock.cpp
 * @brief Implements global socket manipulations
 * @author Jonathan Calmels
 * @date Fri Jun 10 2011
 */

#include "NinaDef.hpp"

#if defined (NINA_POSIX)
# include <sys/socket.h>
#endif // !NINA_POSIX
#if defined (NINA_WIN32)
# include <winsock2.h>
#endif // !NINA_WIN32

#include "NinaSock.hpp"
#include "NinaOS.hpp"
#include "NinaSystemError.hpp"

NINA_BEGIN_NAMESPACE_DECL

Sock::Sock()
	: SAP()
{
}

Sock::~Sock()
{
	close();
}

Sock::Sock(Sock const& sap)
	: SAP(sap)
{
}

Sock&
Sock::operator=(Sock const& sap)
{
	if (this != &sap)
		SAP::operator=(sap);
	return *this;
}

int
Sock::open(Addr const& addr, int family, int protocol,
	   	int sockType, bool reuseAddr)
{ 
	int on = 1;

	mTransportEndpoint = ::socket(family, sockType, protocol);
	if (mTransportEndpoint == NINA_INVALID_HANDLE) {
		OS::setErrnoToWSALastError();
		return -1;
 	}
	if (reuseAddr == true && setOption(SOL_SOCKET, SO_REUSEADDR,
				&on, sizeof on) < 0) {
		close();
		return -1;
	}
	if (addr != Addr::sapAny && ::bind(mTransportEndpoint,
			   	static_cast<sockaddr const*> (addr.getAddr()),
			   	addr.getSize()) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		close();
		return -1;
	}
	return 0;
} 

void
Sock::open(Addr const& addr, int family, std::string const& protocol,
		int sockType, bool reuseAddr)
{
	protoent*			pRes;
	OS::NINAProtoBuf	pBuf;
	int					errCode;

	if (protocol.compare("any") == 0)
		errCode = open(addr, family, ANY_PROTO, sockType, reuseAddr);
	else {
		if ((pRes = OS::getProtoByName(protocol.c_str(), &pBuf)) == 0)
#if defined (NINA_WIN32)
			throw Error::SystemError(OS::setErrnoToWSALastError());
#else
			throw Error::SystemError("Could not record protocol");
#endif // !NINA_WIN32
		errCode = open(addr, family, pRes->p_proto, sockType, reuseAddr);
		delete pRes;
	}
	if (errCode < 0)
		throw Error::SystemError(errno);
}

int
Sock::close()
{ 
	if (mTransportEndpoint == NINA_INVALID_HANDLE)
		return -1;
	if (OS::sockClose(mTransportEndpoint) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	mTransportEndpoint = NINA_INVALID_HANDLE;
	return 0;
}

int
Sock::getOption(int level, int opt, void* buf, int* bufLen) const
{
	if (OS::getSockOpt(mTransportEndpoint, level, opt, buf, bufLen) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return 0;
}

int
Sock::setOption(int level, int opt, void const* buf, int bufLen) const
{
	if (OS::setSockOpt(mTransportEndpoint, level, opt, buf, bufLen) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return 0;
}

int
Sock::getLocalAddr(Addr& sap) const
{
	sockaddr_storage	addr;
	int					addrLen;
	int					family;

	sap.reset();
	addrLen = sizeof addr;
	if (OS::getSockName(mTransportEndpoint, reinterpret_cast<sockaddr*> (&addr),
			   	&addrLen) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	} 
	// It is safe here, to not consider errors
	// /!\ However AF_UNIX is currently not supported and could leads to failures
	sap.setAddr(&addr, addrLen);
	family = retrieveFamily(&addr);
	sap.setFamily(family);
	sap.setSize(addrLen);
	return 0;
}

int
Sock::getRemoteAddr(Addr& sap) const
{
	sockaddr_storage	addr;
	int					addrLen;
	int					family;

	sap.reset();
	addrLen = sizeof addr;
	if (OS::getPeerName(mTransportEndpoint, reinterpret_cast<sockaddr*> (&addr),
			   	&addrLen) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	// It is safe here, to not consider errors
	// /!\ However AF_UNIX is currently not supported and could leads to failures
	sap.setAddr(&addr, addrLen);
	family = retrieveFamily(&addr);
	sap.setFamily(family);
	sap.setSize(addrLen);
	return 0;
}

int
Sock::retrieveFamily(sockaddr_storage const* addr) const
{
	sockaddr const* sa = reinterpret_cast<sockaddr const*> (addr);
	if (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)
		return -1;
	return sa->sa_family;
}

NINA_END_NAMESPACE_DECL
