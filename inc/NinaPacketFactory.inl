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
 * @file NinaPacketFactory.inl
 * @brief Implements a packet manager to craft packets upon bytes stream protocols (inline functions)
 * @author Jonathan Calmels
 * @date Fri Nov 11 2011
 */

NINA_BEGIN_NAMESPACE_DECL

template <class IN_PACKET> NINA_INLINE bool
PacketFactory<IN_PACKET>::empty() const
{
	return mList.empty();
}

template <class IN_PACKET> NINA_INLINE size_t
PacketFactory<IN_PACKET>::size() const
{
	return mList.size();
}

template <class IN_PACKET> NINA_INLINE void
PacketFactory<IN_PACKET>::sort()
{
	return mList.sort();
}

template <class IN_PACKET> NINA_INLINE void
PacketFactory<IN_PACKET>::clear()
{
	return mList.clear();
}

template <class IN_PACKET> NINA_INLINE void
PacketFactory<IN_PACKET>::updatePayload()
{
	for (size_t i = mWheel.rPtr; i != mWheel.wPtr; i = (i + 1) % BUF_SIZE)
		mPayload.push_back(mWheel.data[i]);
}

template <class IN_PACKET> NINA_INLINE void
PacketFactory<IN_PACKET>::forgePacket()
{
	if (mPayload.size() > 0) {
		IN_PACKET ipacket;
		ipacket.assign(mPayload);
		mPayload.clear();
		mList.push_back(ipacket);
	}
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::iterator
PacketFactory<IN_PACKET>::begin()
{
	return mList.begin();
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::iterator
PacketFactory<IN_PACKET>::end()
{
	return mList.end();
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::const_iterator
PacketFactory<IN_PACKET>::begin() const
{
	return mList.begin();
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::const_iterator
PacketFactory<IN_PACKET>::end() const
{
	return mList.end();
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::iterator
PacketFactory<IN_PACKET>::remove(iterator pos)
{
	return mList.erase(pos);
}

template <class IN_PACKET> NINA_INLINE typename PacketFactory<IN_PACKET>::iterator
PacketFactory<IN_PACKET>::remove(iterator first, iterator last)
{
	return mList.erase(first, last);
}

NINA_END_NAMESPACE_DECL
