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
 * @file NinaError.cpp
 * @brief Implements wrapper functions to retrieve errors across operating systems
 * @author Jonathan Calmels
 * @date Sun Aug 28 2011
 */

#include "NinaDef.hpp"
#include "NinaError.hpp"

#if defined (NINA_POSIX)
# include <cstring>
#endif // !NINA_POSIX

#include <iostream>
#include <sstream>
#include <cerrno>

NINA_BEGIN_NAMESPACE_DECL
 NINA_BEGIN_NAMESPACE_ERROR

std::string
getLastError()
{
	std::ostringstream	os;
#if defined (NINA_WIN32)
	LPTSTR				buf;

	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				0, errno, 0, reinterpret_cast<LPTSTR> (&buf), 0, 0) == 0)
		os << "Unknown error";
	else
		os << buf;
	::LocalFree(buf);
#else
	StrerrBuf			buf;

#if !defined __linux__ || (defined _POSIX_C_SOURCE >= 200112L || defined _XOPEN_SOURCE >= 600) && !defined _GNU_SOURCE
	::strerror_r(errno, buf, sizeof buf);
	os << buf;
#else
	char* res = ::strerror_r(errno, buf, sizeof buf);
	os << res;
#endif // !_POSIX_C_SOURCE || _XOPEN_SOURCE && _GNU_SOURCE
#endif // !NINA_WIN32
	return os.str();
}

 NINA_END_NAMESPACE_ERROR
NINA_END_NAMESPACE_DECL
