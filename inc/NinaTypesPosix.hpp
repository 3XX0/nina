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
 * @file NinaTypesPosix.hpp
 * @brief Nina specifics types for POSIX plateforms
 * @author Jonathan Calmels
 * @date Fri Jul 22 2011
 */

#ifndef __NINA_TYPESPOSIX_HPP__
# define __NINA_TYPESPOSIX_HPP__

# if defined (NINA_POSIX)

#  include <netdb.h>
#  include <poll.h>
#  include <stdint.h>

NINA_BEGIN_NAMESPACE_DECL

//! Nina networking handle
typedef int NINAHandle;

#  if defined (__linux__)
class EpollPolicy;
//! Default Reactor policy on GNU/Linux operating systems
typedef EpollPolicy NINADefaultPolicy;
#  endif // !__linux__
#  if defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) || defined (__bsdi__) || defined (__DragonFly__)
class KqueuePolicy;
//! Default Reactor policy on BSD operating systems
typedef KqueuePolicy NINADefaultPolicy;
#  endif // !__FreeBSD__ || __NetBSD__ || __OpenBSD__ || __bsdi__ || __DragonFly__

 NINA_BEGIN_NAMESPACE_OS

//! @name Types required to manipulate protocols and services buffers
//! @{
#  if defined (NINA_HAS_INET_REENTRANT) && !defined (NINA_LACK_OF_ENTDATA)
typedef protoent_data NINAProtoBuf;
typedef servent_data NINAServBuf;
#  else
typedef char NINAProtoBuf[1024];
typedef char NINAServBuf[1024];
#  endif // !NINA_HAS_INET_REENTRANT && !NINA_LACK_OF_ENTDATA
//! @}

//! Nina poll structure for event handling
typedef pollfd NINAPollData;
//! Nina timeval structure for time storage
typedef timeval NINATimeval;

 NINA_END_NAMESPACE_OS
NINA_END_NAMESPACE_DECL

# endif // !NINA_POSIX

#endif // !__NINA_TYPESPOSIX_HPP__
