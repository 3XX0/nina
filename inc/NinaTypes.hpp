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
 * @file NinaTypes.hpp
 * @brief Defines global types used by Nina framework
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#ifndef __NINA_TYPES_HPP__
# define __NINA_TYPES_HPP__

# include "NinaDef.hpp"
# include "NinaTypesPosix.hpp"
# include "NinaTypesWin32.hpp"

NINA_BEGIN_NAMESPACE_DECL
 NINA_BEGIN_NAMESPACE_ERROR

//! Defines the buffer to store error strings
typedef char StrerrBuf[1024];

 NINA_END_NAMESPACE_ERROR
NINA_END_NAMESPACE_DECL

#endif // !__NINA_TYPES_HPP__
