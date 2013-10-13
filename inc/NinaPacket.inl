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
 * @file NinaPacket.inl
 * @brief Implements the packets wrapping of NINA api (inline functions)
 * @author Jonathan Calmels
 * @date Fri Nov 11 2011
 */

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE bool
OutputPacket::operator==(OutputPacket const& opack) const
{
	return (dump() == opack.dump());
}

NINA_INLINE bool
OutputPacket::operator!=(OutputPacket const& opack) const
{
	return (dump() != opack.dump());
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::Boolean value)
{
	mStream.sputc(static_cast<char>(value));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::Char value)
{
	mStream.sputc(value);
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::UChar value)
{
	mStream.sputc(value);
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::Short value)
{
	CDR::Short netValue = htons(value);

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::UShort value)
{
	CDR::UShort netValue = htons(value);

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::Integer value)
{
	CDR::Integer netValue = htonl(value);

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::UInteger value)
{
	CDR::UInteger netValue = htonl(value);

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::Long value)
{
	CDR::Long netValue = static_cast<CDR::Long> (htonll(value));

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::ULong value)
{
	CDR::ULong netValue = htonll(value);

	mStream.sputn(reinterpret_cast<char const*> (&netValue), sizeof(netValue));
	return *this;
}

NINA_INLINE OutputPacket&
OutputPacket::operator<<(CDR::String const& str)
{
	mStream.sputn(str.c_str(), str.size());
	return *this;
}

NINA_INLINE std::string
OutputPacket::dump() const
{
	return mStream.str();
}

NINA_INLINE void
OutputPacket::assign(std::string const& str)
{
	clear();
	mStream.str(str);
}

NINA_INLINE void
OutputPacket::clear()
{
	mStream.str("");
	mStream.pubseekpos(0);
}

NINA_INLINE bool
InputPacket::operator==(InputPacket const& ipack) const
{
	return (dump() == ipack.dump());
}

NINA_INLINE bool
InputPacket::operator!=(InputPacket const& ipack) const
{
	return (dump() != ipack.dump());
}

NINA_INLINE bool
InputPacket::operator<(InputPacket const&) const
{
	return false;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::Boolean& value)
{
	value = (mStream.sbumpc() != 0);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::Char& value)
{
	value = mStream.sbumpc();
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::UChar& value)
{
	value = mStream.sbumpc();
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::Short& value)
{
	CDR::Short	hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohs(hostValue);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::UShort& value)
{
	CDR::UShort	hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohs(hostValue);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::Integer& value)
{
	CDR::Integer hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohl(hostValue);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::UInteger& value)
{
	CDR::UInteger hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohl(hostValue);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::Long& value)
{
	CDR::Long hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohll(hostValue);
	return *this;
}

NINA_INLINE InputPacket&
InputPacket::operator>>(CDR::ULong& value)
{
	CDR::ULong hostValue;

	mStream.sgetn(reinterpret_cast<char*> (&hostValue), sizeof(hostValue));
	value = ntohll(hostValue);
	return *this;
}

NINA_INLINE void
InputPacket::peek(CDR::Boolean& value) const
{
	value = (mStream.sgetc() != 0);
}

NINA_INLINE void
InputPacket::peek(CDR::Char& value) const
{
	value = mStream.sgetc();
}

NINA_INLINE void
InputPacket::peek(CDR::UChar& value) const
{
	value = mStream.sgetc();
}


NINA_INLINE std::string
InputPacket::dump() const
{
	return mStream.str();
}

NINA_INLINE void
InputPacket::assign(std::string const& str)
{
	clear();
	mStream.str(str);
}

NINA_INLINE void
InputPacket::clear()
{
	mStream.str("");
	mStream.pubseekpos(0);
}

NINA_END_NAMESPACE_DECL
