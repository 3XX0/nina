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
 * @file NinaReactor.inl
 * @brief Implements the Nina reactor interface using the bridge pattern and a policy system (inline functions)
 * @author Jonathan Calmels
 * @date Mon Jul 25 2011
 */

NINA_BEGIN_NAMESPACE_DECL

template <class SYNC_POLICY> NINA_INLINE int
Reactor<SYNC_POLICY>::registerHandler(EventHandler* eHandler, uint16_t eType)
{
	return mReactImplement->registerHandler(eHandler, eType);
}

template <class SYNC_POLICY> NINA_INLINE int
Reactor<SYNC_POLICY>::registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType)
{
	return mReactImplement->registerHandler(handle, eHandler, eType);
}

template <class SYNC_POLICY> NINA_INLINE int
Reactor<SYNC_POLICY>::removeHandler(EventHandler* eHandler, uint16_t eType)
{
	return mReactImplement->removeHandler(eHandler, eType);
}

template <class SYNC_POLICY> NINA_INLINE int
Reactor<SYNC_POLICY>::removeHandler(NINAHandle handle, uint16_t eType)
{
	return mReactImplement->removeHandler(handle, eType);
}

template <class SYNC_POLICY> NINA_INLINE int
Reactor<SYNC_POLICY>::handleEvents(Time const* timeout)
{
	return mReactImplement->handleEvents(timeout);
}

NINA_END_NAMESPACE_DECL
