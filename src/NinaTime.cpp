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
 * @file NinaTime.cpp
 * @brief Implements time specifications
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#include "NinaTime.hpp"

NINA_BEGIN_NAMESPACE_DECL

Time const Time::timeNull(0, 0);

Time::Time(time_t sec, long uSec)
{
	set(sec, uSec);
	if (sec != 0 || uSec != 0)
		normalize();
}

Time::~Time()
{
}

Time::Time(Time const& time)
{
#if defined (NINA_WIN32)
	::memcpy(&mExportTimeValue, &time.mExportTimeValue, sizeof mExportTimeValue);
#endif // !NINA_WIN32
	::memcpy(&mTimeValue, &time.mTimeValue, sizeof mTimeValue);
}

Time&
Time::operator=(Time const& time)
{
	if (this != &time) {
#if defined (NINA_WIN32)
		::memcpy(&mExportTimeValue, &time.mExportTimeValue, sizeof(mExportTimeValue));
#endif // !NINA_WIN32
		::memcpy(&mTimeValue, &time.mTimeValue, sizeof mTimeValue);
	}
	return *this;
}

Time&
Time::operator+=(Time const& time)
{
	if (time != timeNull) {
		mTimeValue.tv_sec += time.mTimeValue.tv_sec;
		mTimeValue.tv_usec += time.mTimeValue.tv_usec;
		normalize();
	}
	return *this;
}

Time&
Time::operator-=(Time const& time)
{
	if (time != timeNull) {
		mTimeValue.tv_sec -= time.mTimeValue.tv_sec;
		mTimeValue.tv_usec -= time.mTimeValue.tv_usec;
		normalize();
	}
	return *this;
}

Time&
Time::operator*=(double factor)
{
	if (factor == 0)
		*this = timeNull;
	else {
		mTimeValue.tv_sec = static_cast<time_t> (mTimeValue.tv_sec * factor);
		mTimeValue.tv_usec = static_cast<long> (mTimeValue.tv_usec * factor);
		normalize();
	}
	return *this;
}

Time
Time::operator+(Time const& time)
{
	Time copy(*this);

	if (time != timeNull) {
		copy += time;
		normalize();
	}
	return copy;
}

Time
Time::operator-(Time const& time)
{
	Time copy(*this);

	if (time != timeNull) {
		copy -= time;
		normalize();
	}
	return copy;
}

Time
Time::operator*(double factor)
{
	Time copy(*this);

	if (factor == 0)
		return Time(timeNull);
	copy *= factor;
	normalize();
	return copy;
}

void
Time::operator++()
{
	mTimeValue.tv_sec += 1;
	mTimeValue.tv_usec += 1;
	normalize();
}

void
Time::operator--()
{
	mTimeValue.tv_sec -= 1;
	mTimeValue.tv_usec -= 1;
	normalize();
}

void
Time::normalize()
{
	time_t tmp;

	tmp = mTimeValue.tv_sec + mTimeValue.tv_usec / SEC_IN_USEC;
	if (tmp >= 0) {
		mTimeValue.tv_sec = tmp;
		mTimeValue.tv_usec %= SEC_IN_USEC;
	}
	while (mTimeValue.tv_sec > 0 && mTimeValue.tv_usec < 0) {
		--mTimeValue.tv_sec;
		mTimeValue.tv_usec += SEC_IN_USEC;
	}
}

NINA_END_NAMESPACE_DECL
