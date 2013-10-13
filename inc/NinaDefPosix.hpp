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
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHNINA_DEFAULT_POLICY// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

/*!
 * @file NinaDefPosix.hpp
 * @brief Nina specifics definitions for POSIX plateforms
 * @author Jonathan Calmels
 * @date Fri Jul 22 2011
 */

#ifndef __NINA_DEFPOSIX_HPP__
# define __NINA_DEFPOSIX_HPP__ 

# if defined (__unix__) || defined (__linux__)

#  include <errno.h>

//! @def NINA_POSIX
//! Defines Nina POSIX subroutine
#  define NINA_POSIX
//! @def NINA_ENDPOINT_ERROR
//! Defines common networking errors
#  define NINA_ENDPOINT_ERROR -1
//! @def NINA_INVALID_HANDLE
//! Defines an error on an invalid handle
#  define NINA_INVALID_HANDLE -1
//! @def NINA_DLLREQ
//! Defines the keyword required for dll processing<br/>
#  define NINA_DLLREQ
//! @def NINA_BAD_ARG
//! Defines the error code for a bad argument
#  define NINA_BAD_ARG EINVAL
/*!
 * @def NINA_EXTERN
 * Defines the keyword required to forward STL declarations<br/>
 * This define depends on compiler definitions
 */
#  if defined (NINA_DLLEXPORT)
#	define NINA_EXTERN
#  else
#   define NINA_EXTERN extern
#  endif // !NINA_DLLEXPORT

/*!
 * @def NINA_HAS_INET_REENTRANT
 * Defines whether or not inet reentrant functions are available<br/>
 * (ex. getservbyname_r)
 */
#  if !defined (__FreeBSD__)
#   define NINA_HAS_INET_REENTRANT
#  endif // !__FreeBSD__

//! @def NINA_LACK_OF_V4MAPPED
//! Defines whether or not the ipv4 mapping is supported (FreeBSD implementation ??)
#  if defined (__NetBSD__) || defined (__OpenBSD__) || defined (__FreeBSD__)
#   define NINA_LACK_OF_V4MAPPED
#  endif // !__NetBSD__ || __OpenBSD__

//! @def NINA_LACK_OF_ENTDATA
//! Defines whether or not `entdata` buffers are used into inet reentrant functions
//! @def NINA_HAS_EPOLL
//! Defines that the synchroneous event demultiplexer `epoll` is available on the OS
#  if defined (__linux__)
#	define NINA_LINUX
#   define NINA_LACK_OF_ENTDATA
#   define NINA_HAS_EPOLL
#  endif // !__linux__

//! @def NINA_HAS_KQUEUE
//! Defines that the synchroneous event demultiplexer `kqueue` is available on the OS
#  if defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) || defined (__bsdi__) || defined (__DragonFly__)
#	define NINA_BSD
#   define NINA_HAS_KQUEUE
#  endif // !__FreeBSD__ || __NetBSD__ || __OpenBSD__ || __bsdi__ || __DragonFly__

//! @def INFTIM
//! Defines INFTIM if not set
#  if !defined (INFTIM)
#   define INFTIM -1
#  endif // !INFTIM

//! @name Defines used to shutdown a connection
//! @{
#  define NINA_SHUT_READ SHUT_RD
#  define NINA_SHUT_WRITE SHUT_WR
#  define NINA_SHUT_ALL SHUT_RDWR
//! @}

# endif // !__unix__ || __linux__

#endif // !__NINA_DEFPOSIX_HPP__
