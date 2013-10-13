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
 * @file NinaSAP.cpp
 * @brief Implements the service access point
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#include "NinaDef.hpp"

#if defined (NINA_POSIX)
# include <unistd.h>
# include <fcntl.h>
#endif // !NINA_POSIX
#if defined (NINA_WIN32)
# include <winsock2.h>
#endif // !NINA_WIN32

#include "NinaSAP.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

SAP::SAP()
	: mTransportEndpoint(NINA_INVALID_HANDLE),
	mIOFlags(-1),
	mClFlag(-1)
{
}

SAP::SAP(SAP const& sap)
{
		mTransportEndpoint = sap.mTransportEndpoint;
		mIOFlags = sap.mIOFlags;
		mClFlag = sap.mClFlag;	
}

SAP&
SAP::operator=(SAP const& sap)
{
	if (this != &sap) {
		mTransportEndpoint = sap.mTransportEndpoint;
		mIOFlags = sap.mIOFlags;
		mClFlag = sap.mClFlag;
	}
	return *this;
}

int
SAP::enable(uint8_t opt)
{
#if defined (NINA_WIN32)
	if (opt & NON_BLOCK) {
		unsigned long	on = 1;

		if (OS::IOCtl(mTransportEndpoint, FIONBIO, &on) == NINA_ENDPOINT_ERROR) {
			OS::setErrnoToWSALastError();
			return -1;
	 	}
	} 
#else
	if ((opt & NON_BLOCK) || (opt & SIGNAL_DRIVEN)) {
		if (mIOFlags < 0 && (mIOFlags = ::fcntl(mTransportEndpoint, F_GETFL, 0)) < 0)
			return -1;
		if (opt & NON_BLOCK)
			mIOFlags |= O_NONBLOCK;
		if (opt & SIGNAL_DRIVEN)
			mIOFlags |= O_ASYNC;
		if (::fcntl(mTransportEndpoint, F_SETFL, mIOFlags) < 0)
			return -1;
	}
	if (opt & CLOSE_ON_EXEC) {
		if (mClFlag < 0 && (mClFlag = ::fcntl(mTransportEndpoint, F_GETFD, 0)) < 0)
			return -1;
		mClFlag |= FD_CLOEXEC;
		if (::fcntl(mTransportEndpoint, F_SETFD, mIOFlags) < 0)
			return -1;
	 }
	if ((opt & OWNERSHIP) && ::fcntl(mTransportEndpoint, F_SETOWN, ::getpid()) < 0)
			return -1;
#endif // !NINA_WIN32
	return 0;
}

int
SAP::disable(uint8_t opt)
{
#if defined (NINA_WIN32)
	if (opt & NON_BLOCK) {
		unsigned long	off = 0;

		if (OS::IOCtl(mTransportEndpoint, FIONBIO, &off) == NINA_ENDPOINT_ERROR) {
			OS::setErrnoToWSALastError();
			return -1;
		}	
	}
#else
	if ((opt & NON_BLOCK) || (opt & SIGNAL_DRIVEN)) {
		if (mIOFlags < 0 && (mIOFlags = ::fcntl(mTransportEndpoint, F_GETFL, 0)) < 0)
			return -1;
		if (opt & NON_BLOCK)
			mIOFlags &= ~O_NONBLOCK;
		if (opt & SIGNAL_DRIVEN)
			mIOFlags &= ~O_ASYNC;
		if (::fcntl(mTransportEndpoint, F_SETFL, mIOFlags) < 0)
			return -1;
	}
	if (opt & CLOSE_ON_EXEC) {
		if (mClFlag < 0 && (mClFlag = ::fcntl(mTransportEndpoint, F_GETFD, 0)) < 0)
			return -1;
		mClFlag &= ~FD_CLOEXEC;
		if (::fcntl(mTransportEndpoint, F_SETFD, mIOFlags) < 0)
			return -1;
	}
	if ((opt & OWNERSHIP) && ::fcntl(mTransportEndpoint, F_SETOWN, 0) < 0)
			return -1;
#endif // !NINA_WIN32
	return 0;
}

int
SAP::control(int cmd, void* args)
{
	if (OS::IOCtl(mTransportEndpoint, cmd, args) == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
		return -1;
	}
	return 0;
} 

NINA_END_NAMESPACE_DECL
