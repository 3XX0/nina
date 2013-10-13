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
 * @file NinaServiceHandler.inl
 * @brief Implements an half of an end-to-end service (inline functions)
 * @author Jonathan Calmels
 * @date Sat Sep 03 2011
 */

NINA_BEGIN_NAMESPACE_DECL

template <class IPC_STREAM> NINA_INLINE NINAHandle
ServiceHandler<IPC_STREAM>::getHandle() const
{
	return mIPCStream.getHandle();
}

template <class IPC_STREAM> NINA_INLINE IPC_STREAM&
ServiceHandler<IPC_STREAM>::getPeer()
{
	return mIPCStream;
}

template <class IPC_STREAM> NINA_INLINE typename ServiceHandler<IPC_STREAM>::_Addr& 
ServiceHandler<IPC_STREAM>::getRemoteAddr()
{
	mIPCStream.getRemoteAddr(mIPCAddr);
	return mIPCAddr;
}

NINA_END_NAMESPACE_DECL
