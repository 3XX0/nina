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
 * @file NinaOS.cpp
 * @brief Implements operating systems specific mechanisms
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#include <stdexcept>
#include <sstream>

#include "NinaDef.hpp"

#if defined (NINA_POSIX)
# include <sys/time.h>
# include <string.h>
# include <unistd.h>
#endif // !NINA_POSIX

#include "NinaIOContainer.hpp"
#include "NinaSystemError.hpp"

NINA_BEGIN_NAMESPACE_DECL

Init::Init(std::string const& WSAVersion)
{
#if defined (NINA_WIN32)
	try {
		if (startWSA(WSAVersion) < 0)
			throw std::invalid_argument("Invalid WSA version");
	}
	catch (...) {
		throw;
	}
#else
	NINA_UNUSED_ARG(WSAVersion);
#endif // !NINA_WIN32
	errno = 0;
}

Init::~Init()
{
#if defined (NINA_WIN32)
	::WSACleanup();
#endif // !NINA_WIN32
}

#if defined (NINA_WIN32)
int
Init::startWSA(std::string const& WSAVersion) const
{
	std::stringstream	ss;
	size_t				pos;
	uint16_t			hWord;
	uint16_t			lWord;
	WSADATA				WSAData;
	int					errCode;

	if ((pos = WSAVersion.find('.')) != std::string::npos) {
		ss << WSAVersion.substr(0, pos);
		ss >> lWord;
		ss.clear();
		ss << WSAVersion.substr(pos + 1);
		ss >> hWord;
		if ((errCode = ::WSAStartup(MAKEWORD(lWord, hWord), &WSAData)) == 0)
			return 0;
		throw Error::SystemError(errCode);
	}
	return -1;
}
#endif // !NINA_WIN32

 NINA_BEGIN_NAMESPACE_OS 

protoent*
getProtoByNumber(int protocol, NINAProtoBuf* pBuf)
{
	protoent	*pRes;
	protoent	*pEntry;

#if defined (NINA_HAS_INET_REENTRANT)
	pEntry = new protoent;
# if defined (NINA_LACK_OF_ENTDATA)
	::memset(*pBuf, 0, sizeof(NINAProtoBuf));	
	if (::getprotobynumber_r(protocol, pEntry, *pBuf,
			sizeof(NINAProtoBuf), &pRes) > 0) {
# else
	pRes = pEntry;
	::memset(pBuf, 0, sizeof(NINAProtoBuf));
	if (::getprotobynumber_r(protocol, pEntry, pBuf) < 0) {
# endif // !NINA_LACK_OF_ENTDATA
		delete pEntry;
		return 0;
	}
#else
	NINA_UNUSED_ARG(pBuf);
	pRes = new protoent;
	pEntry = ::getprotobynumber(protocol);
	::memcpy(pRes, pEntry, sizeof(protoent));
#endif // !NINA_HAS_INET_REENTRANT
	return pRes;
}

protoent*
getProtoByName(char const* name, NINAProtoBuf* pBuf)
{
	protoent			*pRes;
	protoent			*pEntry;

#if defined (NINA_HAS_INET_REENTRANT)
	pEntry = new protoent;
# if defined (NINA_LACK_OF_ENTDATA)
	::memset(*pBuf, 0, sizeof(NINAProtoBuf));
	if (::getprotobyname_r(name, pEntry, *pBuf,
				sizeof(NINAProtoBuf), &pRes) > 0) {
# else
	pRes = pEntry;
	::memset(pBuf, 0, sizeof(NINAProtoBuf));
	if (::getprotobyname_r(name, pEntry, pBuf) < 0) {
# endif // !NINA_LACK_OF_ENTDATA
		delete pEntry;
		return 0;
	}
#else
	NINA_UNUSED_ARG(pBuf);
	pRes = new protoent;
	pEntry = ::getprotobyname(name);
	::memcpy(pRes, pEntry, sizeof(protoent));
#endif // !NINA_HAS_INET_REENTRANT
	return pRes;
}

servent*
getServByPort(int port, char const* protocol, NINAServBuf* sBuf)
{ 
	servent			*sRes;
	servent			*sEntry;

#if defined (NINA_HAS_INET_REENTRANT)
	sEntry = new servent;
# if defined (NINA_LACK_OF_ENTDATA)
	::memset(*sBuf, 0, sizeof(NINAServBuf));
	if (::getservbyport_r(port, protocol, sEntry, *sBuf,
				sizeof(NINAServBuf), &sRes) > 0) {
# else
	sRes = sEntry;
	::memset(sBuf, 0, sizeof(NINAServBuf));
	if (::getservbyport_r(port, protocol, sEntry, sBuf) < 0) {
# endif // !NINA_LACK_OF_ENTDATA
		delete sEntry;
		return 0;
	}
#else
	NINA_UNUSED_ARG(sBuf);
	sRes = new servent;
	sEntry = ::getservbyport(port, protocol);
	::memcpy(sRes, sEntry, sizeof(servent));
#endif // !NINA_HAS_INET_REENTRANT
	return sRes;
}

servent*
getServByName(char const* name, char const* protocol, NINAServBuf* sBuf)
{
	servent			*sRes;
	servent			*sEntry;

#if defined (NINA_HAS_INET_REENTRANT)
	sEntry = new servent;
# if defined (NINA_LACK_OF_ENTDATA)
	::memset(*sBuf, 0, sizeof(NINAServBuf));
	if (::getservbyname_r(name, protocol, sEntry, *sBuf,
				sizeof(NINAServBuf), &sRes) > 0) {
# else
	sRes = sEntry;
	::memset(sBuf, 0, sizeof(NINAServBuf));
	if (::getservbyname_r(name, protocol, sEntry, sBuf) < 0) {
# endif // !NINA_LACK_OF_ENTDATA
		delete sEntry;
		return 0;
	}
#else
	NINA_UNUSED_ARG(sBuf);
	sRes = new servent;
	sEntry = ::getservbyname(name, protocol);
	::memcpy(sRes, sEntry, sizeof(servent));
#endif // !NINA_HAS_INET_REENTRANT
	return sRes;
}

int
scatterRead(NINAHandle sock, IOContainer& ioc, unsigned long* flags)
{
	int		errCode;

#if defined (NINA_WIN32)
	errCode = ::WSARecv(sock, ioc(), ioc.getSize(), 0, flags, 0, 0);
#else
	NINA_UNUSED_ARG(flags);
	errCode = ::readv(sock, ioc(), ioc.getSize());
#endif // !NINA_WIN32
	ioc.cleanup();
	return errCode;	
}

int
gatherWrite(NINAHandle sock, IOContainer& ioc, unsigned long flags)
{
	int errCode;

#if defined (NINA_WIN32)
	errCode = ::WSASend(sock, ioc(), ioc.getSize(), 0, flags, 0, 0);
#else
	NINA_UNUSED_ARG(flags);
	errCode = ::writev(sock, ioc(), ioc.getSize());
#endif // !NINA_WIN32
	ioc.cleanup();
	return errCode;
}

int
select(int fdLen, fd_set* readFds, fd_set* writeFds, fd_set* exceptFds, Time const* timeout)
{
	int			errCode;
	Time*		nonConstTimeout = const_cast<Time*> (timeout);
	Time		savedTime = Time::timeNull;

	if (timeout == 0)
		return ::select(fdLen, readFds, writeFds, exceptFds, 0);
	savedTime = *timeout;
#if defined (NINA_WIN32)
	errCode = ::select(fdLen, readFds, writeFds, exceptFds, timeout->getTimeval());
#else
	timeval* tv = const_cast<timeval*> (timeout->getTimeval());
	errCode = ::select(fdLen, readFds, writeFds, exceptFds, tv);
#endif // !NINA_WIN32
	*nonConstTimeout = savedTime;
	return errCode;
}

int
poll(NINAPollData* fdSet, unsigned long fdSetLen, Time const* timeout)
{
	time_t	time;

	if (timeout != 0) {
		time = timeout->getSeconds() * Time::SEC_IN_MSEC;
		time += timeout->getUSeconds() / Time::MSEC_IN_USEC;
	}
#if defined (NINA_WIN32)
	else
		return ::WSAPoll(fdSet, fdSetLen, -1);
	return ::WSAPoll(fdSet, fdSetLen, static_cast<int> (time));
#else
	else
		return ::poll(fdSet, fdSetLen, INFTIM);
	return ::poll(fdSet, fdSetLen, time);
#endif // !NINA_WIN32
}

int
getCurrentTime(NINATimeval* tv)
{
	int				errCode = 0;
#if defined (NINA_WIN32)
	ULARGE_INTEGER	ul;
	FILETIME		ft;

	GetSystemTimeAsFileTime(&ft);
	ul.LowPart = ft.dwLowDateTime;
	ul.HighPart = ft.dwHighDateTime;
	ul.QuadPart /= 10ULL; // to microseconds
	ul.QuadPart -= 11644473600000000ULL; // UNIX Epoch delta
	tv->tv_usec = static_cast<long> (ul.QuadPart % 1000000LL); // microseconds
	tv->tv_sec = static_cast<time_t> (ul.QuadPart / 1000000LL); // seconds
#else
	timespec			ts;

	errCode = clock_gettime(CLOCK_REALTIME, &ts);
	if (errCode == 0) {
		tv->tv_usec = ts.tv_nsec / 1000;
		tv->tv_sec = ts.tv_sec + tv->tv_usec / 1000000;
		tv->tv_usec %= 1000000;
	}
#endif // !NINA_WIN32
	return errCode;
}

 NINA_END_NAMESPACE_OS
NINA_END_NAMESPACE_DECL
