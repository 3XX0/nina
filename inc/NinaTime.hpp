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
 * @file NinaTime.hpp
 * @brief Class that encapsulates time specifications
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#ifndef __NINA_TIME_HPP__
# define __NINA_TIME_HPP__

# include "NinaDef.hpp"

# if defined (NINA_POSIX)
#  include <sys/time.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
# endif // !NINA_WIN32

# include <ctime>
# include <cstring>

NINA_BEGIN_NAMESPACE_DECL

// Forward declaration
 NINA_BEGIN_NAMESPACE_OS
int getCurrentTime(NINATimeval* tv);
 NINA_END_NAMESPACE_OS

/*! @class Time
 * @brief Encapsulates time specifications
 *
 * @details This class wrapped time representations in a partable manner
 */
class NINA_DLLREQ Time
{
	public:
		enum
		{
			SEC_IN_USEC = 1000000, //!< One second in microseconds
			SEC_IN_MSEC = 1000, //!< One second in milliseconds
			MSEC_IN_USEC = 1000, //!< One millisecond in microseconds
			USEC_IN_NSEC = 1000 //!< One microsecond in nanoseconds
		};

	public:
		/*!
		 * @brief Constructor
		 * @param[in] sec : number of seconds
		 * @param[in] uSec : number of microseconds
		 */
		Time(time_t sec, long uSec);
		//! @brief Destructor
		~Time();
		//! @brief Copy constructor
		Time(Time const& time);
		//! @brief Assignement operator
		Time& operator=(Time const& time);

	public:
		//! @brief Get the underlying time representation
		//! @return A pointer on a timeval structure
		timeval const* getTimeval() const;
#if defined (NINA_POSIX)
		//! @brief Get the underlying time representation
		//! @return A pointer on a timespec structure
		timespec const* getTimespec() const;
#endif // !NINA_POSIX
		//! @brief Check for time equality	
		bool operator==(Time const& time) const;
		//! @brief Check for time inequality
		bool operator!=(Time const& time) const;
		//! @brief Check for time inferiority
		bool operator<(Time const& time) const;
		//! @brief Check for time superiority
		bool operator>(Time const& time) const;
		//! @brief Check for time inferiority or equality
		bool operator<=(Time const& time) const;
		//! @brief Check for time superiority or equality
		bool operator>=(Time const& time) const;
		//! @brief Add a time value to the current time
		Time& operator+=(Time const& time);
		//! @brief Substract a time value to the current time
		Time& operator-=(Time const& time);
		//! @brief Multiply a factor to the current time
		//! @param factor : factor of the multiplication
		Time& operator*=(double factor);
		//! @brief Add some time values
		Time operator+(Time const& time);
		//! @brief Substract some time values
		Time operator-(Time const& time);
		//! @brief Multiply a factor to a time value
		//! @param factor : factor of the multiplication
		Time operator*(double factor);
		//! @brief Increment the current time
		//! @details The time is incremented with one second and one microsecond
		void operator++();
		//! @brief Decrement the current time
		//! @details The time is decremented with one second and one microsecond
		void operator--();
		/*!
		 * @brief Set the time with user defined values
		 * @param[in] sec : number of seconds
		 * @param[in] uSec : number of microseconds
		 */
		void set(time_t sec, long uSec);
		//! @brief Get the seconds used
		//! @return Number of seconds used
		time_t getSeconds() const;
		//! @brief Get the microseconds used
		//! @return Number of microseconds used
		long getUSeconds() const;
		/*!
		 * @brief Get the current timestamp
		 * @details This is actually the number of seconds and micro-seconds since January the 1st 1970
		 * unlike the Microsoft Windows epoch standard (January the 1st 1601)
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int timestamp();
	private:
		//! @brief Normalize the time by balancing seconds and microseconds
		void normalize();

	public:
		static Time const timeNull; //!< Represents a time null
	private:
		OS::NINATimeval		mTimeValue; //!< Concrete time storage
# if defined (NINA_WIN32)
		mutable timeval		mExportTimeValue; //!< Transition container for time storage (Miscrosoft Windows)
# else
		mutable timespec	mTimeSpecValue; //!< Alternative concrete time storage
# endif // !NINA_WIN32
};

NINA_END_NAMESPACE_DECL

# include "NinaTime.inl"

#endif // !__NINA_TIME_HPP__
