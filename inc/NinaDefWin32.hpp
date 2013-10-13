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
 * @file NinaDefWin32.hpp
 * @brief Nina specifics definitions for WIN32 plateforms
 * @author Jonathan Calmels
 * @date Fri Jul 22 2011
 */

#ifndef __NINA_DEFWIN32_HPP__
# define __NINA_DEFWIN32_HPP__

# if defined (_WIN32)

#  include <winerror.h>

//! @def NINA_WIN32
//! Defines Nina WIN32 subroutine
#  define NINA_WIN32
//! @def NINA_ENDPOINT_ERROR
//! Defines common networking errors
#  define NINA_ENDPOINT_ERROR SOCKET_ERROR
//! @def NINA_INVALID_HANDLE
//! Defines an error on an invalid handle
#  define NINA_INVALID_HANDLE INVALID_SOCKET
//! @def NINA_BAD_ARG
//! Defines the error code for a bad argument
#  define NINA_BAD_ARG ERROR_BAD_ARGUMENTS

/*!
 * @def NINA_DLLREQ
 * Defines the keyword required for dll processing<br/>
 * This define depends on compiler definitions
 */
/*!
 * @def NINA_EXTERN
 * Defines the keyword required to forward STL declarations<br/>
 * This define depends on compiler definitions
 */
#  if defined (NINA_DLLEXPORT)
#   define NINA_DLLREQ __declspec(dllexport)
#	define NINA_EXTERN
#  else
#   define NINA_DLLREQ __declspec(dllimport)
#   define NINA_EXTERN extern
#  endif // !NINA_DLLEXPORT

//! @name Defines used to shutdown a connection
//! @{
#  define NINA_SHUT_READ SD_RECEIVE
#  define NINA_SHUT_WRITE SD_SEND
#  define NINA_SHUT_ALL SD_BOTH
//! @}

# endif // !_WIN32

#endif // !__NINA_DEFWIN32_HPP__
