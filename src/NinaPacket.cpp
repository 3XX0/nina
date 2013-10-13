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
 * @file NinaPacket.cpp
 * @brief Implements the packets wrapping of NINA api
 * @author Jonathan Calmels
 * @date Fri Nov 11 2011
 */

#include "NinaPacket.hpp"

NINA_BEGIN_NAMESPACE_DECL

OutputPacket::OutputPacket(std::string const& str)
{
	mStream.str(str);
}

OutputPacket::OutputPacket(OutputPacket const& opack)
{
	mStream.str(opack.mStream.str());
}

OutputPacket&
OutputPacket::operator=(OutputPacket const& opack)
{
	if (this != &opack) {
		mStream.str(opack.mStream.str());
	}
	return *this;
}

InputPacket::InputPacket(std::string const& str)
{
	mStream.str(str);
}

InputPacket::InputPacket(InputPacket const& ipack)
{
	mStream.str(ipack.mStream.str());
}

InputPacket&
InputPacket::operator=(InputPacket const& ipack)
{
	if (this != &ipack) {
		mStream.str(ipack.mStream.str());
	}
	return *this;
}

InputPacket&
InputPacket::operator>>(CDR::String& str)
{
	char	c;

	str.erase();
	while ((c = mStream.sbumpc()) != '\0' && c != EOF)
		str.push_back(c);
	return *this;
}

void
InputPacket::peek(CDR::Short& value) const
{
	CDR::Short		hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohs(hostValue);
}

void
InputPacket::peek(CDR::UShort& value) const
{
	CDR::UShort		hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohs(hostValue);
}

void
InputPacket::peek(CDR::Integer& value) const
{
	CDR::Integer	hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohl(hostValue);
}

void
InputPacket::peek(CDR::UInteger& value) const
{
	CDR::UInteger	hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohl(hostValue);
}

void
InputPacket::peek(CDR::Long& value) const
{
	CDR::Long		hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohll(hostValue);
}

void
InputPacket::peek(CDR::ULong& value) const
{
	CDR::ULong		hostValue;
	std::streamsize	sz;

	sz = mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
	value = ntohll(hostValue);
}


void
InputPacket::peek(CDR::String& str) const
{
	char			c;
	std::streamsize	sz = 0;

	str.erase();
	while ((c = mStream.sbumpc()) != '\0' && c != EOF) {
		str.push_back(c);
		sz++;
	}
	mStream.pubseekoff(-sz, std::ios_base::cur, std::ios_base::in);
}

void
InputPacket::get(CDR::String& str, size_t len)
{
	char			c;

	str.erase();
	while ((c = mStream.sbumpc()) != EOF && len > 0) {
		str.push_back(c);
		len--;
	}
}

NINA_END_NAMESPACE_DECL
