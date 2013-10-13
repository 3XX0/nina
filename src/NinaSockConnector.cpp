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
 * @file NinaSockConnector.cpp
 * @brief Implements a connection wrapper
 * @author Jonathan Calmels
 * @date Tue Aug 30 2011
 */

#include "NinaDef.hpp"
#include "NinaOS.hpp"
#include "NinaSockConnector.hpp"

NINA_BEGIN_NAMESPACE_DECL

SockConnector::SockConnector()
{
}

SockConnector::~SockConnector()
{
}

SockConnector::SockConnector(SockConnector const&)
{
}

SockConnector&
SockConnector::operator=(SockConnector const&)
{
	return *this;
}

int
SockConnector::connect(SockIO const& sock, Addr const& addr) const
{
	NINAHandle	handle;	

	handle = sock.getHandle();
	if (handle == NINA_INVALID_HANDLE) {
		OS::setLastError(NINA_BAD_ARG);
		return -1;
	}
	// We admit that getters can't failed, the worst case leading to connection failure
	if (OS::connect(handle, static_cast<sockaddr const*> (addr.getAddr()), addr.getSize()) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return 0;
}

int
SockConnector::disconnect(SockDatagram const& sock) const
{
	InetAddr unspecAddr(AF_UNSPEC);

	if (connect(sock, unspecAddr) < 0 && errno != EAFNOSUPPORT)
		return -1;
	return 0;
}

int
SockConnector::disconnect(SockStream const& sock, Process how) const
{
	NINAHandle	handle;	

	handle = sock.getHandle();
	if (handle == NINA_INVALID_HANDLE) {
		OS::setLastError(NINA_BAD_ARG);
		return -1;
	}
	if (::shutdown(handle, how) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return 0;
}

int
SockConnector::abort(SockStream& sock) const
{
	linger	closeOptions;

	closeOptions.l_onoff = 1;
	closeOptions.l_linger = 0;
	if (sock.setOption(SOL_SOCKET, SO_LINGER, &closeOptions, sizeof(linger)) < 0)
		return -1;
	return sock.close();
}

int
SockConnector::complete(SockStream& sock, Time const* timeout) const
{
	NINAHandle	handle;
	int			errCode;
	fd_set		writeSet;
	int			opt = 0;
	int			optLen = sizeof opt;
	bool		hasFailed = false;
#if defined (NINA_WIN32)
	fd_set		exceptSet;
	bool		needChecking = false;
#else
	bool		needChecking = true;
#endif // !NINA_WIN32

	handle = sock.getHandle();
	FD_ZERO(&writeSet);
	FD_SET(handle, &writeSet);
#if defined (NINA_WIN32)
	FD_ZERO(&exceptSet);
	FD_SET(handle, &exceptSet);
	errCode = OS::select(handle + 1, 0, &writeSet, &exceptSet, timeout);
#else
	errCode = OS::select(handle + 1, 0, &writeSet, 0, timeout);
#endif // !NINA_WIN32
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	if (errCode == 0)
		return 0;
#if defined (NINA_WIN32)
	if (FD_ISSET(handle, &exceptSet) != 0) {
		needChecking = true;
		hasFailed = true;
	}
#endif // !NINA_WIN32
	if (needChecking == true) {
		if (sock.getOption(SOL_SOCKET, SO_ERROR, reinterpret_cast<char*> (&opt), &optLen) < 0)
			return -1;
		if (opt != 0 || hasFailed == true) {
			errno = opt;
			return -1;
		}
	}
	return 1;
}

NINA_END_NAMESPACE_DECL
