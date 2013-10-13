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
 * @file NinaEpollPolicy.cpp
 * @brief Implements the sychroneous event demultiplexer 'epoll' as concrete reactor implementation
 * @author Jonathan Calmels
 * @date Wed Jul 20 2011
 */

#include <utility>
#include <stdexcept>
#include <algorithm>
#include "NinaSystemError.hpp"
#include "NinaReactor.hpp"
#include "NinaEpollPolicy.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

EpollPolicy::EpollPolicy()
{
	mOpenMax = OS::getOpenMax();
	mFdQueue = ::epoll_create(1);
	if (mFdQueue == -1)
		throw Error::SystemError(errno);
	mFdSet = new epoll_event[mOpenMax];
	::memset(mFdSet, 0, mOpenMax * sizeof(epoll_event));
}

EpollPolicy::~EpollPolicy()
{
	delete[] mFdSet;
	::close(mFdQueue);
}

int
EpollPolicy::registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType)
{
	epoll_event				event;
	DemuxTable::iterator	i;
	int						op;

	if (handle == NINA_INVALID_HANDLE || eHandler == 0)
		return -1;
	try {
		mTable.at(handle);
		op = EPOLL_CTL_MOD;
	}
	catch (std::out_of_range) {
		op = EPOLL_CTL_ADD;
	}
	mTable[handle].first = eHandler;
	mTable[handle].second |= eType;
	i = mTable.find(handle);
	event.data.ptr = &*i;
	event.events = 0;
	if (eType & Events::READ)
		event.events |= EPOLLIN | EPOLLRDHUP;
	if (eType & Events::WRITE)
		event.events |= EPOLLOUT;
	if (eType & Events::URGENT)
		event.events |= EPOLLPRI;
	return ::epoll_ctl(mFdQueue, op, handle, &event);
}

int
EpollPolicy::removeHandler(NINAHandle handle, uint16_t eType)
{
	epoll_event				event;
	DemuxTable::iterator	i;
	int						op;

	try {
		HandleInfo& hInfo = mTable.at(handle);
		hInfo.second &= ~eType;
		//hInfo.first->handleClose(handle);
		if (hInfo.second == 0) {
			mTable.erase(handle);
			op = EPOLL_CTL_DEL;
		}
		else {
			op = EPOLL_CTL_MOD;
			i = mTable.find(handle);
			event.data.ptr = &*i;
			event.events = 0;
			if (hInfo.second & Events::READ)
				event.events |= EPOLLIN | EPOLLRDHUP;
			if (hInfo.second & Events::WRITE)
				event.events |= EPOLLOUT;
			if (hInfo.second & Events::URGENT)
				event.events |= EPOLLPRI;
		}
	}
	catch (std::out_of_range) {
		return -1;
	}
	return ::epoll_ctl(mFdQueue, op, handle, &event);
}

int
EpollPolicy::handleEvents(Time const* timeout)
{
	int							errCode;
	int							time;
	DemuxTableElement const*	elem;

	time = (timeout == 0) ? -1 : timeout->getSeconds() * Time::SEC_IN_MSEC +
								timeout->getUSeconds() / Time::MSEC_IN_USEC;	
	errCode = ::epoll_wait(mFdQueue, mFdSet, mOpenMax, time);
	if (errCode == NINA_ENDPOINT_ERROR && errno != EINTR)
		return -1;
	for (int n = 0; n < errCode; ++n)
	{
		elem = static_cast<DemuxTableElement const*> (mFdSet[n].data.ptr);
		if ((elem->second.second & Events::READ) &&
				(mFdSet[n].events & (EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0 && 
				dispatchEvent(Events::READ, elem) != NINA_INVALID_HANDLE)
			continue;
		if ((elem->second.second & Events::WRITE) && (mFdSet[n].events & EPOLLOUT) != 0 &&
				dispatchEvent(Events::WRITE, elem) != NINA_INVALID_HANDLE)
			continue;
		if ((elem->second.second & Events::URGENT) && (mFdSet[n].events & EPOLLPRI) != 0)
			dispatchEvent(Events::URGENT, elem);
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
