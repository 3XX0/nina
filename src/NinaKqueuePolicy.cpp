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
 * @file NinaKqueuePolicy.cpp
 * @brief Implements the sychroneous event demultiplexer 'kqueue' as concrete reactor implementation
 * @author Jonathan Calmels
 * @date Wed Jul 20 2011
 */

#include <utility>
#include <stdexcept>
#include <algorithm>
#include "NinaSystemError.hpp"
#include "NinaReactor.hpp"
#include "NinaKqueuePolicy.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

KqueuePolicy::KqueuePolicy()
{
	mOpenMax = OS::getOpenMax() * 2;
	mFdQueue = ::kqueue();
	if (mFdQueue == -1)
		throw Error::SystemError(errno);
	mFdSet = new struct kevent[mOpenMax];
	::memset(mFdSet, 0, mOpenMax * sizeof(struct kevent));
}

KqueuePolicy::~KqueuePolicy()
{
	::close(mFdQueue);
	delete[] mFdSet;
}

int
KqueuePolicy::registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType)
{
	int						errCode;
	struct kevent			event[2];
	DemuxTable::iterator	i;
	int						nbEvents = 0;

	if (handle == NINA_INVALID_HANDLE || eHandler == 0)
		return -1;
	mTable[handle].first = eHandler;
	mTable[handle].second |= eType;
	i = mTable.find(handle);
	EV_SET(&event[0], handle, 0, EV_ADD, 0, 0, &*i);
	EV_SET(&event[1], handle, 0, EV_ADD, 0, 0, &*i);
	if (eType & Events::READ) {
		event[0].filter = EVFILT_READ;
		++nbEvents;
	}
	if (eType & Events::WRITE) {
		event[nbEvents].filter = EVFILT_WRITE;
		++nbEvents;
	}
	errCode = ::kevent(mFdQueue, event, nbEvents, 0, 0, 0);
	if (errCode > 0 && mFdSet[0].flags == EV_ERROR) {
		errno = mFdSet[0].data;
		return -1;
	}
	return errCode;
}

int
KqueuePolicy::removeHandler(NINAHandle handle, uint16_t eType)
{
	struct kevent			event[2];
	int						errCode;
	int						nbEvents = 0;

	try {
		HandleInfo& hInfo = mTable.at(handle);
		hInfo.second &= ~eType;
		//hInfo.first->handleClose(handle);
		EV_SET(&event[0], handle, 0, EV_DISABLE | EV_DELETE, 0, 0, 0);
		EV_SET(&event[1], handle, 0, EV_DISABLE | EV_DELETE, 0, 0, 0);
		if (hInfo.second == 0)
			mTable.erase(handle);
		if (eType & Events::READ) {
			event[0].filter = EVFILT_READ;
			++nbEvents;
		}
		if (eType & Events::WRITE) {
			event[nbEvents].filter = EVFILT_WRITE;
			++nbEvents;
		}
		errCode = ::kevent(mFdQueue, event, nbEvents, 0, 0, 0);
		if (errCode > 0 && mFdSet[0].flags == EV_ERROR) {
			errno = mFdSet[0].data;
			return -1;
		}
	}
	catch (std::out_of_range) {
		return -1;
	}
	return errCode;
}

int
KqueuePolicy::handleEvents(Time const* timeout)
{
	int							errCode;
	DemuxTableElement const*	elem;
	NINAHandle					skipHandle = NINA_INVALID_HANDLE;

	timespec const*	time = (timeout == 0) ? 0 : timeout->getTimespec();
	errCode = ::kevent(mFdQueue, 0, 0, mFdSet, mOpenMax, time);
	if (errCode == NINA_ENDPOINT_ERROR && errno != EINTR)
			return -1;
	for (int n = 0; n < errCode; ++n) {
		elem = static_cast<DemuxTableElement const*> (mFdSet[n].udata);
		if (static_cast<NINAHandle> (mFdSet[n].ident) != skipHandle && (elem->second.second & Events::READ) 
				&& mFdSet[n].filter == EVFILT_READ)
			skipHandle = dispatchEvent(Events::READ, elem); 
		if (static_cast<NINAHandle> (mFdSet[n].ident) != skipHandle && (elem->second.second & Events::WRITE)
			   	&& mFdSet[n].filter == EVFILT_WRITE)
			skipHandle = dispatchEvent(Events::WRITE, elem);
	}
	if (errCode == 0 || errCode == NINA_ENDPOINT_ERROR) {
		DemuxTable::const_iterator i = mTable.begin();
		DemuxTable::const_iterator j = mTable.begin();
		if (i != mTable.end())
			std::advance(j, 1);
		while (i != mTable.end()) {
			if (errCode == 0)
				dispatchEvent(Events::TIME_OUT, &*i);
			else if (errCode == NINA_ENDPOINT_ERROR)
				dispatchEvent(Events::SIGNAL, &*i);
			i = j;
			if (j != mTable.end())
				++j;
		}
	}
	return 0;
}

NINA_END_NAMESPACE_DECL
