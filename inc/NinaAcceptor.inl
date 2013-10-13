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
 * @file NinaAcceptor.inl
 * @brief Implements the acceptor design pattern (inline functions)
 * @author Jonathan Calmels
 * @date Sun Sep 04 2011
 */

NINA_BEGIN_NAMESPACE_DECL

template <class SVC_HANDLER, class IPC_ACCEPTOR, class SYNC_POLICY, bool THROW_ON_ERR> NINA_INLINE int
Acceptor<SVC_HANDLER, IPC_ACCEPTOR, SYNC_POLICY, THROW_ON_ERR>::handleRead(NINAHandle)
{
	return accept();
}

template <class SVC_HANDLER, class IPC_ACCEPTOR, class SYNC_POLICY, bool THROW_ON_ERR> NINA_INLINE NINAHandle
Acceptor<SVC_HANDLER, IPC_ACCEPTOR, SYNC_POLICY, THROW_ON_ERR>::getHandle() const
{
	return mIPCAcceptor.getHandle();
}

template <class SVC_HANDLER, class IPC_ACCEPTOR, class SYNC_POLICY, bool THROW_ON_ERR> NINA_INLINE int
Acceptor<SVC_HANDLER, IPC_ACCEPTOR, SYNC_POLICY, THROW_ON_ERR>::suspend()
{
	mIsSuspended = true;
	return mReactor->removeHandler(this, Events::ACCEPT);
}

template <class SVC_HANDLER, class IPC_ACCEPTOR, class SYNC_POLICY, bool THROW_ON_ERR> NINA_INLINE int
Acceptor<SVC_HANDLER, IPC_ACCEPTOR, SYNC_POLICY, THROW_ON_ERR>::resume()
{
	return mReactor->registerHandler(this, Events::ACCEPT);
}

template <class SVC_HANDLER, class IPC_ACCEPTOR, class SYNC_POLICY, bool THROW_ON_ERR> NINA_INLINE SVC_HANDLER*
Acceptor<SVC_HANDLER, IPC_ACCEPTOR, SYNC_POLICY, THROW_ON_ERR>::makeServiceHandler() const
{
	return new SVC_HANDLER;
}

NINA_END_NAMESPACE_DECL
