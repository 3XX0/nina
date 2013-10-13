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
 * @file NinaTime.inl
 * @brief Implements time specifications (inline functions)
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE timeval const*
Time::getTimeval() const
{
#if defined (NINA_WIN32)
	//! @todo Requires valid truncation operations (see ACE::Truncate)
	mExportTimeValue.tv_sec = static_cast<long> (mTimeValue.tv_sec);
	mExportTimeValue.tv_usec = mTimeValue.tv_usec;
	return &mExportTimeValue;
#else
	return &mTimeValue;
#endif // !NINA_WIN32
}

#if defined (NINA_POSIX)
NINA_INLINE timespec const*
Time::getTimespec() const
{
	mTimeSpecValue.tv_sec = mTimeValue.tv_sec;
	mTimeSpecValue.tv_nsec = mTimeValue.tv_usec * USEC_IN_NSEC;
	return &mTimeSpecValue;
}
#endif // !NINA_POSIX

NINA_INLINE bool
Time::operator==(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) == 0)
		return true;
	return false;
}

NINA_INLINE bool
Time::operator!=(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) != 0)
		return true;
	return false;
}

NINA_INLINE bool
Time::operator<(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) < 0)
		return true;
	return false;
}

NINA_INLINE bool
Time::operator>(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) > 0)
		return true;
	return false;
}

NINA_INLINE bool
Time::operator<=(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) <= 0)
		return true;
	return false;
}

NINA_INLINE bool
Time::operator>=(Time const& time) const
{
	if (::memcmp(&mTimeValue, &time.mTimeValue, sizeof mTimeValue) >= 0)
		return true;
	return false;
}

NINA_INLINE void
Time::set(time_t sec, long uSec)
{
	mTimeValue.tv_sec = sec;
	mTimeValue.tv_usec = uSec;
}

NINA_INLINE time_t
Time::getSeconds() const
{
	return mTimeValue.tv_sec;
}

NINA_INLINE long
Time::getUSeconds() const
{
	return mTimeValue.tv_usec;
}

NINA_INLINE int
Time::timestamp()
{
	return OS::getCurrentTime(&mTimeValue);
}

NINA_END_NAMESPACE_DECL
