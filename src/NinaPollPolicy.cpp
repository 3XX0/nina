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
 * @file NinaPollPolicy.cpp
 * @brief Implements the synchroneous event demultiplexer 'poll' as concrete reactor implementation
 * @author Jonathan Calmels
 * @date Wed Jul 20 2011
 */

#include <utility>
#include <stdexcept>
#include <algorithm>
#include "NinaReactor.hpp"
#include "NinaPollPolicy.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

PollPolicy::PollPolicy()
	: mFdSetIndex(0)
{
	mOpenMax = OS::getOpenMax();
	mFdSet = new OS::NINAPollData[mOpenMax];
	::memset(mFdSet, 0, mOpenMax * sizeof(OS::NINAPollData));
	for (long i = 0; i < mOpenMax; ++i)
		mFdSet[i].fd = -1;
}

PollPolicy::~PollPolicy()
{
	delete[] mFdSet;
}

int
PollPolicy::registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType)
{
	size_t	idx;

	if (handle == NINA_INVALID_HANDLE || eHandler == 0)
		return -1;
	try {
		mTable.at(handle);
		idx = mHandleToIndex[handle];
	}
	catch (std::out_of_range) {
		if (mAvailableIndex.empty()) {
			if (mFdSetIndex == static_cast<size_t> (mOpenMax))
				return -1;
			idx = mFdSetIndex;
			++mFdSetIndex;
		}
		else {
			idx = mAvailableIndex.top();
			mAvailableIndex.pop();
		}
		mFdSet[idx].fd = handle;
		mHandleToIndex[handle] = idx;
	}
	mTable[handle].first = eHandler;
	mTable[handle].second |= eType;
	if (eType & Events::READ)
		mFdSet[idx].events |= POLLRDNORM;
	if (eType & Events::WRITE)
		mFdSet[idx].events |= POLLWRNORM;
	if (eType & Events::URGENT)
		mFdSet[idx].events |= POLLPRI;
	return 0;
}

int
PollPolicy::removeHandler(NINAHandle handle, uint16_t eType)
{
	size_t	idx;

	try {
		HandleInfo& hInfo = mTable.at(handle);
		hInfo.second &= ~eType;
		//hInfo.first->handleClose(handle);
		idx = mHandleToIndex[handle];
		if (hInfo.second == 0) {
			mTable.erase(handle);
			mHandleToIndex.erase(handle);
			mFdSet[idx].fd = -1;
			mFdSet[idx].events = 0;
			mFdSet[idx].revents = 0;
			mAvailableIndex.push(idx);
			return 0;
		}
		if (eType & Events::READ)
			mFdSet[idx].events &= ~POLLRDNORM;
		if (eType & Events::WRITE)
			mFdSet[idx].events &= ~POLLWRNORM;
		if (eType & Events::URGENT)
			mFdSet[idx].events &= ~POLLPRI;
	}
	catch (std::out_of_range) {
		return -1;
	}
	return 0;
}

// errCode may be used to break the for loop for sake of optimisation
int
PollPolicy::handleEvents(Time const* timeout)
{
	int		errCode;
	size_t	idx;
	
	errCode = OS::poll(mFdSet, mFdSetIndex, timeout);
	if (errCode == NINA_ENDPOINT_ERROR) {
		OS::setErrnoToWSALastError();
#if defined (NINA_POSIX)
		if (errno != EINTR)
#endif // !NINA_POSIX
			return -1;
	}
	DemuxTable::const_iterator i = mTable.begin();
	DemuxTable::const_iterator j = mTable.begin();
	if (i != mTable.end())
		std::advance(j, 1);
	while (i != mTable.end()) {
		if (errCode == 0)
			dispatchEvent(Events::TIME_OUT, &*i);
#if defined (NINA_POSIX)
		else if (errCode == NINA_ENDPOINT_ERROR && errno == EINTR)
			dispatchEvent(Events::SIGNAL, &*i);
#endif // !NINA_POSIX
		else {
			idx = mHandleToIndex[i->first];
			if ((i->second.second & Events::READ) &&
					(mFdSet[idx].revents & (POLLRDNORM | POLLERR | POLLHUP)) &&
					dispatchEvent(Events::READ, &*i) != NINA_INVALID_HANDLE)
				goto it_copy;
			if ((i->second.second & Events::WRITE) && (mFdSet[idx].revents & POLLWRNORM) &&
					dispatchEvent(Events::WRITE, &*i) != NINA_INVALID_HANDLE)
				goto it_copy;
			if ((i->second.second & Events::URGENT) && (mFdSet[idx].revents & POLLPRI))
				dispatchEvent(Events::URGENT, &*i);
		}
		it_copy:
			i = j;
			if (j != mTable.end())
				++j;
	}
	return 0;
}

NINA_END_NAMESPACE_DECL
