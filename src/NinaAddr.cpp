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
 * @file NinaAddr.cpp
 * @brief Implementing the base system of addressing
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#include "NinaAddr.hpp"

NINA_BEGIN_NAMESPACE_DECL

Addr const Addr::sapAny(Addr::AF_ANY, -1);

Addr::Addr(int family, int size)
	: mFamily(family),
	mSize(size)
{
}

Addr::Addr(Addr const& sap)
	: mFamily(sap.mFamily),
	mSize(sap.mSize)
{
}

Addr& 
Addr::operator=(Addr const& sap)
{
	if (this != &sap) {
		mFamily = sap.mFamily;
		mSize = sap.mSize;
	}
	return *this;
}

NINA_END_NAMESPACE_DECL


