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
 * @file NinaIOContainer.cpp
 * @brief Implementation of Input/Output container
 * @author Jonathan Calmels
 * @date Wed Jun 08 2011
 */

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include "NinaIOContainer.hpp"

NINA_BEGIN_NAMESPACE_DECL

IOContainer::IOContainer()
	: mConcreteContainer(0)
{
}

IOContainer::~IOContainer()
{
	cleanup();
	clear();
}

IOContainer::IOContainer(IOContainer const& ioc)
{
	mContainer = ioc.mContainer;
	if (ioc.mConcreteContainer == 0)
		mConcreteContainer = 0;
	else
		::memcpy(mConcreteContainer, ioc.mConcreteContainer, sizeof *mConcreteContainer);
}

IOContainer&
IOContainer::operator=(IOContainer const& ioc)
{
	if (this != &ioc) {
		mContainer = ioc.mContainer;
		if (mConcreteContainer != 0)
			delete[] mConcreteContainer;
		if (ioc.mConcreteContainer == 0)
			mConcreteContainer = 0;
		else
			::memcpy(mConcreteContainer, ioc.mConcreteContainer, sizeof *mConcreteContainer);
	}
	return *this;
}

void
IOContainer::remove(size_t idx)
{
	IOVector::iterator i;

	try {
		if ((i = std::find(mContainer.begin(), mContainer.end(),
					   	mContainer.at(idx))) != mContainer.end())
			mContainer.erase(i);
	}
	catch (std::out_of_range) {}
}

#if defined (NINA_WIN32)
LPWSABUF
IOContainer::operator()()
{
	int n = 0;

	mConcreteContainer = new WSABUF[mContainer.size()];
	for (IOVector::const_iterator i = mContainer.begin();
			i != mContainer.end(); ++i) {
		mConcreteContainer[n].buf = static_cast<char*> (i->first);
		mConcreteContainer[n].len = static_cast<unsigned long> (i->second);
		++n;
	}
	return mConcreteContainer;
}
#else
iovec const*
IOContainer::operator()()
{
	int n = 0;

	mConcreteContainer = new iovec[mContainer.size()];
	for (IOVector::const_iterator i = mContainer.begin();
			i != mContainer.end(); ++i) {
		mConcreteContainer[n].iov_base = i->first;
		mConcreteContainer[n].iov_len = i->second;
		++n;
	}
	return mConcreteContainer;
}
#endif // !NINA_WIN32

NINA_END_NAMESPACE_DECL
