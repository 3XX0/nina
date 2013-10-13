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
 * @file NinaAddr.inl
 * @brief Implements the base system of addressing (inline functions)
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE bool
Addr::operator==(Addr const& sap) const
{
	if (mFamily == sap.mFamily
			&& mSize == sap.mSize)
		return true;
	return false;
}

NINA_INLINE bool
Addr::operator!=(Addr const& sap) const
{
	if (mFamily != sap.mFamily
			&& mSize != sap.mSize)
		return true;
	return false;
}

NINA_INLINE int
Addr::getFamily() const
{
	return mFamily;
}

NINA_INLINE void
Addr::setFamily(int family)
{
	mFamily = family;
}

NINA_INLINE int
Addr::getSize() const
{
	return mSize;
}

NINA_INLINE void
Addr::setSize(int size)
{
	mSize = size;
}

NINA_INLINE void const*
Addr::getAddr() const
{
	return 0;
}

NINA_INLINE int
Addr::setAddr(void* addr, size_t addrLen)
{
	NINA_UNUSED_ARG(addr);
	NINA_UNUSED_ARG(addrLen);
	return -1;
}

NINA_INLINE void
Addr::reset()
{
	mFamily = AF_ANY;
	mSize = -1;	
}

NINA_END_NAMESPACE_DECL
