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
 * @file NinaDef.hpp
 * @brief Nina global definitions
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#ifndef __NINA_DEF_HPP__
# define __NINA_DEF_HPP__

# include <iostream>
# include <cstdlib>

# include "NinaDefPosix.hpp"
# include "NinaDefWin32.hpp"

//! @namespace NINA
//! Global namespace defining the boundary of Nina framework
# define NINA_BEGIN_NAMESPACE_DECL namespace NINA {
# define NINA_END_NAMESPACE_DECL }

//! @namespace NINA::Error
//! Sub Nina Namespace which handles exceptions
# define NINA_BEGIN_NAMESPACE_ERROR namespace Error {
# define NINA_END_NAMESPACE_ERROR }

//! @namespace NINA::OS
//! Sub Nina Namespace which handles operating systems specifications
# define NINA_BEGIN_NAMESPACE_OS namespace OS {
# define NINA_END_NAMESPACE_OS }

//! @def NINA_INLINE
//! Force inlining on .inl files
# define NINA_INLINE inline

//! @def NINA_UNUSED_ARG(x)
//! Used by Nina when arguments are not necessary
# define NINA_UNUSED_ARG(x) (void) (x)

//! @def NINA_ASSERT_SINGLETON(exp, name, where)
//! Singleton assertion, see below @see NinaCppUtils.hpp
# define	NINA_ASSERT_SINGLETON(exp, name, where) \
	if (exp) {}									\
	else {										\
    	std::cerr << "BAD SINGLETON ASSERTION "	\
		<< name	<< where << std::endl;			\
    	abort();								\
	}		 	

# include "NinaTypes.hpp" // version < MSVC++ 10.0

# if BYTE_ORDER == BIG_ENDIAN
#  define HTONLL(x) (x)
#  define NTOHLL(x) (x)
# else
#  define HTONLL(x) ((((uint64_t)(x) & 0xFFULL) << 56) | \
		(((uint64_t)(x) & 0xFF00ULL) << 40) | \
		(((uint64_t)(x) & 0xFF0000ULL) << 24) | \
		(((uint64_t)(x) & 0xFF000000ULL) << 8) | \
		(((uint64_t)(x) & 0xFF00000000ULL) >> 8) | \
		(((uint64_t)(x) & 0xFF0000000000ULL) >> 24) | \
		(((uint64_t)(x) & 0xFF000000000000ULL) >> 40) | \
		(((uint64_t)(x) & 0xFF00000000000000ULL) >> 56))
#  define NTOHLL(x) HTONLL(x)
# endif // !BYTE_ORDER == BIG_ENDIAN

//! Convert a long long integer between host and network byte order
uint64_t htonll(uint64_t hostLongLong);
//! Convert a long long integer between host and network byte order
uint64_t ntohll(uint64_t netLongLong);
# define htonll(x) HTONLL(x)
# define ntohll(x) NTOHLL(x)

#endif // !__NINA_DEF_HPP__
