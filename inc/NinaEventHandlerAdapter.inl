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
 * @file NinaEventHandlerAdapter.inl
 * @brief Implements a class useful to handle events with functions pointers (inline functions)
 * @author Jonathan Calmels
 * @date Sat Jul 30 2011
 */

NINA_BEGIN_NAMESPACE_DECL

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setReadHandler(Handler readHandler)
{
	mReadHandler = readHandler;
}

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setWriteHandler(Handler writeHandler)
{
	mWriteHandler = writeHandler;
}

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setUrgentHandler(Handler urgentHandler)
{
	mUrgentHandler = urgentHandler;
}

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setTimeoutHandler(Handler timeoutHandler)
{
	mTimeoutHandler = timeoutHandler;
}

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setSignalHandler(Handler signalHandler)
{
	mSignalHandler = signalHandler;
}

template <class IPC_STREAM> NINA_INLINE void
EventHandlerAdapter<IPC_STREAM>::setCloseHandler(Handler closeHandler)
{
	mCloseHandler = closeHandler;
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleRead(NINAHandle)
{
	if (mReadHandler == 0)
		return 0;
	return (*mReadHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleWrite(NINAHandle)
{
	if (mWriteHandler == 0)
		return 0;
	return (*mWriteHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleUrgent(NINAHandle)
{
	if (mUrgentHandler == 0)
		return 0;
	return (*mUrgentHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleTimeout(NINAHandle)
{
	if (mTimeoutHandler == 0)
		return 0;
	return (*mTimeoutHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleSignal(NINAHandle)
{
	if (mSignalHandler == 0)
		return 0;
	return (*mSignalHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE int
EventHandlerAdapter<IPC_STREAM>::handleClose(NINAHandle)
{
	if (mCloseHandler == 0)
		return 0;
	return (*mCloseHandler)(mIPCStream);
}

template <class IPC_STREAM> NINA_INLINE NINAHandle 
EventHandlerAdapter<IPC_STREAM>::getHandle() const
{
	return mIPCStream.getHandle();
}

template <class IPC_STREAM> NINA_INLINE IPC_STREAM&
EventHandlerAdapter<IPC_STREAM>::getPeer()
{
	return mIPCStream;
}

NINA_END_NAMESPACE_DECL
