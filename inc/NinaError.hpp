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
 * @file NinaError.hpp
 * @brief Defines wrapper functions to retrieve errors across operating systems
 * @author Jonathan Calmels
 * @date Sun Aug 28 2011
 */

#ifndef __NINA_ERROR_HPP__
# define __NINA_ERROR_HPP__

# include <string>
# include "NinaDef.hpp"

NINA_BEGIN_NAMESPACE_DECL
 NINA_BEGIN_NAMESPACE_ERROR

//! @brief Retrieve the last error encountered, using the global variable errno
//! @return a string containing the error message
NINA_DLLREQ std::string getLastError();
//! @brief Print the last error encountered, using the global variable errno
//! @param[in] context : message to display before the error
NINA_DLLREQ void printLastError(std::string const& context = "");
//! @brief Throw the last error encountered, using the global variable errno
//! @throw NINA::Error::SystemError
NINA_DLLREQ void throwLastError();
//! @brief Check whether there is a pending error or not
//! @return true if an error occurred, false otherwise
NINA_DLLREQ bool pendingError();

 NINA_END_NAMESPACE_ERROR
NINA_END_NAMESPACE_DECL

# include "NinaError.inl"

#endif /* !__NINA_ERROR_HPP__ */
