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
 * @file NinaOS.inl
 * @brief Implements operating systems specific mechanisms (inline functions)
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#if defined (NINA_POSIX)
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <unistd.h>
#endif // !NINA_POSIX

#include <cstdio>
#include <cerrno>

NINA_BEGIN_NAMESPACE_DECL 
 NINA_BEGIN_NAMESPACE_OS

NINA_INLINE void
setLastError(int errCode)
{
#if defined (NINA_WIN32)
	::SetLastError(errCode);
#endif // !NINA_WIN32
	errno = errCode;	
}

NINA_INLINE int
getLastError()
{
#if defined (NINA_WIN32)
	int		lError = ::GetLastError();
	int		lErrno = errno;

	return lErrno == 0 ? lError : lErrno;
#else
	return errno;
#endif // !NINA_WIN32
}

NINA_INLINE int
setErrnoToWSALastError()
{
#if defined (NINA_WIN32)
	return errno = ::WSAGetLastError();
#else
	return errno;
#endif // !NINA_WIN32
}

NINA_INLINE int
setErrnoToLastError()
{
#if defined (NINA_WIN32)
	return errno = ::GetLastError();
#else
	return errno;
#endif // !NINA_WIN32	
} 

NINA_INLINE int
IOCtl(NINAHandle handle, int cmd, void* args)
{
#if defined (NINA_WIN32)
	return ::ioctlsocket(handle, cmd, reinterpret_cast<unsigned long*> (args));
#else
	return ::ioctl(handle, cmd, args);
#endif // !NINA_WIN32
}

NINA_INLINE int
sockClose(NINAHandle sock)
{
#if defined (NINA_WIN32)
	return ::closesocket(sock);
#else
	return ::close(sock);
#endif // !NINA_WIN32
}

NINA_INLINE int
setSockOpt(NINAHandle sock, int level, int opt,
	   		void const* buf, int bufLen)
{
#if defined (NINA_WIN32)
	return ::setsockopt(sock, level, opt, reinterpret_cast<char const*> (buf), bufLen);
#else
	return ::setsockopt(sock, level, opt, buf, bufLen);
#endif // !NINA_WIN32
}

NINA_INLINE int
getSockOpt(NINAHandle sock, int level, int opt,
	   		void* buf, int* bufLen)
{
#if defined (NINA_WIN32)
	return ::getsockopt(sock, level, opt, reinterpret_cast<char*> (buf), bufLen);
#else
	return ::getsockopt(sock, level, opt, buf, reinterpret_cast<socklen_t*> (bufLen));
#endif // !NINA_WIN32
}

NINA_INLINE int
getPeerName(NINAHandle sock, sockaddr* addr, int* addrLen)
{
#if defined (NINA_WIN32)
	return ::getpeername(sock, addr, addrLen);
#else
	return ::getpeername(sock, addr, reinterpret_cast<socklen_t*> (addrLen));
#endif // !NINA_WIN32
}

NINA_INLINE int
getSockName(NINAHandle sock, sockaddr* addr, int* addrLen)
{ 
#if defined (NINA_WIN32)
	return ::getsockname(sock, addr, addrLen); 
#else
	return ::getsockname(sock, addr, reinterpret_cast<socklen_t*> (addrLen));
#endif // !NINA_WIN32
}   

NINA_INLINE int
recv(NINAHandle sock, void* buf, size_t bufLen, int flags)
{ 
#if defined (NINA_WIN32)
	return ::recv(sock, static_cast<char*> (buf), bufLen, flags);
#else
	return ::recv(sock, buf, bufLen, flags);	
#endif // !NINA_WIN32
}

NINA_INLINE int
send(NINAHandle sock, void const* buf, size_t bufLen, int flags)
{
#if defined (NINA_WIN32)
	return ::send(sock, static_cast<char const*> (buf), bufLen, flags);
#else
	return ::send(sock, buf, bufLen, flags);	
#endif // !NINA_WIN32
}

NINA_INLINE long
getOpenMax()
{
#if defined (NINA_WIN32)
	return ::_getmaxstdio();
#else
	return ::sysconf(_SC_OPEN_MAX);
#endif // !NINA_WIN32
}

NINA_INLINE int
accept(NINAHandle sock, sockaddr* addr, int* addrLen)
{
#if defined (NINA_WIN32)
	return ::accept(sock, addr, addrLen);
#else
	return ::accept(sock, addr, reinterpret_cast<socklen_t*> (addrLen));
#endif // !NINA_WIN32
}

NINA_INLINE int
connect(NINAHandle sock, sockaddr const* addr, int addrLen)
{
#if defined (NINA_WIN32)
	return ::connect(sock, addr, addrLen);
#else
	return ::connect(sock, addr, static_cast<socklen_t> (addrLen));
#endif // !NINA_WIN32
}

NINA_INLINE int
recvFrom(NINAHandle sock, void* buf, size_t bufLen, int flags, sockaddr* addr, int* addrLen)
{
#if defined (NINA_WIN32)
	return ::recvfrom(sock, static_cast<char*> (buf), bufLen, flags, addr, addrLen);
#else
	return ::recvfrom(sock, buf, bufLen, flags, addr, reinterpret_cast<socklen_t*> (addrLen));
#endif // !NINA_WIN32
}

NINA_INLINE int
sendTo(NINAHandle sock, void const* buf, size_t bufLen, int flags, sockaddr const* addr, int addrLen)
{
#if defined (NINA_WIN32)
	return ::sendto(sock, static_cast<char const*> (buf), bufLen, flags, addr, addrLen);
#else
	return ::sendto(sock, buf, bufLen, flags, addr, static_cast<socklen_t> (addrLen));
#endif // !NINA_WIN32
}

 NINA_END_NAMESPACE_OS
NINA_END_NAMESPACE_DECL
