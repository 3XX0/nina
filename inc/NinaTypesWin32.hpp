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
 * @file NinaTypesWin32.hpp
 * @brief Nina specifics types for WIN32 plateforms
 * @author Jonathan Calmels
 * @date Fri Jul 22 2011
 */

#ifndef __NINA_TYPESWIN32_HPP__
# define __NINA_TYPESWIN32_HPP__

# if defined (NINA_WIN32)

#  include <winsock2.h>
#  include <mswsock.h>

#  if (_MSC_VER < 1600)
typedef __int8				int8_t;
typedef unsigned __int8		uint8_t;
typedef __int16				int16_t;
typedef unsigned __int16	uint16_t;
typedef __int32				int32_t;
typedef unsigned __int32	uint32_t;
typedef __int64				int64_t;
typedef unsigned __int64	uint64_t;
#  else
#   include <stdint.h>
#  endif // !_MSC_VER < 1600

NINA_BEGIN_NAMESPACE_DECL

//! Nina networking handle
typedef SOCKET NINAHandle;

class SelectPolicy;
//! Default Reactor policy on Microsoft Windows operating systems
typedef SelectPolicy NINADefaultPolicy;

 NINA_BEGIN_NAMESPACE_OS

//! @name Types required to manipulate protocols and services buffers
//! @{
typedef char NINAProtoBuf[1024];
typedef char NINAServBuf[1024];
//! @}

//! Nina poll structure for event handling
typedef WSAPOLLFD NINAPollData;
//! Nina timeval structure for time storage
typedef struct
{
	time_t 	tv_sec;
	long 	tv_usec;
} NINATimeval;

 NINA_END_NAMESPACE_OS
NINA_END_NAMESPACE_DECL

# endif // !NINA_WIN32

#endif // !__NINA_TYPESWIN32_HPP__
