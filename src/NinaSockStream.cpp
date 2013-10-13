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
 * @file NinaSockStream.cpp
 * @brief Implementation of stream bytes based socket operations
 * @author Jonathan Calmels
 * @date Sat Jun 11 2011
 */

#include "NinaSockStream.hpp"

NINA_BEGIN_NAMESPACE_DECL

SockStream::SockStream()
{
}

SockStream::~SockStream()
{
}

SockStream::SockStream(SockStream const& sap)
	: SockIO(sap)
{
}

SockStream&
SockStream::operator=(SockStream const& sap)
{
	if (this != &sap)
		SockIO::operator=(sap);
	return *this;
}

int
SockStream::urgentSend(void const* buf, size_t bufLen, uint8_t flag) const
{
	int errCode;

	errCode = OS::send(mTransportEndpoint, buf, bufLen, MSG_OOB | flag);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

int
SockStream::urgentReceive(void* buf, size_t bufLen, uint8_t flags) const
{
	int errCode;

	errCode = OS::recv(mTransportEndpoint, buf, bufLen, MSG_OOB | flags);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

int
SockStream::urgentReceive(std::string& str, uint8_t flags) const
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
	errCode = urgentReceive(data, capacity, flags);
	if (errCode == -1)
		return -1;
	data[errCode] = '\0';
	str.assign(data, errCode);
	delete[] data;
	return errCode;
}

int
SockStream::exactSend(void const* buf, size_t bufLen, uint8_t flag) const
{
	size_t		bytesTransferred;
	int			n;

	for (bytesTransferred = 0; bytesTransferred < bufLen;
		   	bytesTransferred += n) {
		n = OS::send(mTransportEndpoint, static_cast<char const*> (buf) + bytesTransferred,
					   	bufLen - bytesTransferred, flag);
		if (n == NINA_ENDPOINT_ERROR) {
			OS::setErrnoToWSALastError();
			return -1;
		}
		else if (n == 0)
			break;
	}
	return bytesTransferred;
}

int
SockStream::gatherWrite(IOContainer& ioc) const
{
	int errCode;

	errCode = OS::gatherWrite(mTransportEndpoint, ioc, 0);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

int
SockStream::scatterRead(IOContainer& ioc, uint8_t flags) const
{
	unsigned long	mode = flags;
	int				errCode;

	errCode = OS::scatterRead(mTransportEndpoint, ioc, &mode);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return errCode;
}

NINA_END_NAMESPACE_DECL
