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
 * @file NinaSockAcceptor.cpp
 * @brief Implements a wrapper to a passive socket
 * @author Jonathan Calmels
 * @date Tue Aug 23 2011
 */

#include "NinaSockAcceptor.hpp"
#include "NinaSystemError.hpp"

NINA_BEGIN_NAMESPACE_DECL

SockAcceptor::SockAcceptor()
	: Sock()
{
}

SockAcceptor::~SockAcceptor()
{
}

SockAcceptor::SockAcceptor(SockAcceptor const& sap)
	: Sock(sap)
{
}

SockAcceptor&
SockAcceptor::operator=(SockAcceptor const& sap)
{
	if (this != &sap)
		Sock::operator=(sap);
	return *this;
}

int
SockAcceptor::open(Addr const& addr, int family, int protocol, bool reuseAddr, int backlog)
{
	if (family == PF_UNSPEC)
		family = addr.getFamily();
	if (Sock::open(addr, family, protocol, SOCK_STREAM, reuseAddr) < 0)
		return -1;
	if (::listen(mTransportEndpoint, backlog) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		close();
		return -1;
	}
	return 0;
}

void
SockAcceptor::open(Addr const& addr, int family, std::string const& protocol, bool reuseAddr, int backlog)
{
	if (family == PF_UNSPEC)
		family = addr.getFamily();
	try {
		Sock::open(addr, family, protocol, SOCK_STREAM, reuseAddr);
	}
	catch (...) {
		throw;
	}
	if (::listen(mTransportEndpoint, backlog) == NINA_ENDPOINT_ERROR) {
		close();
		throw Error::SystemError(OS::setErrnoToWSALastError());
	}
}

int
SockAcceptor::accept(SockStream& sock, Addr* peerAddr)
{
	int					s;
	sockaddr_storage	addr;
	int					size = sizeof addr;

	if (peerAddr != 0)
		s = OS::accept(mTransportEndpoint, reinterpret_cast<sockaddr*> (&addr), &size);
	else
		s = OS::accept(mTransportEndpoint, 0, 0);
	if (s == NINA_INVALID_HANDLE) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	// We assume that the address returned is always correct
	if (peerAddr != 0)
		peerAddr->setAddr(&addr, size);
	sock.setHandle(s);
	sock.disable(NON_BLOCK);
	return 0;
}

NINA_END_NAMESPACE_DECL
