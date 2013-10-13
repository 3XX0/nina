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
 * @file NinaSockIO.cpp
 * @brief Implementation of common socket Input/Output operations
 * @author Jonathan Calmels
 * @date Fri Jun 10 2011
 */

#include "NinaSockIO.hpp"

NINA_BEGIN_NAMESPACE_DECL

SockIO::SockIO()
	: Sock()
{
}

SockIO::SockIO(SockIO const& sap)
	: Sock(sap)
{
}

SockIO&
SockIO::operator=(SockIO const& sap)
{
	if (this != &sap)
		Sock::operator=(sap);
	return *this;
}

int
SockIO::receive(void* buf, size_t bufLen, uint8_t flags) const
{
	int	errCode;

	errCode = OS::recv(mTransportEndpoint, buf, bufLen, flags);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

int
SockIO::receive(std::string& str, uint8_t flags) const
{
	char*	data;
	int		capacity;
	int		errCode;

	capacity = str.capacity();
	if (capacity == 0) {
		OS::setLastError(NINA_BAD_ARG);
		return -1;
	}
	data = new char[capacity + 1];
	errCode = receive(data, capacity, flags);
	if (errCode == -1)
		return -1;
	data[errCode] = '\0';
	str.assign(data, errCode);
	delete[] data;
	return errCode;
}

int
SockIO::send(void const* buf, size_t bufLen, uint8_t flag) const
{
	int errCode;

	errCode = OS::send(mTransportEndpoint, buf, bufLen, flag);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

NINA_END_NAMESPACE_DECL
