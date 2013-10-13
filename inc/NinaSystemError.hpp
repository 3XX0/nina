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
 * @file NinaSystemError.hpp
 * @brief Nina defined exceptions
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#ifndef __NINA_SYSTEMERROR_HPP__
# define __NINA_SYSTEMERROR_HPP__

# include "NinaDef.hpp"

# if defined (NINA_WIN32)
// Disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#  pragma warning(disable: 4251)
# endif // !NINA_WIN32

# include "NinaTypes.hpp"

#if defined (NINA_POSIX)
# include <cstring>
#endif // !NINA_POSIX

# include <iostream>
# include <sstream>
# include <stdexcept>

NINA_BEGIN_NAMESPACE_DECL
 NINA_BEGIN_NAMESPACE_ERROR

/*! @class SystemError
 * @brief Handles operating systems common errors
 *
 * @details This class encapsulates the formatting errors in a portable manner
 */
class NINA_DLLREQ SystemError : public std::exception
{
	public:
		/*!
		 * @brief Constructor
		 * @details Constructor which build an error message from an error code
		 * @param[in] errCode : error code which had to be resolved
		 */
		SystemError(unsigned long errCode)
			: mErrorString(buildMsg(errCode))
		{}
		
		/*!
		 * @brief Constructor
		 * @details Constructor which build an error message from a string
		 * @param[in] err : error string which will appear on the exception raised
		 */
		SystemError(const char* err)
			: mErrorString(err)
		{}

		//! @brief Destructor
		~SystemError() throw()
		{}

	public:
		//! @brief Return the current error string
		//! @return An error string describing the exception raised
		const char* what() const throw()
		{
			return mErrorString.c_str();
		}
	private:
		/*!
		 * @brief Build an error message from an error code
		 * @param[in] errCode : error code which had to be resolved
		 * @return An error string corresponding to the errorCode
		 */
		std::string buildMsg(unsigned long errCode)
		{
			std::ostringstream	os;
#if defined (NINA_WIN32)
			LPTSTR				buf;

			if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					0, errCode, 0, reinterpret_cast<LPTSTR> (&buf), 0, 0) == 0)
					os << "Unknown error";
			else
				os << buf;
			::LocalFree(buf);
#else
			StrerrBuf			buf;

#if !defined __linux__ || (defined _POSIX_C_SOURCE >= 200112L || defined _XOPEN_SOURCE >= 600) && !defined _GNU_SOURCE
	::strerror_r(errCode, buf, sizeof buf);
	os << buf;
#else
	char* res = ::strerror_r(errCode, buf, sizeof buf);
	os << res;
#endif // !_POSIX_C_SOURCE || _XOPEN_SOURCE && _GNU_SOURCE
#endif // !NINA_WIN32
			return os.str();
		}

	private:
		std::string mErrorString; //!< Error output string
};

 NINA_END_NAMESPACE_ERROR
NINA_END_NAMESPACE_DECL

#endif // !__NINA_SYSTEMERROR_HPP__
