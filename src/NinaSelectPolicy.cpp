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
 * @file NinaSelectPolicy.cpp
 * @brief Implements the sychroneous event demultiplexer 'select' as concrete reactor implementation
 * @author Jonathan Calmels
 * @date Wed Jul 20 2011
 */

#include <utility>
#include <stdexcept>
#include <algorithm>
#include "NinaReactor.hpp"
#include "NinaSelectPolicy.hpp"
#include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

SelectPolicy::SelectPolicy()
{
	FD_ZERO(&mReadSet);
	FD_ZERO(&mWriteSet);
	FD_ZERO(&mExceptSet);
}

SelectPolicy::~SelectPolicy()
{
}

int
SelectPolicy::registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType)
{
	if (handle == NINA_INVALID_HANDLE || eHandler == 0)
		return -1;
	mTable[handle].first = eHandler;
	mTable[handle].second |= eType;
	if (eType & Events::READ)
		FD_SET(handle, &mReadSet);
	if (eType & Events::WRITE)
		FD_SET(handle, &mWriteSet);
	if (eType & Events::URGENT)
		FD_SET(handle, &mExceptSet);
	return 0;
}

int
SelectPolicy::removeHandler(NINAHandle handle, uint16_t eType)
{
	try {
		HandleInfo& hInfo = mTable.at(handle);
		hInfo.second &= ~eType;
		//hInfo.first->handleClose(handle);
		if (hInfo.second == 0)
			mTable.erase(handle);
		if (eType & Events::READ)
			FD_CLR(handle, &mReadSet);
		if (eType & Events::WRITE)
			FD_CLR(handle, &mWriteSet);
		if (eType & Events::URGENT)
			FD_CLR(handle, &mExceptSet);
	}
	catch (std::out_of_range) {
		return -1;
	}
	return 0;
}

// errCode may be used to break the for loop for sake of optimisation
// So check if select implementation varies around OS
int
SelectPolicy::handleEvents(Time const* timeout)
{
	int							errCode;
	int							fdLen;
	fd_set						readSet = mReadSet;
	fd_set						writeSet = mWriteSet;
	fd_set						exceptSet = mExceptSet;

	fdLen = mTable.empty() ? 0 : mTable.rbegin()->first + 1;
	errCode = OS::select(fdLen, &readSet, &writeSet, &exceptSet, timeout);
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
			if ((i->second.second & Events::READ) && FD_ISSET(i->first, &readSet) != 0 &&
				   	dispatchEvent(Events::READ, &*i) != NINA_INVALID_HANDLE)
				goto it_copy;
			if ((i->second.second & Events::WRITE) && FD_ISSET(i->first, &writeSet) != 0 &&
				   	dispatchEvent(Events::WRITE, &*i) != NINA_INVALID_HANDLE)
				goto it_copy;
			if ((i->second.second & Events::URGENT) && FD_ISSET(i->first, &exceptSet) != 0)
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
